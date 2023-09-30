#include "ConnmanData.hpp"
#include <QDBusArgument>
#include <QDebug>

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
