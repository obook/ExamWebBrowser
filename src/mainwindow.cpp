#include "mainwindow.h"
#include "./ui_mainwindow.h"
//#include <QCursor>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ExamWebBrowser");
    setupToolBar();
    webview = new QWebEngineView(this);
    // webview->load(QUrl("google.fr"));
    webview->load(QUrl("https://smb33.keosystems.com"));
    setCentralWidget(webview);
    QObject::connect(monTimer, SIGNAL(timeout()), this,SLOT(finTempo()));
}

void MainWindow::setupToolBar()
{
    toolbar = addToolBar(tr("File"));
    toolbar->setFloatable(false);
    toolbar->setMovable(false);
    toolbar->setFixedHeight(36);
    toolbar->setStyleSheet("QToolBar {background-color: grey;}");
    monTimer = new QTimer(this);
    connect(monTimer, SIGNAL(timeout()), this, SLOT(update()));
    monTimer->start(8000);
}

void MainWindow::finTempo()
{
    qDebug()<<"Fin Timer";
}

void MainWindow::FocusLost()
{

}

/* Détection de la perte de focus */
bool MainWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::WindowActivate:
        qDebug() << "Window has focus!!!";
        toolbar->setStyleSheet("QToolBar {background-color: grey;}");
        webview->setVisible(true);
        break;
    case QEvent::WindowDeactivate:
        qDebug() << "Window lost focus!!!";
        toolbar->setStyleSheet("QToolBar {background-color: red;}");
        //QCursor cursor(Qt::BlankCursor);
        //QApplication::setOverrideCursor(cursor);
        //QApplication::changeOverrideCursor(cursor);
        webview->setVisible(false);
        break;
    default:
        break;
    }

    return QWidget::event(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}
