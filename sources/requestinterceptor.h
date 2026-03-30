/*
 * Nom : requestinterceptor.h
 * Description : Intercepteur de requêtes HTTP pour filtrer les URL hors domaine autorisé.
 * Auteur : O. Booklage
 * Date : 2024
 */

#ifndef REQUESTINTERCEPTOR_H
#define REQUESTINTERCEPTOR_H

#include <QWebEngineProfile>
#include <QWebEngineUrlRequestInterceptor>
#include "settings.h"

class RequestInterceptor : public QWebEngineUrlRequestInterceptor {
public:
    explicit RequestInterceptor(QObject *parent = Q_NULLPTR)
        : QWebEngineUrlRequestInterceptor(parent) {
    }

    virtual void interceptRequest(QWebEngineUrlRequestInfo &info) Q_DECL_OVERRIDE;

private:
    Settings m_settings;
};

#endif /* REQUESTINTERCEPTOR_H */
