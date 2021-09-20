#ifndef INCLUDE_COMPONENT_CAN_SNIFFER_HPP
#define INCLUDE_COMPONENT_CAN_SNIFFER_HPP

#include <QtCore/QAbstractListModel>
#include <QtCore/QDateTime>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QString>
#include <QtQml/QtQml>

#include "CanBus.hpp"

namespace Canary::Component {

class CanSniffer : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

   public:
    enum class Role : int {
        Timestamp,
        Identifier,
        Bytes,
        Values,
        Expanded,
        Focused,
    };

    explicit CanSniffer(QObject* parent = nullptr);
    ~CanSniffer();

    /* QAbstractListModel methods */

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

   private:
    CanBus::SubscriberGuard rawSubscriberGuard_;

    struct Entry {
        unsigned int identifier_;
        QString timestamp_;
        QList<QVariant> bytes_;
        QList<QVariant> values_;
        bool expanded_;
        bool focused_;

        Entry(unsigned int identifier, QString timestamp, QList<QVariant> bytes, QList<QVariant> values)
            : identifier_(identifier),
              timestamp_(std::move(timestamp)),
              bytes_(std::move(bytes)),
              values_(std::move(values)),
              expanded_(false),
              focused_(false) {}
    };

    std::vector<Entry> entries_;
    std::unordered_map<unsigned int, unsigned int> identifierIndices_;

    void rawCallback(const CanBus::Frame& frame);

   signals:
    void newEntry(unsigned int identifier, QString timestamp, QList<QVariant> bytes, QList<QVariant> values);

   private slots:
    void onNewEntry(unsigned int identifier, QString timestamp, QList<QVariant> bytes, QList<QVariant> values);
};

} /* namespace Canary::Component */

#endif /* INCLUDE_COMPONENT_CAN_SNIFFER_HPP */