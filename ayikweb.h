#ifndef AYIKWEB_H
#define AYIKWEB_H

#include <QtGui>

class ayikWeb : public QThread
{
    Q_OBJECT

public:
    ayikWeb(QObject *parent = 0);
    void run();
    void lookupWord(QString word);
    QString getAnswer();

private:
    QString targetWord;
    QString answer;
    QMutex mutex;

signals:
    void lookupDone();

public slots:

};

#endif // AYIKWEB_H
