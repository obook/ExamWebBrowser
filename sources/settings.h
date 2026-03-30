/*
 * Nom : settings.h
 * Description : Gestion de la configuration via le fichier ExamWebBrowser.ini.
 * Auteur : O. Booklage
 * Date : 2024
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject {
    Q_OBJECT

public:
    explicit Settings(QObject *parent = nullptr);

    QString getUrl();
    QString getAppName();

private:
    QSettings *m_settings;
    QString m_mainUrl;
    QString m_appName;
};

#endif /* SETTINGS_H */
