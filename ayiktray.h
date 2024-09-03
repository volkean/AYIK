#ifndef AYIKTRAY_H
#define AYIKTRAY_H

#include <QtGui>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSystemTrayIcon>

class AyikTray : public QDialog
{
    Q_OBJECT

public:
    AyikTray();
    static AyikTray* getInstance();
    void setVisible(bool visible);

private slots:
    void showMessage();
    void showSetup();
    void showHelp();
    void startMessageTimer();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    static AyikTray* instance;
    void createActions();
    void createTrayIcon();
    QTimer timer;

    QAction *setupAction;
    QAction *helpAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // AYIKTRAY_H
