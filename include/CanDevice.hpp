#ifndef INCLUDE_CANARY_CAN_DEVICE_HPP
#define INCLUDE_CANARY_CAN_DEVICE_HPP

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtQml/QtQml>

namespace Canary {

class CanDevice : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString driver READ getDriver WRITE setDriver NOTIFY driverChanged)
    Q_PROPERTY(QString interface READ getInterface WRITE setInterface NOTIFY interfaceChanged)
    Q_PROPERTY(unsigned int bitrate READ getBitrate WRITE setBitrate NOTIFY bitrateChanged)
    Q_PROPERTY(long long quark READ getQuark WRITE setQuark)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    QML_ELEMENT

   public:
    explicit CanDevice(QObject* parent = nullptr);

    void setDriver(const QString& driver);
    QString getDriver() const;

    void setInterface(const QString& interface);
    QString getInterface() const;

    void setBitrate(unsigned int bitrate);
    unsigned int getBitrate() const;

    void setQuark(long long quark);
    long long getQuark() const;

    bool isConnected() const;

   public slots:
    void connect();
    void disconnect();

   signals:
    void driverChanged();
    void interfaceChanged();
    void bitrateChanged();
    void connectedChanged();

   private:
    QString driver_;
    QString interface_;
    unsigned int bitrate_ = 500000;
    long long quark_      = -1;
};

} /* namespace Canary */

#endif /* INCLUDE_CANARY_CAN_DEVICE_HPP */