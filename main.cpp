#include <QtWidgets/QApplication>
#include "ayiktray.h"
#include "ayikpopup.h"
#include "ayikoption.h"

FILE *fplog;
void myMessageOutput(QtMsgType type, const char *msg)
 {
     switch (type) {
     case QtDebugMsg:
         fprintf(fplog, "Debug: %s\n", msg);
         break;
     case QtWarningMsg:
         fprintf(fplog, "Warning: %s\n", msg);
         break;
     case QtCriticalMsg:
         fprintf(fplog, "Critical: %s\n", msg);
         break;
     case QtFatalMsg:
         fprintf(fplog, "Fatal: %s\n", msg);
         abort();
     default:
         fprintf(fplog, "Log: %s\n", msg);
     }
 }

int main(int argc, char *argv[])
{
    fplog = fopen("ayik.log","w");
    //qInstallMsgHandler(myMessageOutput);
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(AYIK);

    a.setWindowIcon(QIcon(":/images/ayik.png"));
    QTranslator translator;
    translator.load("AYIKtr_la");
    a.installTranslator(&translator);

    AyikTray *tray = AyikTray::getInstance();
    tray->setVisible(true);

    AyikOption::getInstance();//dummy call to initialize
    AyikOption::getInstance()->postInit();

    return a.exec();
}
