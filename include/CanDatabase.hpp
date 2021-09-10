#ifndef INCLUDE_CANARY_CAN_DATABASE_HPP
#define INCLUDE_CANARY_CAN_DATABASE_HPP

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtQml/QtQml>

namespace Canary {

class CanDatabase : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(long long quark READ getQuark WRITE setQuark)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)
    QML_ELEMENT

   public:
    explicit CanDatabase(QObject* parent = nullptr);

    void setPath(const QString& driver);
    QString getPath() const;

    void setQuark(long long quark);
    long long getQuark() const;

    bool isConnected() const;

   public slots:
    void connect();
    void disconnect();

   signals:
    void pathChanged();
    void connectedChanged();

   private:
    QString path_;
    long long quark_ = -1;
};

} /* namespace Canary */

#endif /* INCLUDE_CANARY_CAN_DATABASE_HPP */