#include "ayiktray.h"
#include "ayikoption.h"
#include "ayikpopup.h"
#include <QtWidgets/QMenu>

AyikTray* AyikTray::instance;

AyikTray::AyikTray()
{
    createActions();
    createTrayIcon();

    AyikPopup* popup = AyikPopup::getInstance();

    connect(popup, SIGNAL(windowHidden()), this, SLOT(startMessageTimer()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
             this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(&timer,SIGNAL(timeout()),this,SLOT(showMessage()));

    trayIcon->show();

    timer.start(2000);//first message after 2 secs
}
AyikTray* AyikTray::getInstance()
{
    if(instance==NULL) instance = new AyikTray();
    return instance;
}
void AyikTray::setVisible(bool visible)
{
    trayIcon->setVisible(visible);
}
void AyikTray::showSetup()
{
    AyikOption* options = AyikOption::getInstance();
    //setupAction->setEnabled( !(options->isVisible()) );
    options->setVisible(true);
}
void AyikTray::startMessageTimer() {
    AyikOption* options = AyikOption::getInstance();
    timer.start(options->getMessageFrequency()*60000);//convert into mins -- 60*1000 = 60 sec * 1000 msec
}
void AyikTray::showMessage()
{
    timer.stop();
    AyikPopup::getInstance()->showPopup();
}
void AyikTray::showHelp()
{
    AyikOption::getInstance()->showHelp();
}
void AyikTray::createActions()
{
    setupAction = new QAction(tr("&Options"), this);
    connect(setupAction, SIGNAL(triggered()), this, SLOT(showSetup()));

    helpAction = new QAction(tr("&Help"), this);
    connect(helpAction, SIGNAL(triggered()), this, SLOT(showHelp()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}
void AyikTray::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(setupAction);
    trayIconMenu->addAction(helpAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(QIcon(":/images/ayik.png"));
    trayIcon->setToolTip("AYIK");
    trayIcon->setContextMenu(trayIconMenu);
}
 void AyikTray::iconActivated(QSystemTrayIcon::ActivationReason reason)
 {
     switch (reason) {
     case QSystemTrayIcon::Trigger:
     case QSystemTrayIcon::DoubleClick:
         showMessage();
         break;
     case QSystemTrayIcon::MiddleClick:
         showMessage();
         break;
     default:
         ;
     }
 }
