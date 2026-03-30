/*
 * Nom : settings.cpp
 * Description : Gestion de la configuration via le fichier ExamWebBrowser.ini.
 * Auteur : O. Booklage
 * Date : 2024
 */

#include "settings.h"
#include <QFile>
#include <QCoreApplication>

Settings::Settings(QObject *parent)
    : QObject{parent} {
    QString index(QCoreApplication::applicationDirPath() + "/notice.html");

    m_settings = new QSettings("ExamWebBrowser.ini", QSettings::IniFormat);

    if (!QFile::exists(index)) {
        QFile notice(index);
        if (!notice.open(QFile::WriteOnly | QFile::Text)) {
            return;
        }
        QString html(
            "<!DOCTYPE html>"
            "<html lang='fr'>"
            "<head>"
            "<meta charset='utf-8'>"
            "<title></title>"
            "</head>"
            "<body>"
            "<header></header>"
            "<main>"
            "<p>Quitter le programme avec un clic droit sur l'horloge</p>"
            "<p>Éditer le fichier ExamWebBrowser.ini et changer la valeur 'URL'</p>"
            "<p>Relancer le programme</p>"
            "</main>"
            "<footer></footer>"
            "</body>"
            "</html>");
        notice.write(html.toUtf8());
        notice.close();
    }
}

QString Settings::getUrl() {
    m_mainUrl = m_settings->value("URL").toString();
    if (m_mainUrl == "") {
        m_mainUrl = "notice.html";
        m_settings->setValue("URL", m_mainUrl);
    }
    return m_mainUrl;
}

QString Settings::getAppName() {
    m_appName = m_settings->value("AppName").toString();
    if (m_appName == "") {
        m_appName = "SAINTE-MARIE BASTIDE - NAVIGATEUR EN MODE EXAMEN";
        m_settings->setValue("AppName", m_appName);
    }
    return m_appName;
}
