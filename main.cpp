#include <iostream>
#include <QApplication>
#include "ConnmanGlobal.hpp"
#include "ConnmanTechnology.hpp"
#include <QDebug>

using namespace std;

int main(int argc, char* argv[])
{
    QApplication a(argc,argv);

    ConnmanTechnology tech(ConnmanTechnology::_technologyTypeWiFi);

    qDebug() << "Version: " << Connman::findConnmanVersion();

//    qDebug() << tech.powered();
//    tech.setPowered(true);
//    qDebug() << tech.powered();

    tech.scan();

    a.exec();

    return 0;
}
