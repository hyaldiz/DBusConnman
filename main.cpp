#include <iostream>
#include <QApplication>
#include "ConnmanGlobal.hpp"
#include "ConnmanManager.hpp"
#include <QDebug>
#include <QTimer>

using namespace std;

int main(int argc, char* argv[])
{
    QApplication a(argc,argv);

    ConnmanManager tech(ConnmanManager::_technologyTypeWiFi);

    qDebug() << "Version: " << Connman::findConnmanVersion();

    tech.setPowered(true);

    QTimer* tmr = new QTimer;
    tmr->setInterval(1000);
    QObject::connect(tmr,&QTimer::timeout,[&tech]()
    {
        tech.scan();
    });
    tmr->start();


    a.exec();

    return 0;
}
