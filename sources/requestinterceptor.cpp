/*
 * Nom : requestinterceptor.cpp
 * Description : Intercepteur de requêtes HTTP pour filtrer les URL hors domaine autorisé.
 * Auteur : O. Booklage
 * Date : 2024
 */

#include "requestinterceptor.h"
#include <QDebug>

void RequestInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info) {
    /* En-tête personnalisé pour la détection par le plugin Moodle */
    info.setHttpHeader("X-ExamWebBrowser", "1.0");

    /*
     * Filtrage des URL : rester dans le même domaine (FQDN),
     * mais autoriser les CDN externes nécessaires au fonctionnement.
     */
    QString requestHost = info.requestUrl().host();
    QString settingsHost = (QUrl(m_settings.getUrl())).host();

    if (requestHost != settingsHost
        && !info.requestUrl().toString().contains("mathjax", Qt::CaseInsensitive)
        && !info.requestUrl().toString().contains("basthon.fr", Qt::CaseInsensitive)
        && !info.requestUrl().toString().contains("jsdelivr.net", Qt::CaseInsensitive)
        && !info.requestUrl().toString().contains("education.fr", Qt::CaseInsensitive)) {
        qDebug() << "/!\\ HÔTE" << requestHost
                 << "HORS DOMAINE BLOQUÉ (" << info.requestUrl().toString() << ")";
        info.block(true);
    }
}
