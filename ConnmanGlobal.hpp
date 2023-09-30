#ifndef CONNMANGLOBAL_HPP
#define CONNMANGLOBAL_HPP

#define DBUS_PATH "/"
#define DBUS_CON_SERVICE      "net.connman"
#define DBUS_CON_MANAGER      "net.connman.Manager"
#define DBUS_CON_TECHNOLOGY   "net.connman.Technology"

namespace Connman {

    enum {
        ERROR_NO                        = 0x00,
        ERROR_NO_DBUS                   = (1 << 0),
        ERROR_Invalid_Connman_Interface = (1 << 1),
        ERROR_PROPERTIES                = (1 << 2),
        ERROR_Technologies              = (1 << 3),
        ERROR_Services                  = (1 << 4)
    };
}

#endif // CONNMANGLOBAL_HPP
