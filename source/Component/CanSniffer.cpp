#include <functional>
#include <iostream>

#include "CanBus.hpp"
#include "Component/CanSniffer.hpp"
#include "Log.hpp"
#include "spdlog/fmt/bundled/core.h"

namespace Canary::Component {

CanSniffer::CanSniffer(QObject* parent) : QAbstractListModel(parent), rawSubscriberGuard_(nullptr) {
    CanBus::RawCallback callback = std::bind(&CanSniffer::rawCallback, this, std::placeholders::_1);
    rawSubscriberGuard_          = std::move(CanBus::getInstance().subscribe(callback));

    QObject::connect(this, &CanSniffer::newEntry, this, &CanSniffer::onNewEntry);
}

CanSniffer::~CanSniffer() {}

int CanSniffer::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return entries_.size();
}

int CanSniffer::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 1;
}

bool CanSniffer::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount()) {
        return false;
    }

    switch (static_cast<Role>(role)) {
    case Role::Expanded:
        if (value.typeName() == QString("bool")) {
            entries_[index.row()].expanded_ = value.value<bool>();
            return true;
        }
        break;
    case Role::Focused:
        if (value.typeName() == QString("bool")) {
            entries_[index.row()].focused_ = value.value<bool>();
            return true;
        }
        break;
    default:
        return false;
    }

    return false;
}

QVariant CanSniffer::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount()) {
        return QVariant();
    }

    switch (static_cast<Role>(role)) {
    case Role::Timestamp:
        return entries_.at(index.row()).timestamp_;
    case Role::Identifier:
        return entries_.at(index.row()).identifier_;
    case Role::Bytes:
        return entries_.at(index.row()).bytes_;
    case Role::Values:
        return entries_.at(index.row()).values_;
    case Role::Expanded:
        return entries_.at(index.row()).expanded_;
    case Role::Focused:
        return entries_.at(index.row()).focused_;
    default:
        return QVariant();
    }
}

QVariant CanSniffer::headerData(int section, Qt::Orientation orientation, int role) const {
    switch (static_cast<Role>(role)) {
    case Role::Timestamp:
        return "Timestamp";
    case Role::Identifier:
        return "Identifier";
    case Role::Bytes:
        return "Bytes";
    case Role::Values:
        return "Values";
    case Role::Expanded:
        return "Expanded";
    case Role::Focused:
        return "Focused";
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> CanSniffer::roleNames() const {
    return {
        {static_cast<int>(Role::Timestamp), "timestamp"}, {static_cast<int>(Role::Identifier), "identifier"},
        {static_cast<int>(Role::Bytes), "bytes"},         {static_cast<int>(Role::Values), "values"},
        {static_cast<int>(Role::Expanded), "expanded"},   {static_cast<int>(Role::Focused), "focused"}
    };
}

void CanSniffer::rawCallback(const CanBus::Frame& frame) {
    QDateTime datetime = QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(frame->timestamp_ / 1000U));
    QString timestamp  = datetime.toString("dd/MM/yyyy hh:mm:ss.zzz");

    QList<QVariant> bytes;
    for (unsigned int i = 0; i < frame->length_; i++) {
        bytes.append(frame->bytes_[i]);
    }

    QList<QVariant> values;
    auto signal_values = CanBus::getInstance().extract(frame->identifier_, frame->bytes_, frame->length_);
    for (auto [signal, rawValue] : signal_values) {
        auto signalName    = QString::fromStdString(signal->get_name());
        auto resolvedValue = QString::fromStdString(signal->resolve(rawValue));

        QList<QVariant> pair;
        if (!resolvedValue.isEmpty()) {
            pair.push_back(signalName);
            pair.push_back(resolvedValue);
        } else {
            auto decodedValue = signal->decode(rawValue);
            auto signalUnit   = QString::fromStdString(signal->get_unit());

            QString formattedValue;
            if (signal->is_integral()) {
                if (signal->get_bit_count() == 1) {
                    formattedValue = (rawValue == 0) ? "false" : "true";
                } else {
                    formattedValue.setNum((long long)decodedValue);
                }
            } else {
                formattedValue.setNum(decodedValue);
            }

            if (!signalUnit.isEmpty()) {
                formattedValue.append(" ");
                formattedValue.append(signalUnit);
            }

            pair.push_back(signalName);
            pair.push_back(formattedValue);
        }

        values.push_back(pair);
    }

    emit newEntry(frame->identifier_, timestamp, bytes, values);
}

void CanSniffer::onNewEntry(unsigned int identifier, QString timestamp, QList<QVariant> bytes, QList<QVariant> values) {
    if (identifierIndices_.contains(identifier)) {
        auto row                  = identifierIndices_.at(identifier);
        auto index                = createIndex(row, 0);
        entries_[row].identifier_ = identifier;
        entries_[row].timestamp_  = timestamp;
        entries_[row].bytes_      = bytes;
        entries_[row].values_     = values;
        emit dataChanged(index, index);
    } else {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        entries_.emplace_back(identifier, std::move(timestamp), std::move(bytes), std::move(values));
        identifierIndices_.emplace(identifier, rowCount() - 1);
        endInsertRows();
    }
}

} /* namespace Canary::Component */