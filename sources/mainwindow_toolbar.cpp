/*
 * Nom : mainwindow_toolbar.cpp
 * Description : Mise en place de la barre d'outils et slots des boutons de navigation.
 * Auteur : O. Booklage
 * Date : 2024
 */

#include "mainwindow.h"
#include "clickablelabel.h"

void MainWindow::setupToolBar() {
    m_toolbar = (ToolBar *)addToolBar(tr(""));
    m_toolbar->setFloatable(false);
    m_toolbar->setMovable(false);
    m_toolbar->setFixedHeight(TOOLBAR_HEIGHT);
    m_toolbar->toggleViewAction()->setEnabled(false);
    m_toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
    m_toolbar->setFocusOnStyle();

    /* Bouton d'accueil (gauche) */
    m_homeButton = new QPushButton(this);
    QPixmap pixmap("images/home.svg");
    QIcon buttonIcon(pixmap);
    m_homeButton->setIcon(buttonIcon);
    m_toolbar->addWidget(m_homeButton);
    connect(m_homeButton, &QPushButton::released, this, &MainWindow::handleHomeButton);

    /* Boutons de navigation : Moodle et Basthon */
    m_webButton = new QPushButton(this);
    m_webButton->setText("MOODLE");
    m_toolbar->addWidget(m_webButton);
    connect(m_webButton, &QPushButton::released, this, &MainWindow::onWebButtonClicked);

    m_basthonButton = new QPushButton(this);
    m_basthonButton->setText("BASTHON");
    m_toolbar->addWidget(m_basthonButton);
    connect(m_basthonButton, &QPushButton::released, this, &MainWindow::onBasthonButtonClicked);
    m_basthonButton->setEnabled(false);

    /* Séparateur extensible gauche */
    QWidget *spacerLeft = new QWidget(this);
    spacerLeft->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerLeft->setVisible(true);
    m_toolbar->addWidget(spacerLeft);

    /* Titre de l'application (cliquable) */
    QLabel *titleLabel = new ClickableLabel(this);
    titleLabel->setText(m_settings.getAppName());
    titleLabel->setStyleSheet("* { background-color: rgba(0,125,0,0) }");
    m_toolbar->addWidget(titleLabel);
    connect(titleLabel, SIGNAL(clicked()), this, SLOT(onLabelClicked()));

    /* Séparateur extensible droit */
    QWidget *spacerRight = new QWidget(this);
    spacerRight->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerRight->setVisible(true);
    m_toolbar->addWidget(spacerRight);

    /* Horloge / bouton de déverrouillage (droite) */
    m_clockButton = new ClickableButton(this);
    m_clockButton->setFlat(true);
    m_clockButton->setStyleSheet("* { background-color: rgba(0,125,0,0) }");
    m_toolbar->addWidget(m_clockButton);
    connect(m_clockButton, SIGNAL(rightClicked()), this, SLOT(onClockRightClicked()));
    connect(m_clockButton, SIGNAL(leftClicked()), this, SLOT(onClockLeftClicked()));
    m_clockButton->installEventFilter(this);
}

void MainWindow::onClockRightClicked() {
    if (!m_focusLost) {
        showExitDialog();
    }
}

void MainWindow::onClockLeftClicked() {
    if (m_focusLost) {
        showCodeInputDialog();
    }
}

void MainWindow::onWebButtonClicked() {
    m_stackedWidget->setCurrentIndex(TAB_WEB);
    m_webButton->setStyleSheet("QPushButton { background-color : blue }");
    m_basthonButton->setStyleSheet("QPushButton { background-color : normal }");
}

void MainWindow::onBasthonButtonClicked() {
    m_stackedWidget->setCurrentIndex(TAB_BASTHON);
    m_webButton->setStyleSheet("QPushButton { background-color : normal }");
    m_basthonButton->setStyleSheet("QPushButton { background-color : blue }");
}

void MainWindow::onFocusTimer() {
    updateToolBar();
}

void MainWindow::updateToolBar() {
    if (m_focusLost) {
        /* Alternance visuelle pour attirer l'attention du surveillant */
        if (!m_toggleColors) {
            m_clockButton->setText("APPEL");
            m_toolbar->setStyleSheet("QToolBar {background-color: yellow;}");
            m_toggleColors = true;
        } else {
            m_clockButton->setText("SURVEILLANT");
            m_toolbar->setFocusOffStyle();
            m_toggleColors = false;
        }
    } else {
        /* Affichage de l'horloge en fonctionnement normal */
        m_clockButton->setText(QDateTime::currentDateTime().toString("hh:mm"));
    }
}
