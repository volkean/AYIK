#ifndef AYIKWEB_H
#define AYIKWEB_H

#include "ayikword.h"
#include <QtGui>
#include <QSslSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class AyikWeb : public QObject
{
    Q_OBJECT

public:
    AyikWeb(QObject *parent = 0);
    QString getLastWordMeaning();
    void lookupWordMeaning(AyikWord ayikWord);
private:
    QString targetWord;
    AyikWord lastAyikWord;
    QString lastWordMeaning;
    QString lastHttpResponse;
    QNetworkAccessManager* networkAccessManager;
    void lookupWordMeaningFromSeslisozluk(AyikWord ayikWord);
signals:
    void lookupDone();

public slots:
private slots:
    void onFinished(QNetworkReply* reply);
};

#endif // AYIKWEB_H
