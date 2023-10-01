#ifndef CONNMANDATA_H
#define CONNMANDATA_H

#include <QObject>
#include <QDBusObjectPath>
#include <QDBusMessage>

struct ConnmanData {
    Q_GADGET

    Q_PROPERTY(QDBusObjectPath objPath MEMBER _objPath CONSTANT)
    Q_PROPERTY(QMap<QString,QVariant> objMap MEMBER _objMap CONSTANT)

public:
    using ObjectMap_t = QMap<QString,QVariant>;

    static bool array(QList<ConnmanData> &referanceList, const QDBusMessage &referanceMessage);
    static bool map(ObjectMap_t &referanceMap, const QDBusMessage &referanceMessage);

    void setObjectPath(QDBusObjectPath objectPath);
    void setObjectMap(ObjectMap_t objectMap);

    QDBusObjectPath& objectPath() {return _objPath;}
    ObjectMap_t& objectMap() {return _objMap;}

private:
    QDBusObjectPath _objPath;
    ObjectMap_t     _objMap;
};

Q_DECLARE_METATYPE(ConnmanData)
Q_DECLARE_METATYPE(ConnmanData*)
Q_DECLARE_METATYPE(const ConnmanData*)

#endif // CONNMANDATA_H
