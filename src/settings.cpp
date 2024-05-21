#include "settings.h"

Settings::Settings(QObject *parent)
    : QObject{parent}
{
    qDebug() << "Settings::Settings";
    /* Settings */
    pSettings = new QSettings("ExamWebBrowser.ini",QSettings::IniFormat);
}

QString Settings::GetUrl() {
    MainUrl = pSettings->value("URL").toString();
    if(MainUrl==""){
        MainUrl = "https://moodle-4.0.7.keosystems.com/html/login/index.php";
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

QString Settings::GetServerPort() {
    ServerPort = pSettings->value("ServerPort").toString();
    if(ServerPort==""){
        ServerPort = "127.0.01";
        pSettings->setValue("ServerPort", ServerPort);
    }
    return ServerPort;
}

QString Settings::GetAppName() {
    AppName = pSettings->value("AppName").toString();
    if(AppName==""){
        AppName = "   SAINTE-MARIE BASTIDE  ";
        pSettings->setValue("AppName", AppName);
    }
    return AppName;
}

