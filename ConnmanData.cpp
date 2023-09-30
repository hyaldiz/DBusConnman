#include "ConnmanData.hpp"
#include <QDBusArgument>

bool ConnmanData::array(QList<ConnmanData> &referanceList, const QDBusMessage &referanceMessage)
{
    if(!referanceMessage.arguments().at(0).canConvert<QDBusArgument>())
        return false;

    const QDBusArgument& qdbusArgument = referanceMessage.arguments().at(0).value<QDBusArgument>();
    if(qdbusArgument.currentType() != QDBusArgument::ArrayType)
        return false;

    referanceList.clear();
    qdbusArgument.beginArray();

    while(!qdbusArgument.atEnd()) {

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

bool ConnmanData::map(QMap<QString, QVariant> &referanceMap, const QDBusMessage &referanceMessage)
{

}
