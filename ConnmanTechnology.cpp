#include "ConnmanTechnology.hpp"
#include "ConnmanGlobal.hpp"
#include <QDebug>

ConnmanTechnology::ObjectPath_t ConnmanTechnology::_objectPathInfoMap;

const char* ConnmanTechnology::_technologyTypeWiFi      = "wifi";
const char* ConnmanTechnology::_technologyEthernet      = "ethernet";
const char* ConnmanTechnology::_technologyTypeBluetooth = "bluetooth";
const char* ConnmanTechnology::_technologyTypep2p       = "p2p";

ConnmanTechnology::ConnmanTechnology(const QString &type, QObject *parent) : QObject(parent)
    ,_connmanManager(nullptr)
    ,_type(type)
{
    if(_objectPathInfoMap.isEmpty())
        buildObjectPaths();

    checkDBusConnection();
}

void ConnmanTechnology::buildObjectPaths()
{
    _objectPathInfoMap[ConnmanTechnology::_technologyTypeWiFi]      = "/net/connman/technology/wifi";
    _objectPathInfoMap[ConnmanTechnology::_technologyEthernet]      = "/net/connman/technology/ethernet";
    _objectPathInfoMap[ConnmanTechnology::_technologyTypeBluetooth] = "/net/connman/technology/bluetooth";
    _objectPathInfoMap[ConnmanTechnology::_technologyTypep2p]       = "/net/connman/technology/p2p";
}

void ConnmanTechnology::checkDBusConnection()
{
    quint16 geciciError = 0;
    if(!QDBusConnection::systemBus().isConnected())
        geciciError = Connman::ERROR_NO_DBUS;
    else {
        _connmanManager = new QDBusInterface(DBUS_CON_SERVICE,DBUS_PATH,DBUS_CON_MANAGER,QDBusConnection::systemBus(),this);
        if(!_connmanManager->isValid())
            geciciError = Connman::ERROR_Invalid_Connman_Interface;
        else {
            geciciError &= ~(Connman::ERROR_PROPERTIES);
            geciciError &= ~(Connman::ERROR_Technologies);
            geciciError &= ~(Connman::ERROR_Services);

            if(!technologies())
                geciciError = Connman::ERROR_Technologies;
            else {
                //WAIT FOR DONE!
            }
        }
    }
}

bool ConnmanTechnology::technologies()
{
    QDBusMessage reply = _connmanManager->call("GetTechnologies");
    processReply(reply);
    return ConnmanData::array(_technologies,reply);
}

QDBusMessage::MessageType ConnmanTechnology::processReply(const QDBusMessage& reply)
{
    if(!reply.errorName().isEmpty() && !reply.errorMessage().isEmpty())
        qDebug() << "Error name: " << reply.errorName() << "\tError message: " << reply.errorMessage();
    return reply.type();
}

void ConnmanTechnology::setPowered(bool powered)
{
    QDBusInterface* techInterface =
        new QDBusInterface(DBUS_CON_SERVICE,_objectPathInfoMap[_type],DBUS_CON_TECHNOLOGY,QDBusConnection::systemBus(),this);

    processReply(techInterface->call(QDBus::NoBlock,"SetProperty","Powered",QVariant::fromValue(QDBusVariant(powered))));
    techInterface->deleteLater();
}

void ConnmanTechnology::setTethering(bool tethering)
{

}

void ConnmanTechnology::setTetheringIdentifier(const QString &tetheringIdentifier)
{

}

void ConnmanTechnology::setTetheringPassphrase(const QString &tetheringPassphrase)
{

}

bool ConnmanTechnology::powered() const
{

}

bool ConnmanTechnology::connected() const
{

}

QString ConnmanTechnology::name() const
{

}

QString ConnmanTechnology::type() const
{
    return _type;
}

bool ConnmanTechnology::tethering() const
{

}

QString ConnmanTechnology::tetheringIdentifier() const
{

}

QString ConnmanTechnology::tetheringPassphrase() const
{

}
