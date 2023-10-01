#include "ConnmanManager.hpp"
#include "ConnmanGlobal.hpp"
#include <QDebug>

ConnmanManager::ObjectPath_t ConnmanManager::_objectPathInfoMap;

const char* ConnmanManager::_technologyTypeWiFi = "wifi";
const char* ConnmanManager::_technologyEthernet = "ethernet";
const char* ConnmanManager::_technologyTypeBluetooth = "bluetooth";
const char* ConnmanManager::_technologyTypep2p = "p2p";

const char* ConnmanManager::_poweredPropertyName = "Powered";
const char* ConnmanManager::_connectedPropertyName = "Connected";
const char* ConnmanManager::_namePropertyName = "Name";
const char* ConnmanManager::_typePropertyName = "Type";
const char* ConnmanManager::_tetheringPropertyName = "Tethering";

ConnmanManager::ConnmanManager(const QString &type, QObject *parent) : QObject(parent)
    ,_connmanManager(nullptr)
    ,_type(type)
    ,_error(Connman::ERROR_NO)
{
    if(_objectPathInfoMap.isEmpty())
        buildObjectPathMap();

    checkDBusConnection();
}

void ConnmanManager::buildObjectPathMap()
{
    _objectPathInfoMap[ConnmanManager::_technologyTypeWiFi]      = "/net/connman/technology/wifi";
    _objectPathInfoMap[ConnmanManager::_technologyEthernet]      = "/net/connman/technology/ethernet";
    _objectPathInfoMap[ConnmanManager::_technologyTypeBluetooth] = "/net/connman/technology/bluetooth";
    _objectPathInfoMap[ConnmanManager::_technologyTypep2p]       = "/net/connman/technology/p2p";
}

void ConnmanManager::checkDBusConnection()
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

            if(!services())
                _error = Connman::ERROR_Services;
            else {
                for(auto& service:_services) {
                    QDBusConnection::systemBus().connect
                        (
                            CONNMAN_SERVICE,
                            service.objectPath().path(),
                            CONNMAN_SERVICE_INTERFACE,
                            "PropertyChanged",
                            this,
                            SLOT(servicePropertyChanged(QString,QDBusVariant,QDBusMessage))
                        );
                }
            }

            if(managerProperties().isEmpty())
                _error = Connman::ERROR_PROPERTIES;

            processReply(_connmanManager->call(QDBus::BlockWithGui,"RegisterAgent",QVariant::fromValue(QDBusObjectPath("DUMMY"))));

            QDBusConnection::systemBus().connect(CONNMAN_SERVICE,CONNMAN_MANAGER_PATH,CONNMAN_MANAGER_INTERFACE,"PropertyChanged",this,SLOT(managerPropertyChanged(QString,QDBusVariant)));
            QDBusConnection::systemBus().connect(CONNMAN_SERVICE,CONNMAN_MANAGER_PATH,CONNMAN_MANAGER_INTERFACE,"TechnologyAdded",this,SLOT(technologyAdded(QDBusObjectPath,QVariantMap)));
            QDBusConnection::systemBus().connect(CONNMAN_SERVICE,CONNMAN_MANAGER_PATH,CONNMAN_MANAGER_INTERFACE,"TechnologyRemoved",this,SLOT(technologyRemoved(QList<QVariant>,QList<QDBusObjectPath>)));
            QDBusConnection::systemBus().connect(CONNMAN_SERVICE,CONNMAN_MANAGER_PATH,CONNMAN_MANAGER_INTERFACE,"ServicesChanged",this,SLOT(servicesChanged(QList<QVariant>,QList<QDBusObjectPath>,QDBusMessage)));
            QDBusConnection::systemBus().connect(CONNMAN_SERVICE,CONNMAN_MANAGER_PATH,CONNMAN_MANAGER_INTERFACE,"PeersChanged",this,SLOT(peersChanged(QList<QVariant>,QList<QDBusObjectPath>)));
        }
    }
}

bool ConnmanManager::technology()
{
    QList<ConnmanData> technologies;
    QDBusMessage reply = _connmanManager->call("GetTechnologies");
    processReply(reply);
    ConnmanData::array(technologies,reply);

    for(auto& connmanData : technologies) {
        if(connmanData.objectMap().value("Type").toString() == _type) {
            _technology = connmanData;
            return true;
        }
    }
    return false;
}

bool ConnmanManager::services()
{
    QDBusMessage reply = _connmanManager->call(QDBus::BlockWithGui,"GetServices");
    processReply(reply);
    bool ok = ConnmanData::array(_services,reply);
    emit servicesListChanged();
    return ok;
}

ConnmanData::ObjectMap_t ConnmanManager::managerProperties()
{
    ConnmanData::ObjectMap_t properties;
    QDBusMessage reply = _connmanManager->call(QDBus::BlockWithGui,"GetProperties");
    processReply(reply);
    ConnmanData::map(properties,reply);
    return properties;
}

QDBusMessage::MessageType ConnmanManager::processReply(const QDBusMessage& reply)
{
    if(!reply.errorName().isEmpty() && !reply.errorMessage().isEmpty())
        qDebug() << "Error name: " << reply.errorName() << "\tError message: " << reply.errorMessage();
    return reply.type();
}

void ConnmanManager::scan()
{
    QDBusInterface* techInterface =
        new QDBusInterface(CONNMAN_SERVICE,_objectPathInfoMap[_type],CONNMAN_TECHNOLOGY_INTERFACE,QDBusConnection::systemBus(),this);
    processReply(techInterface->call(QDBus::BlockWithGui,"Scan"));
    setScannig(true);
    techInterface->deleteLater();
}

