/*
 * Nom : main.cpp
 * Description : Point d'entrée de l'application ExamWebBrowser.
 *               Lance la fenêtre principale en plein écran avec traduction.
 * Auteur : O. Booklage
 * Date : 2024
 */

#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("images/ExamWebBrowser.png")); /* Non affiché sous KDE Wayland */

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ExamWebBrowser_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.setWindowState(Qt::WindowFullScreen);
    w.show();
    return a.exec();
}
