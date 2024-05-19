#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);
public:
    QString GetUrl();
    QString GetServerIP();
    QString GetServerPort();
private:
    QSettings *pSettings;
    QString MainUrl;
    QString ServerIP;
    QString ServerPort;
};

#endif // SETTINGS_H
