#ifndef CONNMANTECHNOLOGY_HPP
#define CONNMANTECHNOLOGY_HPP

#include <QObject>
#include <QDBusVariant>
#include <QDBusMessage>
#include <QDBusInterface>
#include "ConnmanData.hpp"

class ConnmanTechnology : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool powered                 READ powered             WRITE setPowered               NOTIFY poweredChanged)
    Q_PROPERTY(bool connected               READ connected                                          NOTIFY connectedChanged)
    Q_PROPERTY(QString name                 READ name                                               NOTIFY nameChanged)
    Q_PROPERTY(QString type                 READ type                                               CONSTANT)
    Q_PROPERTY(bool tethering               READ tethering           WRITE setTethering             NOTIFY tetheringChanged)
    Q_PROPERTY(QString tetheringIdentifier  READ tetheringIdentifier WRITE setTetheringIdentifier   NOTIFY tetheringIdentifierChanged)
    Q_PROPERTY(QString tetheringPassphrase  READ tetheringPassphrase WRITE setTetheringPassphrase   NOTIFY tetheringPassphraseChanged)
public:
    using ObjectPath_t = QMap<QString,QString>;

    ConnmanTechnology(const QString& type,QObject* parent = nullptr);

    void setPowered(bool powered);
    void setTethering(bool tethering);
    void setTetheringIdentifier(const QString& tetheringIdentifier);
    void setTetheringPassphrase(const QString& tetheringPassphrase);

    bool    powered()const;
    bool    connected()const;
    QString name()const;
    QString type()const;
    bool    tethering()const;
    QString tetheringIdentifier()const;
    QString tetheringPassphrase()const;

    static const char* _technologyTypeWiFi;
    static const char* _technologyEthernet;
    static const char* _technologyTypeBluetooth;
    static const char* _technologyTypep2p;

signals:
    void propertyChanged(const QString& name,QDBusVariant dbvalue,QDBusMessage message);

    void poweredChanged();
    void connectedChanged();
    void nameChanged();
    void tetheringChanged();
    void tetheringIdentifierChanged();
    void tetheringPassphraseChanged();

private:
    void checkDBusConnection();

    bool technologies();

    QDBusMessage::MessageType processReply(const QDBusMessage& reply);

    static void buildObjectPaths();
    static ObjectPath_t _objectPathInfoMap;

    QDBusInterface*     _connmanManager;
    QList<ConnmanData>  _technologies;

    bool    _powered;
    bool    _connected;
    QString _name;
    QString _type;
    bool    _tethering;
    QString _tetheringIdentifier;
    QString _tetheringPassphrase;
};

#endif // CONNMANTECHNOLOGY_HPP
