#include "settings.h"

Settings::Settings(QObject *parent)
    : QObject{parent}
{
    /* Settings */
    pSettings = new QSettings("ExamWebBrowser.ini",QSettings::IniFormat);
}

QString Settings::GetUrl() {
    MainUrl = pSettings->value("URL").toString();
    if(MainUrl==""){
        MainUrl = "https://smb33.keosystems.com";
        pSettings->setValue("URL", MainUrl);
    }
    return MainUrl;
}

QString Settings::GetServerIP() {
    ServerIP = pSettings->value("ServerIP").toString();
    if(ServerIP==""){
        ServerIP = "127.0.01";
        pSettings->setValue("ServerIP", ServerIP);
    }
    return ServerIP;
}

