#ifndef GLOBALVARS
#define GLOBALVARS
#define REBOOT_EVENT 1001
#define N_LINES    8
/* colors */
#define C_DISABLED  "#283942"
#define C_ERROR     "#9a27f3"
#define C_VALID     "#23a045"
#define C_TEXT      "#283942"
/* CA types */
enum DevType {COOLMASTER, COOLINK,COOLINKHUB,COOLPLUG,COOLPLUGADMIN} ;
enum WarningType{USB_NOT_CONNECTED, SESSION_CONNECTION_LOST,
                 PORT_UNAVAILABLE, DEVICE_UNAVAILABLE, OPEN_ERROR,
                 SWITCHES_INCORRECT_STATE,RESOURCE_ERROR,REBOOT_REQUIRED, READING_ERROR, NO_DATA_IN_PORT};




#endif // GLOBALVARS
