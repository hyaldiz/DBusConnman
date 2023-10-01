#include "ConnmanData.hpp"
#include <QDBusArgument>

const char* ConnmanData::_autoConnectPropertyName = "AutoConnect";
const char* ConnmanData::_domainsPropertyName = "Domains";
const char* ConnmanData::_domainsConfigurationPropertyName = "Domains.Configuration";
const char* ConnmanData::_ethernetPropertyName = "Ethernet";
const char* ConnmanData::_favoritePropertyName = "Favorite";
const char* ConnmanData::_ipv4PropertyName = "IPv4";
const char* ConnmanData::_ipv4ConfigurationPropertyName = "IPv4.Configuration";
const char* ConnmanData::_ipv6PropertyName = "IPv6";
const char* ConnmanData::_ipv6ConfigurationPropertyName = "IPv6.Configuration";
const char* ConnmanData::_immutablePropertyName = "Immutable";
const char* ConnmanData::_namePropertyName = "Name";
const char* ConnmanData::_nameServersPropertyName = "Nameservers";
const char* ConnmanData::_nameServersConfigurationPropertyName = "Nameservers.Configuration";
const char* ConnmanData::_providerPropertyName = "Provider";
const char* ConnmanData::_proxyPropertyName = "Proxy";
const char* ConnmanData::_proxyConfigurationPropertyName = "Proxy.Configuration";
const char* ConnmanData::_securityPropertyName = "Security";
const char* ConnmanData::_statePropertyName = "State";
const char* ConnmanData::_strengthPropertyName = "Strength";
const char* ConnmanData::_timeServersPropertyName = "Timeservers";
const char* ConnmanData::_timeServersConfigurationPropertyName = "Timeservers.Configuration";
const char* ConnmanData::_typePropertyName = "Type";
const char* ConnmanData::_mDNSPropertyName = "mDNS";
const char* ConnmanData::_mDNSConfigurationPropertyName = "mDNS.Configuration";

ConnmanData::ConnmanData(QObject *parent) : QObject(parent)
{
    connect(this,&ConnmanData::mapDataChanged,this,&ConnmanData::changed);
}

ConnmanData::ConnmanData(QDBusObjectPath objPath, ObjectMap_t objMap, QObject *parent) : QObject(parent)
    ,_objPath(objPath)
    ,_objMap(objMap)
{
}

ConnmanData::ConnmanData(const ConnmanData & data) : QObject(data.parent())
    ,_objPath(data._objPath)
    ,_objMap(data._objMap)
{
    connect(this,&ConnmanData::mapDataChanged,this,&ConnmanData::changed);
}

ConnmanData &ConnmanData::operator=(const ConnmanData & data)
{
    setParent(data.parent());
    _objPath = data._objPath;
    _objMap = data._objMap;
    return *this;
}

bool ConnmanData::array(QList<ConnmanData> &referanceList, const QDBusMessage &referanceMessage)
{
    if(!referanceMessage.arguments().at(0).canConvert<QDBusArgument>())
        return false;

    const QDBusArgument& qdbusArgument = referanceMessage.arguments().at(0).value<QDBusArgument>();
    if(qdbusArgument.currentType() != QDBusArgument::ArrayType)
        return false;

    referanceList.clear();
    qdbusArgument.beginArray();

    while(!qdbusArgument.atEnd())
    {
        if(qdbusArgument.currentType() != QDBusArgument::StructureType)
            return false;

        ConnmanData data;
        qdbusArgument.beginStructure();
        qdbusArgument >> data._objPath >> data._objMap;
        qdbusArgument.endStructure();
        referanceList.append(data);
    }

    qdbusArgument.endArray();

    return true;
}

bool ConnmanData::map(ObjectMap_t &referanceMap, const QDBusMessage &referanceMessage)
{
    if(!referanceMessage.arguments().at(0).canConvert<QDBusArgument>())
        return false;

    const QDBusArgument& qdbusArgument = referanceMessage.arguments().at(0).value<QDBusArgument>();
    if(qdbusArgument.currentType() != QDBusArgument::MapType)
        return false;

    referanceMap.clear();
    qdbusArgument.beginMap();

    while(!qdbusArgument.atEnd())
    {
        QString  key;
        QVariant value;
        qdbusArgument.beginMapEntry();
        qdbusArgument >> key >> value;
        qdbusArgument.endMapEntry();
        referanceMap.insert(key,value);
    }

    qdbusArgument.endMap();
    return true;
}

bool ConnmanData::extractMapData(QMap<QString, QVariant> &referanceMap, const QVariant &referanceVariant)
{
    if(!referanceVariant.canConvert<QDBusArgument>())
        return false;

    const QDBusArgument qdbusArgument = referanceVariant.value<QDBusArgument>();
    if(qdbusArgument.currentType() != QDBusArgument::MapType)
        return false;

    referanceMap.clear();
    qdbusArgument.beginMap();
    while (!qdbusArgument.atEnd()) {
        QString key;
        QVariant value;
        qdbusArgument.beginMapEntry();
        qdbusArgument >> key >> value;
        qdbusArgument.endMapEntry();
        referanceMap.insert(key,value);
    }
    qdbusArgument.endMap();
    return true;
}

void ConnmanData::setObjectPath(QDBusObjectPath objectPath)
{
    _objPath = objectPath;
    emit objectPathChanged();
}

void ConnmanData::setObjectMap(ObjectMap_t objectMap)
{
    _objMap = objectMap;
    emit mapDataChanged();
}

QMap<QString,QVariant> ConnmanData::ipv4Configuration() const
{
    QMap<QString,QVariant> ipv4InfoMap;
    ipv4InfoMap.clear();
    extractMapData(ipv4InfoMap,_objMap.value(_ipv4ConfigurationPropertyName));
    return ipv4InfoMap;
}

QMap<QString,QVariant> ConnmanData::ipv6Configuration() const
{
    QMap<QString,QVariant> ipv6InfoMap;
    ipv6InfoMap.clear();
    extractMapData(ipv6InfoMap,_objMap.value(_ipv6ConfigurationPropertyName));
    return ipv4Configuration();
}

#include <QDebug>

void ConnmanData::changed()
{
    qDebug() << _objMap;
}
