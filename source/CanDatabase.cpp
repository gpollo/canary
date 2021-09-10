#include "CanDatabase.hpp"
#include "CanBus.hpp"
#include "Log.hpp"

namespace Canary {

CanDatabase::CanDatabase(QObject* parent) : QObject(parent) {}

void CanDatabase::setPath(const QString& path) {
    if (isConnected()) {
        logger->error("cannot change path when database is connected");
        return;
    }

    if (path_ != path) {
        path_ = path;
        emit pathChanged();
    }
}

QString CanDatabase::getPath() const {
    return path_;
}

void CanDatabase::setQuark(long long quark) {
    if (quark_ != quark) {
        quark_ = quark;
        emit connectedChanged();
    }
}

long long CanDatabase::getQuark() const {
    return quark_;
}

bool CanDatabase::isConnected() const {
    return (quark_ >= 0);
}

void CanDatabase::connect() {
    auto path     = path_.toStdString();
    auto database = can::database::create(path);
    if (database == nullptr) {
        logger->error("failed to open database '{}'", path);
        return;
    }

    auto quark = CanBus::getInstance().connectDatabase(database);
    if (quark.has_value()) {
        logger->info("connected database '{}'", path);
        setQuark((long long)quark.value());
    } else {
        logger->error("failed to connect database '{}'", path);
    }
}

void CanDatabase::disconnect() {
    auto path = path_.toStdString();

    if (quark_ >= 0) {
        logger->info("disconnected database '{}'", path);
        CanBus::getInstance().disconnectDatabase(quark_);
        setQuark(-1);
    } else {
        logger->error("database '{}' is not connect", path);
    }
}

} /* namespace Canary */