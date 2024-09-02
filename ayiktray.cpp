#include "ayiktray.h"
#include "ayikoption.h"
#include <QtWidgets/QMenu>

ayikTray* ayikTray::instance;

ayikTray::ayikTray()
{
    createActions();
    createTrayIcon();

    ayikPopup* popup = ayikPopup::getInstance();

    connect(popup, SIGNAL(window_hidden()), this, SLOT(startMessageTimer()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
             this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(&timer,SIGNAL(timeout()),this,SLOT(showMessage()));

    trayIcon->show();

    timer.start(2000);//first message after 2 secs
}
ayikTray* ayikTray::getInstance()
{
    if(instance==NULL) instance = new ayikTray();
    return instance;
}
void ayikTray::setVisible(bool visible)
{
    trayIcon->setVisible(visible);
}
void ayikTray::showSetup()
{
    ayikOption* options = ayikOption::getInstance();
    //setupAction->setEnabled( !(options->isVisible()) );
    options->setVisible(true);
}
void ayikTray::startMessageTimer() {
    ayikOption* options = ayikOption::getInstance();
    timer.start(options->getMessageFrequency()*60000);//convert into mins -- 60*1000 = 60 sec * 1000 msec
}
void ayikTray::showMessage()
{
    timer.stop();
    ayikPopup::getInstance()->showPopup();
}
void ayikTray::showHelp()
{
    ayikOption::getInstance()->showHelp();
}
void ayikTray::createActions()
{
    setupAction = new QAction(tr("&Options"), this);
    connect(setupAction, SIGNAL(triggered()), this, SLOT(showSetup()));

    helpAction = new QAction(tr("&Help"), this);
    connect(helpAction, SIGNAL(triggered()), this, SLOT(showHelp()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}
void ayikTray::createTrayIcon()
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
 void ayikTray::iconActivated(QSystemTrayIcon::ActivationReason reason)
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
