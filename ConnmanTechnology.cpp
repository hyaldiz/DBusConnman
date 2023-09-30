#include "ConnmanTechnology.hpp"
#include "ConnmanGlobal.hpp"
#include <QDebug>

ConnmanTechnology::ObjectPath_t ConnmanTechnology::_objectPathInfoMap;

const char* ConnmanTechnology::_technologyTypeWiFi      = "wifi";
const char* ConnmanTechnology::_technologyEthernet      = "ethernet";
const char* ConnmanTechnology::_technologyTypeBluetooth = "bluetooth";
const char* ConnmanTechnology::_technologyTypep2p       = "p2p";

const char* ConnmanTechnology::_poweredPropertyName             = "Powered";
const char* ConnmanTechnology::_connectedPropertyName           = "Connected";
const char* ConnmanTechnology::_namePropertyName                = "Name";
const char* ConnmanTechnology::_typePropertyName                = "Type";
const char* ConnmanTechnology::_tetheringPropertyName           = "Tethering";

ConnmanTechnology::ConnmanTechnology(const QString &type, QObject *parent) : QObject(parent)
    ,_connmanManager(nullptr)
    ,_type(type)
    ,_error(Connman::ERROR_NO)
{
    if(_objectPathInfoMap.isEmpty())
        buildObjectPathMap();

    checkDBusConnection();

    ConnmanData::ObjectMap_t _properties = properties();
    _powered = _properties.value(_poweredPropertyName).toBool();
    _connected = _properties.value(_connectedPropertyName).toBool();
    _name = _properties.value(_namePropertyName).toString();
    _tethering = _properties.value(_tetheringPropertyName).toBool();
}

void ConnmanTechnology::buildObjectPathMap()
{
    _objectPathInfoMap[ConnmanTechnology::_technologyTypeWiFi]      = "/net/connman/technology/wifi";
    _objectPathInfoMap[ConnmanTechnology::_technologyEthernet]      = "/net/connman/technology/ethernet";
    _objectPathInfoMap[ConnmanTechnology::_technologyTypeBluetooth] = "/net/connman/technology/bluetooth";
    _objectPathInfoMap[ConnmanTechnology::_technologyTypep2p]       = "/net/connman/technology/p2p";
}

void ConnmanTechnology::checkDBusConnection()
{
    if(!QDBusConnection::systemBus().isConnected())
        _error = Connman::ERROR_NO_DBUS;
    else {
        _connmanManager = new QDBusInterface(CONNMAN_SERVICE,CONNMAN_MANAGER_PATH,CONNMAN_MANAGER_INTERFACE,QDBusConnection::systemBus(),this);
        if(!_connmanManager->isValid())
            _error = Connman::ERROR_Invalid_Connman_Interface;
        else {
            _error &= ~(Connman::ERROR_PROPERTIES);
            _error &= ~(Connman::ERROR_Technologies);
            _error &= ~(Connman::ERROR_Services);

            if(!technology())
                _error = Connman::ERROR_Technologies;
            else {
                QDBusConnection::systemBus().connect
                    (
                        CONNMAN_SERVICE,
                        _technology.objectPath().path(),
                        CONNMAN_TECHNOLOGY_INTERFACE,
                        "PropertyChanged",
                        this,
                        SLOT(technologyPropertyChanged(QString,QDBusVariant))
                    );
            }
        }
    }
}

bool ConnmanTechnology::technology()
{
    QList<ConnmanData> technologies;
    QDBusMessage reply = _connmanManager->call("GetTechnologies");
    processReply(reply);
    ConnmanData::array(technologies,reply);

    for (const auto& connmanData : technologies) {
        if(connmanData.objectMap().value("Type").toString() == _type) {
            _technology = connmanData;
            return true;
        }
    }
    return false;
}

QDBusMessage::MessageType ConnmanTechnology::processReply(const QDBusMessage& reply)
{
    if(!reply.errorName().isEmpty() && !reply.errorMessage().isEmpty())
        qDebug() << "Error name: " << reply.errorName() << "\tError message: " << reply.errorMessage();
    return reply.type();
}

void ConnmanTechnology::scan()
{
    QDBusInterface* techInterface =
        new QDBusInterface(CONNMAN_SERVICE,_objectPathInfoMap[_type],CONNMAN_TECHNOLOGY_INTERFACE,QDBusConnection::systemBus(),this);
    processReply(techInterface->call("Scan"));
    setScannig(true);
    techInterface->deleteLater();
}

void ConnmanTechnology::setPowered(bool powered)
{
    QDBusInterface* techInterface =
        new QDBusInterface(CONNMAN_SERVICE,_objectPathInfoMap[_type],CONNMAN_TECHNOLOGY_INTERFACE,QDBusConnection::systemBus(),this);
    processReply(techInterface->call(QDBus::BlockWithGui,"SetProperty",_poweredPropertyName,QVariant::fromValue(QDBusVariant(powered))));
    techInterface->deleteLater();
}

void ConnmanTechnology::setTethering(bool tethering)
{
    QDBusInterface* techInterface =
        new QDBusInterface(CONNMAN_SERVICE,_objectPathInfoMap[_type],CONNMAN_TECHNOLOGY_INTERFACE,QDBusConnection::systemBus(),this);
    processReply(techInterface->call(QDBus::BlockWithGui,"SetProperty",_tetheringPropertyName,QVariant::fromValue(QDBusVariant(tethering))));
    techInterface->deleteLater();
}

void ConnmanTechnology::setScannig(bool scannig)
{
    if(scannig != _scanning) {
        _scanning = scannig;
        emit scannigChanged();
    }
}

ConnmanData::ObjectMap_t ConnmanTechnology::properties()
{
    ConnmanData::ObjectMap_t properties;
    QDBusInterface* techInterface =
        new QDBusInterface(CONNMAN_SERVICE,_objectPathInfoMap[_type],CONNMAN_TECHNOLOGY_INTERFACE,QDBusConnection::systemBus(),this);
    QDBusMessage reply = techInterface->call(QDBus::BlockWithGui,"GetProperties");
    processReply(reply);
    ConnmanData::map(properties,reply);
    return properties;
}

bool ConnmanTechnology::powered()
{
    return properties().value(_poweredPropertyName).toBool();
}

bool ConnmanTechnology::connected()
{
    return properties().value(_connectedPropertyName).toBool();
}

QString ConnmanTechnology::name()
{
    return properties().value(_namePropertyName).toString();
}

QString ConnmanTechnology::type()
{
    return _type;
}

bool ConnmanTechnology::tethering()
{
    return properties().value(_tetheringPropertyName).toBool();
}

void ConnmanTechnology::technologyPropertyChanged(QString name, QDBusVariant dbvalue)
{
    emit propertyChanged();
}
