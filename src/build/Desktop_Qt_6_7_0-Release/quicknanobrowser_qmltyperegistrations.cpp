/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>

#include <utils.h>


#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_BrowserUtils()
{
    qmlRegisterTypesAndRevisions<Utils>("BrowserUtils", 1);
    qmlRegisterModule("BrowserUtils", 1, 0);
}

static const QQmlModuleRegistration browserUtilsRegistration("BrowserUtils", qml_register_types_BrowserUtils);
