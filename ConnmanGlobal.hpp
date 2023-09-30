#ifndef CONNMANGLOBAL_HPP
#define CONNMANGLOBAL_HPP

#define CONNMAN_SERVICE                 "net.connman"
#define CONNMAN_PATH                    "/net/connman"
#define CONNMAN_MANAGER_INTERFACE       CONNMAN_SERVICE".Manager"
#define CONNMAN_MANAGER_PATH            "/"
#define CONNMAN_SERVICE_INTERFACE       CONNMAN_SERVICE".Service"
#define CONNMAN_TECHNOLOGY_INTERFACE    CONNMAN_SERVICE".Technology"

#include <QProcess>

struct Connman {

    enum {
        ERROR_NO                        = 0x00,
        ERROR_NO_DBUS                   = (1 << 0),
        ERROR_Invalid_Connman_Interface = (1 << 1),
        ERROR_PROPERTIES                = (1 << 2),
        ERROR_Technologies              = (1 << 3),
        ERROR_Services                  = (1 << 4)
    };

    static float findConnmanVersion()
    {
        float version;
        bool ok = false;
        QProcess qprocess;
        qprocess.start("connmand",{"-v"});
        qprocess.waitForFinished(3000);
        version = qprocess.readAllStandardOutput().toFloat(&ok);
        if(!ok)
            version = -1.0;
        return version;
    }
};

#endif // CONNMANGLOBAL_HPP
