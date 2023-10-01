#ifndef CONNMANMANAGER_H
#define CONNMANMANAGER_H

#include <QObject>
#include <QDBusVariant>
#include <QDBusMessage>
#include <QDBusInterface>
#include <QPair>
#include "ConnmanData.hpp"

class ConnmanManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool powered READ powered WRITE setPowered NOTIFY propertyChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY propertyChanged)
    Q_PROPERTY(QString name READ name NOTIFY propertyChanged)
    Q_PROPERTY(QString type READ type CONSTANT               )
    Q_PROPERTY(bool tethering READ tethering WRITE setTethering NOTIFY  propertyChanged)
    Q_PROPERTY(bool scannig READ scanning NOTIFY scannigChanged)
    Q_PROPERTY(QList<ConnmanData> servicesList READ servicesList NOTIFY servicesListChanged)

public:
    using ObjectPath_t = QMap<QString,QString>;

    ConnmanManager(const QString& type,QObject* parent = nullptr);

    void scan();

    void setPowered(bool powered);
    void setTethering(bool tethering);
    void setScannig(bool scannig);

    ConnmanData::ObjectMap_t technologyProperties();
    bool    powered();
    bool    connected();
    QString name();
    QString type();
    bool    tethering();
    bool    scanning()const {return _scanning;}
    QList<ConnmanData> servicesList()const {return _services;}

    static const char* _technologyTypeWiFi;
    static const char* _technologyEthernet;
    static const char* _technologyTypeBluetooth;
    static const char* _technologyTypep2p;

    static const char* _poweredPropertyName;
    static const char* _connectedPropertyName;
    static const char* _namePropertyName;
    static const char* _typePropertyName;
    static const char* _tetheringPropertyName;

signals:
    void propertyChanged();
    void scannigChanged();
    void servicesListChanged();

private slots:
    void technologyPropertyChanged(QString name,QDBusVariant dbvalue);
    void servicePropertyChanged(QString name,QDBusVariant dbvalue,QDBusMessage);
    void managerPropertyChanged(QString name,QDBusVariant dbvalue) {};
    void servicesChanged(QList<QVariant>,QList<QDBusObjectPath>,QDBusMessage);
    void peersChanged(QList<QVariant>,QList<QDBusObjectPath>) {};
    void technologyAdded(QDBusObjectPath,QVariantMap) {};
    void technologyRemoved(QDBusObjectPath) {};

private:
    void checkDBusConnection();
    bool technology();    
    bool services();
    ConnmanData::ObjectMap_t managerProperties();
    QDBusMessage::MessageType processReply(const QDBusMessage& reply);

    static void buildObjectPathMap();
    static ObjectPath_t _objectPathInfoMap;

    QDBusInterface*     _connmanManager;
    ConnmanData         _technology;
    QList<ConnmanData>  _services;

    QString _type;
    quint16 _error;
    bool    _scanning;
};

#endif // CONNMANMANAGER_H
