#include "CanBus.hpp"
#include <memory>

namespace Canary {

CanBus& CanBus::getInstance() {
    static CanBus instance;
    return instance;
}

CanBus::CanBus(QObject* parent)
    : QObject(parent),
      database_(std::make_shared<can::databases>()),
      listener_(std::make_shared<can::listener>(database_)) {
    addDevice();
    addDatabase();
}

CanBus::~CanBus() {
    listener_->shutdown();
    listener_->shutdown();
}

QList<CanDevice*> CanBus::getDevices() const {
    return devices_;
}

unsigned int CanBus::getDeviceCount() const {
    return devices_.size();
}

std::optional<CanBus::Quark> CanBus::connectDevice(can::transceiver::ptr& transceiver) {
    auto unique_transceiver = transceiver.get_unique_transceiver();
    if (unique_transceiver == nullptr) {
        return {};
    }

    return listener_->add_transceiver(std::move(unique_transceiver));
}

void CanBus::disconnectDevice(Quark transceiver) {
    listener_->shutdown(transceiver);
}

QList<CanDatabase*> CanBus::getDatabases() const {
    return databases_;
}

unsigned int CanBus::getDatabaseCount() const {
    return databases_.size();
}

std::optional<CanBus::Quark> CanBus::connectDatabase(can::database::ptr database) {
    /* TODO: check if database already loaded */
    return database_->add(database);
}

void CanBus::disconnectDatabase(Quark database) {
    database_->remove(database);
}

CanBus::SubscriberGuard CanBus::subscribe(RawCallback callback) {
    return listener_->subscribe(callback);
}

CanBus::SubscriberGuard CanBus::subscribe(unsigned int identifier, MessageCallback callback) {
    return listener_->subscribe(identifier, callback);
}

CanBus::SubscriberGuard CanBus::subscribe(unsigned int identifier, const std::string& signalName,
                                          SignalCallback callback) {
    return listener_->subscribe(identifier, signalName, callback);
}

std::vector<std::pair<CanBus::SignalConstPtr, uint64_t>> CanBus::extract(unsigned int identifier, uint8_t* bytes,
                                                                         size_t length) const {
    return database_->extract(identifier, bytes, length);
}

void CanBus::addDevice() {
    devices_.insert(devices_.end(), new CanDevice());
    emit devicesChanged();
}

void CanBus::removeDevice(int index) {
    if (index < devices_.size()) {
        delete devices_.at(index);
        devices_.removeAt(index);
        emit devicesChanged();
    }
}

void CanBus::addDatabase() {
    databases_.insert(databases_.end(), new CanDatabase());
    emit databasesChanged();
}

void CanBus::removeDatabase(int index) {
    if (index < databases_.size()) {
        delete databases_.at(index);
        databases_.removeAt(index);
        emit databasesChanged();
    }
}

} /* namespace Canary */
