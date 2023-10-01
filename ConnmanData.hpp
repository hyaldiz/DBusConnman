#ifndef CONNMANDATA_H
#define CONNMANDATA_H

#include <QObject>
#include <QDBusObjectPath>
#include <QDBusMessage>
#include <QDBusArgument>

class ConnmanData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QDBusObjectPath objPath READ objectPath NOTIFY objectPathChanged)

public:
    using ObjectMap_t = QMap<QString,QVariant>;

    ConnmanData(QObject* parent = nullptr);
    ConnmanData(QDBusObjectPath objPath,ObjectMap_t objMap,QObject* parent = nullptr);
    ConnmanData(const ConnmanData&);
    ConnmanData& operator=(const ConnmanData&);

    static bool array(QList<ConnmanData> &referanceList, const QDBusMessage &referanceMessage);
    static bool map(ObjectMap_t &referanceMap, const QDBusMessage &referanceMessage);
    static bool extractMapData(QMap<QString,QVariant>& referanceMap,const QVariant& referanceVariant);

    void setObjectPath(QDBusObjectPath objectPath);
    void setObjectMap(ObjectMap_t objectMap);

    QDBusObjectPath objectPath() const{return _objPath;}
    ObjectMap_t objectMap() const{return _objMap;}
    bool autoConnect()const{return _objMap.value(_autoConnectPropertyName).toBool();}
    QStringList domains()const{return _objMap.value(_domainsPropertyName).toStringList();}
    QStringList domainsConfiguration()const{return _objMap.value(_domainsConfigurationPropertyName).toStringList();}
    //QDBusArgument ethernet()const{return _objMap.value(_ethernetPropertyName).;}
    bool favorite()const{return _objMap.value(_favoritePropertyName).toBool();}
    QString ipv4()const{return _objMap.value(_ipv4PropertyName).toString();}
    QMap<QString,QVariant> ipv4Configuration()const;
    QString ipv6()const{return _objMap.value(_ipv6PropertyName).toString();};
    QMap<QString,QVariant> ipv6Configuration()const;
    bool immutable()const{return _objMap.value(_immutablePropertyName).toBool();}
    QString name()const{return _objMap.value(_namePropertyName).toString();}
    QStringList nameServers()const{return _objMap.value(_nameServersPropertyName).toStringList();}
    QStringList nameServersConfiguration()const{return _objMap.value(_nameServersConfigurationPropertyName).toStringList();}
    //QDBusArgument provider()const{return _objMap.value(_providerPropertyName);}
    //QDBusArgument proxy()const{return _objMap.value(_proxyPropertyName);}
    //QDBusArgument proxyConfiguration()const{return _objMap.value(_proxyConfigurationPropertyName);}
    QStringList security()const{return _objMap.value(_securityPropertyName).toStringList();}
    QString state()const{return _objMap.value(_statePropertyName).toString();}
    quint8 strength()const{return _objMap.value(_strengthPropertyName).toUInt();}
    QStringList timeServers()const{return _objMap.value(_timeServersPropertyName).toStringList();}
    QStringList timeServersConfiguration()const{return _objMap.value(_timeServersConfigurationPropertyName).toStringList();}
    QString type()const{return _objMap.value(_typePropertyName).toString();}
    bool mDNS()const{return _objMap.value(_mDNSPropertyName).toBool();}
    bool mDNSConfiguration()const{return _objMap.value(_mDNSConfigurationPropertyName).toBool();}

    static const char* _autoConnectPropertyName;
    static const char* _domainsPropertyName;
    static const char* _domainsConfigurationPropertyName;
    static const char* _ethernetPropertyName;
    static const char* _favoritePropertyName;
    static const char* _ipv4PropertyName;
    static const char* _ipv4ConfigurationPropertyName;
    static const char* _ipv6PropertyName;
    static const char* _ipv6ConfigurationPropertyName;
    static const char* _immutablePropertyName;
    static const char* _namePropertyName;
    static const char* _nameServersPropertyName;
    static const char* _nameServersConfigurationPropertyName;
    static const char* _providerPropertyName;
    static const char* _proxyPropertyName;
    static const char* _proxyConfigurationPropertyName;
    static const char* _securityPropertyName;
    static const char* _statePropertyName;
    static const char* _strengthPropertyName;
    static const char* _timeServersPropertyName;
    static const char* _timeServersConfigurationPropertyName;
    static const char* _typePropertyName;
    static const char* _mDNSPropertyName;
    static const char* _mDNSConfigurationPropertyName;

signals:
    void objectPathChanged();
    void mapDataChanged();

private:
    QDBusObjectPath _objPath;
    ObjectMap_t     _objMap;

private slots:
    void changed();
};

#endif // CONNMANDATA_H
