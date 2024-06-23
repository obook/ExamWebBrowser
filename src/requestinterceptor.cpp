#include "requestinterceptor.h"
#include <QDebug>

/*
 *
 *
<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key>originatorVersion</key>
    <string>SEB_Win_2.0RC</string>
    <key>sendBrowserExamKey</key>
    <true />
    <key>startURL</key>
    <string>http://localhost:3001/?_id=5ea190190dab16145e9c27d9</string>
  </dict>
</plist>

https://sourceforge.net/p/seb/discussion/844844/thread/74d5abe02e/

*/

void RequestInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{

    /* Try to be SEB compatible

    // config key CK
    QByteArray httpHeaderConfigName = "X-SafeExamBrowser-ConfigKeyHash";
    QByteArray httpHeaderConfigValue = "bc7c5c84c9500f00a696c94fcfd0df2288c59ef5025d409e5676459bfbdeeee1"; // Hexa only
    info.setHttpHeader(httpHeaderConfigName, httpHeaderConfigValue);

    // browser exam key BEK
    QByteArray httpHeaderRequestName = "X-SafeExamBrowser-RequestHash";
    QByteArray httpHeaderRequestValue = "bc7c5c84c9500f00a696c94fcfd0df2288c59ef5025d409e5676459bfbdeeee1";
    info.setHttpHeader(httpHeaderRequestName, httpHeaderRequestValue);

    */

    /* Intercepting the requested URL : Stay in same FQD */
    QString RequestHost = info.requestUrl().host();
    QString SettingsHost = (QUrl(pSettings.GetUrl())).host();

    if( RequestHost != SettingsHost)
    {
        qDebug() << "/!\\ HÔTE HORS DOMAINE BLOQUÉ : " << RequestHost;
        info.block(true);
    }
}
