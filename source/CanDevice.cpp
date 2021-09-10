#include "CanDevice.hpp"
#include "CanBus.hpp"
#include "Log.hpp"

namespace Canary {

CanDevice::CanDevice(QObject* parent) : QObject(parent) {}

void CanDevice::setDriver(const QString& driver) {
    if (isConnected()) {
        logger->error("cannot change driver when device is connected");
        return;
    }

    if (driver_ != driver) {
        driver_ = driver;
        emit driverChanged();
    }
}

QString CanDevice::getDriver() const {
    return driver_;
}

void CanDevice::setInterface(const QString& interface) {
    if (isConnected()) {
        logger->error("cannot change interface when device is connected");
        return;
    }

    if (interface_ != interface) {
        interface_ = interface;
        emit interfaceChanged();
    }
}

QString CanDevice::getInterface() const {
    return interface_;
}

void CanDevice::setBitrate(unsigned int bitrate) {
    if (isConnected()) {
        logger->error("cannot change bitrate when device is connected");
        return;
    }

    if (bitrate_ != bitrate) {
        bitrate_ = bitrate;
        emit bitrateChanged();
    }
}

unsigned int CanDevice::getBitrate() const {
    return bitrate_;
}

void CanDevice::setQuark(long long quark) {
    if (quark_ != quark) {
        quark_ = quark;
        emit connectedChanged();
    }
}

long long CanDevice::getQuark() const {
    return quark_;
}

bool CanDevice::isConnected() const {
    return (quark_ >= 0);
}

void CanDevice::connect() {
    auto driver      = driver_.toStdString();
    auto interface   = interface_.toStdString();
    auto transceiver = can::transceiver::create(driver, interface);
    if (transceiver == nullptr) {
        logger->error("failed to create device '{}@{}'", interface, driver);
        return;
    }

    auto quark = CanBus::getInstance().connectDevice(transceiver);
    if (quark.has_value()) {
        logger->info("connected device '{}@{}'", interface, driver);
        setQuark((long long)quark.value());
    } else {
        logger->error("failed to connect device '{}@{}'", interface, driver);
    }
}

void CanDevice::disconnect() {
    auto driver    = driver_.toStdString();
    auto interface = interface_.toStdString();

    if (quark_ >= 0) {
        logger->info("disconnected device '{}@{}'", interface, driver);
        CanBus::getInstance().disconnectDevice(quark_);
        setQuark(-1);
    } else {
        logger->error("device '{}@{}' is not connect", interface, driver);
    }
}

} /* namespace Canary */