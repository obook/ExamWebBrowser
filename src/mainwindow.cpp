#include "mainwindow.h"
#include "./ui_mainwindow.h"
//#include <QCursor>
#include <QKeyEvent>
#include <QDateTime>
#include <QInputDialog>
#include <QWebEngineProfile>
#include <QWebEngineCookieStore>
#include <QMessageBox>
#include <QWebEnginePage>

/*

Todo :

Voir 
QWebEngineView::loadStarted() // This signal is emitted when a new load of the page is started.
QWebEngineView::loadProgress(int progress) // This signal is emitted every time an element in the web view completes loading
void QWebEngineView::loadFinished(bool ok) // This signal is emitted when a load of the page has finished. ok will indicate whether the load was successful or an error occurred.

Voir
https://blog.csdn.net/CHAMSDONCON/article/details/124403358
https://cpp.hotexamples.com/examples/-/QWebEnginePage/-/cpp-qwebenginepage-class-examples.html

Il est possible de toute refaire avec webenginewidgets et être compatible Windows/Linux

*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ExamWebBrowser");
    setupToolBar();
    webview = new QWebEngineView(this);
    webview->setContextMenuPolicy(Qt::NoContextMenu);
    //QWebEnginePage *page =  webview->page();
    //page->
    //connect(page, &QWebEnginePage::)
    //webview->setUrl(QUrl("https://smb33.keosystems.com")); /* ne fait rien, Ok en reload */
    webview->load(QUrl("https://smb33.keosystems.com"));
    webview->reload();
/* Pose problème ...
    QWebEngineProfile * engineProfile = webview->page()->profile();
    engineProfile->clearHttpCache();
    engineProfile->clearAllVisitedLinks();
    engineProfile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    engineProfile->setHttpCacheType(QWebEngineProfile::NoCache);

    QWebEngineCookieStore * pCookie = engineProfile->cookieStore();
    pCookie->deleteAllCookies();
    pCookie->deleteSessionCookies();
*/

    connect(webview, &QWebEngineView::loadStarted, [] {
        QApplication::setOverrideCursor(Qt::WaitCursor);
    });

    connect(webview, &QWebEngineView::loadFinished, [] {
        QApplication::restoreOverrideCursor();
    });

    setCentralWidget(webview);



    bFocusLost = false;
    bToogleColors = false;
    bWebViewHidden = false;
    QObject::connect(monTimer, SIGNAL(timeout()), this,SLOT(finTempo()));
}

void MainWindow::setupToolBar()
{
    toolbar = addToolBar(tr("File"));
    toolbar->setFloatable(false);
    toolbar->setMovable(false);
    toolbar->setFixedHeight(36);
    toolbar->setStyleSheet("QToolBar {background-color: green;}");

    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    toolbar->addWidget(spacerWidget);

    PushButton = new QPushButton(this);
    PushButton->setText("hello");
    connect(PushButton, &QPushButton::released, this, &MainWindow::handleButton);

    toolbar->addWidget(PushButton);

    monTimer = new QTimer(this);
    connect(monTimer, SIGNAL(timeout()), this, SLOT(update()));
    monTimer->start(1000);
}

void MainWindow::handleButton()
{
    if (bFocusLost==true)
    {
        QInputDialog Dialog = new QInputDialog;
        Dialog.setCancelButtonText(QString("Abandon"));
        Dialog.setOkButtonText(QString("Valider"));
        bool ok;
        QString text = Dialog.getText(this, tr("Déverouillage"), tr("Entrer le code:"), QLineEdit::Normal, tr(""), &ok);
        if ( ok && !text.isEmpty() ) {
            QString code_secret = QDateTime::currentDateTime().toString("ddMM"); /* Eg 1605 pour le 16 mai */
            if( text.contains(code_secret)  )
            {
                UnlockWebView();
            }
        }
    }
    else {
        /*
        QMessageBox msgBox;
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        reply = QMessageBox::question(this, "Quitter", "Voulez-vous quitter l'application?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QCoreApplication::exit();
        } else {
            qDebug() << "Yes was *not* clicked";
        }
*/

        QMessageBox msgBox;
        msgBox.setText("Voulez-vous quitter l'application?");
        msgBox.setInformativeText("Tout travail non enregistré sera perdu.");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        //QAbstractButton *myYesButton = msgBox.addButton(QString("Quitter"), QMessageBox::Ok);
        //QAbstractButton *myNoButton = msgBox.addButton(QString("Abandon"), QMessageBox::NoRole);
        msgBox.setIcon(QMessageBox::Warning);
        if( msgBox.exec() == QMessageBox::Ok ) {
            QCoreApplication::exit();
        }
        else
        {
            UnlockWebView();
        }

    }
}

void MainWindow::finTempo()
{
    if (bFocusLost==true)
    {
        if(bToogleColors==false)
        {
            PushButton->setText("Appel");
            toolbar->setStyleSheet("QToolBar {background-color: black;}");
            bToogleColors = true;
        }
        else
        {
            PushButton->setText("Surveillant");
            toolbar->setStyleSheet("QToolBar {background-color: red;}");
            bToogleColors = false;
        }
    } else {
        /* Clock */

        PushButton->setText(QDateTime::currentDateTime().toString("hh:mm"));
    }
}


void MainWindow::UnlockWebView() {
    bFocusLost = false;
    toolbar->setStyleSheet("QToolBar {background-color: green;}");
    bWebViewHidden = false;
    webview->setVisible(true);
}

void MainWindow::LockWebView() {
    bFocusLost = true;
    toolbar->setStyleSheet("QToolBar {background-color: red;}");
    bWebViewHidden = true;
    webview->setVisible(false);
}

/* Détection de la perte de focus */
bool MainWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::WindowActivate:
        break;
    case QEvent::WindowDeactivate:
        toolbar->setStyleSheet("QToolBar {background-color: red;}");
        LockWebView();
        break;
    default:
        break;
    }

    return QWidget::event(event);
}

void MainWindow::closeEvent(QCloseEvent * event)
{
    event->ignore();
}

MainWindow::~MainWindow()
{
    delete ui;
}
