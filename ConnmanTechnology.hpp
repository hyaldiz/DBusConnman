#ifndef CONNMANTECHNOLOGY_HPP
#define CONNMANTECHNOLOGY_HPP

#include <QObject>
#include <QDBusVariant>
#include <QDBusMessage>
#include <QDBusInterface>
#include <QPair>
#include "ConnmanData.hpp"

class ConnmanTechnology : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool powered                 READ powered             WRITE setPowered               NOTIFY  propertyChanged)
    Q_PROPERTY(bool connected               READ connected                                          NOTIFY  propertyChanged)
    Q_PROPERTY(QString name                 READ name                                               NOTIFY  propertyChanged)
    Q_PROPERTY(QString type                 READ type                                               CONSTANT               )
    Q_PROPERTY(bool tethering               READ tethering           WRITE setTethering             NOTIFY  propertyChanged)
    Q_PROPERTY(bool scannig READ scanning NOTIFY scannigChanged)

public:
    using ObjectPath_t = QMap<QString,QString>;

    ConnmanTechnology(const QString& type,QObject* parent = nullptr);

    void scan();

    void setPowered(bool powered);
    void setTethering(bool tethering);
    void setScannig(bool scannig);

    ConnmanData::ObjectMap_t properties();
    bool    powered();
    bool    connected();
    QString name();
    QString type();
    bool    tethering();
    bool    scanning()const {return _scanning;}

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

private slots:
    void technologyPropertyChanged(QString name,QDBusVariant dbvalue);

private:
    void checkDBusConnection();
    bool technology();
    QDBusMessage::MessageType processReply(const QDBusMessage& reply);

    static void buildObjectPathMap();
    static ObjectPath_t _objectPathInfoMap;

    QDBusInterface* _connmanManager;
    ConnmanData     _technology;
    bool    _powered;
    bool    _connected;
    QString _name;
    QString _type;
    bool    _tethering;
    quint16 _error;
    bool    _scanning;
};

#endif // CONNMANTECHNOLOGY_HPP
