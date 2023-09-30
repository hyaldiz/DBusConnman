#ifndef CONNMANDATA_H
#define CONNMANDATA_H

#include <QObject>
#include <QDBusObjectPath>
#include <QDBusMessage>

struct ConnmanData {
    Q_GADGET

    Q_PROPERTY(QDBusObjectPath          objPath MEMBER _objPath CONSTANT)
    Q_PROPERTY(QMap<QString,QVariant>   objMap  MEMBER _objMap  CONSTANT)

public:
    using ObjectMap_t = QMap<QString,QVariant>;

    static bool array(QList<ConnmanData> &referanceList, const QDBusMessage &referanceMessage);
    static bool map(QMap<QString, QVariant> &referanceMap, const QDBusMessage &referanceMessage);

    const QDBusObjectPath& objectPath()const {return _objPath;}
    const ObjectMap_t& objectMap()const {return _objMap;}

private:
    QDBusObjectPath _objPath;
    ObjectMap_t     _objMap;
};

Q_DECLARE_METATYPE(ConnmanData)
Q_DECLARE_METATYPE(ConnmanData*)
Q_DECLARE_METATYPE(const ConnmanData*)

#endif // CONNMANDATA_H