void ConnmanManager::setPowered(bool powered)
{
    QDBusInterface* techInterface =
        new QDBusInterface(CONNMAN_SERVICE,_objectPathInfoMap[_type],CONNMAN_TECHNOLOGY_INTERFACE,QDBusConnection::systemBus(),this);
    processReply(techInterface->call(QDBus::BlockWithGui,"SetProperty",_poweredPropertyName,QVariant::fromValue(QDBusVariant(powered))));
    techInterface->deleteLater();
}

void ConnmanManager::setTethering(bool tethering)
{
    QDBusInterface* techInterface =
        new QDBusInterface(CONNMAN_SERVICE,_objectPathInfoMap[_type],CONNMAN_TECHNOLOGY_INTERFACE,QDBusConnection::systemBus(),this);
    processReply(techInterface->call(QDBus::BlockWithGui,"SetProperty",_tetheringPropertyName,QVariant::fromValue(QDBusVariant(tethering))));
    techInterface->deleteLater();
}

void ConnmanManager::setScannig(bool scannig)
{
    if(scannig != _scanning) {
        _scanning = scannig;
        emit scannigChanged();
    }
}

ConnmanData::ObjectMap_t ConnmanManager::technologyProperties()
{
    ConnmanData::ObjectMap_t properties;
    QDBusInterface* techInterface =
        new QDBusInterface(CONNMAN_SERVICE,_objectPathInfoMap[_type],CONNMAN_TECHNOLOGY_INTERFACE,QDBusConnection::systemBus(),this);
    QDBusMessage reply = techInterface->call(QDBus::BlockWithGui,"GetProperties");
    processReply(reply);
    ConnmanData::map(properties,reply);
    return properties;
}

bool ConnmanManager::powered()
{
    return technologyProperties().value(_poweredPropertyName).toBool();
}

bool ConnmanManager::connected()
{
    return technologyProperties().value(_connectedPropertyName).toBool();
}

QString ConnmanManager::name()
{
    return technologyProperties().value(_namePropertyName).toString();
}

QString ConnmanManager::type()
{
    return _type;
}

bool ConnmanManager::tethering()
{
    return technologyProperties().value(_tetheringPropertyName).toBool();
}

void ConnmanManager::technologyPropertyChanged(QString property, QDBusVariant dbvalue)
{
    emit propertyChanged();
}

void ConnmanManager::servicePropertyChanged(QString property, QDBusVariant dbvalue,QDBusMessage message)
{
    QString serviceState = "";

    for(int i=0; i < _services.size(); i++) {
        if(message.path() == _services[i].objectPath().path()) {

            ConnmanData::ObjectMap_t objMap(_services[i].objectMap());
            objMap.remove(property);
            objMap.insert(property,objMap);
            _services[i].setObjectMap(objMap);
            serviceState = objMap.value("State").toString();
        }
    }

    if(property == "Error" && serviceState == "failure") {
        //QWCApplication ile sinyal yolla
    }
}

void ConnmanManager::servicesChanged(QList<QVariant> managedPskList, QList<QDBusObjectPath> removedList,QDBusMessage message)
{
    if(!removedList.isEmpty()) {
        for(auto& service:_services) {
            if(removedList.contains(service.objectPath()))
                QDBusConnection::systemBus().disconnect
                    (
                        CONNMAN_SERVICE,
                        service.objectPath().path(),
                        CONNMAN_SERVICE_INTERFACE,
                        "PropertyChanged",
                        this,
                        SLOT(servicePropertyChanged(QString,QDBusVariant))
                    );
        }
    }

    if(!managedPskList.isEmpty()) {
        QList<ConnmanData> revisedList;
        if(!ConnmanData::array(revisedList,message))
            return;


        for(int i=0; i < revisedList.size();i++) {
            ConnmanData originalService;

            for (auto& service:_services) {
                if(revisedList[i].objectPath().path() == service.objectPath().path()) {
                    originalService = service;
                    break;
                }
            }

            if(!originalService.objectPath().path().isEmpty()) {
                QMapIterator<QString,QVariant> mapIterator(revisedList[i].objectMap());
                while(mapIterator.hasNext()) {
                    mapIterator.next();
                    originalService.objectMap().insert(mapIterator.key(),mapIterator.value());
                }

                QDBusConnection::systemBus().disconnect
                    (
                        CONNMAN_SERVICE,
                        originalService.objectPath().path(),
                        CONNMAN_SERVICE_INTERFACE,
                        "PropertyChanged",
                        this,
                        SLOT(servicePropertyChanged(QString,QDBusVariant))
                    );

                revisedList.replace(i,originalService);

                QDBusConnection::systemBus().disconnect
                    (
                        CONNMAN_SERVICE,
                        revisedList[i].objectPath().path(),
                        CONNMAN_SERVICE_INTERFACE,
                        "PropertyChanged",
                        this,
                        SLOT(servicePropertyChanged(QString,QDBusVariant))
                    );
            }
        }

        _services.clear();
        _services = revisedList;
        emit servicesListChanged();
    }

    qDebug() << "---------------------------------";
    for (auto& service:_services)
        qDebug() << "Name: " << service.objectMap().value("Name").toString();
    qDebug() << "---------------------------------";

    setScannig(false);
}
