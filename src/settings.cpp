#include "settings.h"
#include <QFile>
#include "QCoreApplication"

Settings::Settings(QObject *parent)
    : QObject{parent}
{
    QString index(QCoreApplication::applicationDirPath() + "/notice.html");

    pSettings = new QSettings("ExamWebBrowser.ini",QSettings::IniFormat);
    if ( !QFile::exists(index) ) {
        QFile notice(index);
        notice.open(QFile::WriteOnly | QFile::Text);
        QString html("<!DOCTYPE html>"
                     "<html lang='Fr'>"
                     "<head>"
                     "<meta charset='utf-8'>"
                     "<title></title>"
                     "</head>"
                     "<body>"
                     "<header></header>"
                     "<main><p>Quitter le programme avec un clic droit sur l'horloge</p>"
                     "<p>Éditer le fichier ExamWebBrowser.ini et changer la valeur 'URL'</p>"
                     "<p>Relancer le programme</p></main>"
                     "<footer></footer>"
                     "</body>"
                     "</html>");
        notice.write(html.toUtf8());
        notice.close();
    }
}

QString Settings::GetUrl() {
    MainUrl = pSettings->value("URL").toString();
    if(MainUrl==""){
        MainUrl = "notice.html";
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
        AppName = "SAINTE-MARIE BASTIDE - NAVIGATEUR EN MODE EXAMEN";
        pSettings->setValue("AppName", AppName);
    }
    return AppName;
}

