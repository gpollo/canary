#include "CanBus.hpp"
#include "UserConfiguration.hpp"

namespace Canary {

CanBus& CanBus::getInstance() {
    static CanBus instance;
    return instance;
}

CanBus::CanBus(QObject* parent)
    : QObject(parent),
      database_(std::make_shared<can::databases>()),
      listener_(std::make_shared<can::listener>(database_)) {
}

CanBus::~CanBus() {
    saveConfig();
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
    auto uniqueTransceiver = transceiver.get_unique_transceiver();
    if (uniqueTransceiver == nullptr) {
        return {};
    }

    return listener_->add_transceiver(std::move(uniqueTransceiver));
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

void CanBus::loadConfig() {
    nlohmann::json devicesConfig = nlohmann::json::array();
    nlohmann::json deviceConfig;
    deviceConfig["driver"]    = "";
    deviceConfig["interface"] = "";
    deviceConfig["bitrate"]   = 500000;
    devicesConfig.push_back(deviceConfig);

    nlohmann::json databasesConfig = nlohmann::json::array();
    databasesConfig.push_back("");

    nlohmann::json defaultConfig = {{"devices", devicesConfig}, {"databases", databasesConfig}};
    auto config = UserConfiguration::getKey("can-bus", defaultConfig);

    for (auto deviceConfig : config["devices"]) {
        auto canDevice = new CanDevice();
        auto driver = QString::fromStdString(deviceConfig["driver"].get<std::string>());
        auto interface = QString::fromStdString(deviceConfig["interface"].get<std::string>());
        auto bitrate = deviceConfig["bitrate"].get<unsigned int>();
        canDevice->setDriver(driver);
        canDevice->setInterface(interface);
        canDevice->setBitrate(bitrate);
        canDevice->connect();
        devices_.insert(devices_.end(), canDevice);
    }
    emit devicesChanged();

    for (auto databaseConfig : config["databases"]) {
        auto canDatabase = new CanDatabase();
        auto path = QString::fromStdString(databaseConfig.get<std::string>());
        canDatabase->setPath(path);
        canDatabase->connect();
        databases_.insert(databases_.end(), canDatabase);
    }
    emit databasesChanged();
}

void CanBus::saveConfig() const {
    nlohmann::json devicesConfig = nlohmann::json::array();
    for (const auto* device : devices_) {
        nlohmann::json deviceConfig;
        deviceConfig["driver"]    = device->getDriver().toStdString();
        deviceConfig["interface"] = device->getInterface().toStdString();
        deviceConfig["bitrate"]   = device->getBitrate();
        devicesConfig.push_back(deviceConfig);
    }

    nlohmann::json databasesConfig = nlohmann::json::array();
    for (const auto* database : databases_) {
        databasesConfig.push_back(database->getPath().toStdString());
    }

    nlohmann::json config = {{"devices", devicesConfig}, {"databases", databasesConfig}};
    UserConfiguration::setKey("can-bus", config);
}


} /* namespace Canary */
