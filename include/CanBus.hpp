#ifndef INCLUDE_CAN_BUS_HPP
#define INCLUDE_CAN_BUS_HPP

#include <optional>

#include <can/database.hpp>
#include <can/databases.hpp>
#include <can/listener.hpp>
#include <can/transceiver.hpp>
#include <can/types.hpp>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtQml/QtQml>

#include "CanDatabase.hpp"
#include "CanDevice.hpp"

namespace Canary {

class CanBus : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<Canary::CanDevice*> devices READ getDevices NOTIFY devicesChanged)
    Q_PROPERTY(unsigned int deviceCount READ getDeviceCount NOTIFY devicesChanged)
    Q_PROPERTY(QList<Canary::CanDatabase*> databases READ getDatabases NOTIFY databasesChanged)
    Q_PROPERTY(unsigned int databaseCount READ getDatabaseCount NOTIFY databasesChanged)
    QML_ELEMENT

   public:
    using Frame           = can::frame::ptr;
    using Message         = can::database::message::const_ptr;
    using Signal          = can::database::signal::const_ptr;
    using RawCallback     = can::listener::raw_callback;
    using MessageCallback = can::listener::message_callback;
    using SignalCallback  = can::listener::signal_callback;
    using SubscriberGuard = can::listener::subscriber_guard::ptr;
    using Quark           = can::quark;
    using SignalConstPtr  = can::database::signal::const_ptr;

    // using message_callback = std::function<void(database::message::const_ptr,
    //                                             const std::vector<std::pair<database::signal::const_ptr, float>>&)>;
    // using signal_callback  = std::function<void(database::signal::const_ptr, float)>;

    static CanBus& getInstance();

    ~CanBus();

    QList<CanDevice*> getDevices() const;
    unsigned int getDeviceCount() const;
    std::optional<Quark> connectDevice(can::transceiver::ptr& transceiver);
    void disconnectDevice(Quark transceiver);

    QList<CanDatabase*> getDatabases() const;
    unsigned int getDatabaseCount() const;
    std::optional<Quark> connectDatabase(can::database::ptr database);
    void disconnectDatabase(Quark database);

    SubscriberGuard subscribe(RawCallback callback);
    SubscriberGuard subscribe(unsigned int identifier, MessageCallback callback);
    SubscriberGuard subscribe(unsigned int identifier, const std::string& signalName, SignalCallback callback);

    std::vector<std::pair<SignalConstPtr, uint64_t>> extract(unsigned int identifier, uint8_t* bytes,
                                                             size_t length) const;

   public slots:
    void addDevice();
    void removeDevice(int index);

    void addDatabase();
    void removeDatabase(int index);

   signals:
    void devicesChanged();
    void databasesChanged();

   private:
    explicit CanBus(QObject* parent = nullptr);

    can::databases::ptr database_;
    can::listener::ptr listener_;

    QList<CanDevice*> devices_;
    QList<CanDatabase*> databases_;
};

} /* namespace Canary */

#endif /* INCLUDE_CAN_BUS_HPP */