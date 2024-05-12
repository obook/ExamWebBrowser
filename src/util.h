 
 #ifndef UTIL_H
 #define UTIL_H

 #include <QtCore/QCoreApplication>
 #include <QtCore/QFileInfo>
 #include <QtCore/QUrl>
 #include <QtCore/QStringList>

 QUrl urlFromUserInput(const QString& userInput)
 {
     QFileInfo fileInfo(userInput);
     if (fileInfo.exists())
         return QUrl(fileInfo.absoluteFilePath());
     return QUrl::fromUserInput(userInput);
 }

 QUrl startupUrl()
 {
     QUrl ret;
     QStringList args(qApp->arguments());
     args.takeFirst();
     Q_FOREACH (const QString& arg, args) {
         if (arg.startsWith(QLatin1Char('-')))
             continue;
         ret = urlFromUserInput(arg);
         if (ret.isValid())
             return ret;
     }
     return QUrl(QStringLiteral("http://qt.io/"));
 }

 #endif // UTIL_H
