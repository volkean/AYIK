#include "ayikweb.h"
#include "dictutil.h"

AyikWeb::AyikWeb(QObject *parent) : QObject(parent)
{
    networkAccessManager = new QNetworkAccessManager(this);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &AyikWeb::onFinished);
}

QString AyikWeb::getLastWordMeaning()
{
    return lastWordMeaning;
}

void AyikWeb::lookupWordMeaning(AyikWord ayikWord) {
    lookupWordMeaningFromSeslisozluk(ayikWord);
}

void AyikWeb::lookupWordMeaningFromSeslisozluk(AyikWord ayikWord) {
    QUrl url("https://www.seslisozluk.net/"+ayikWord.getName()+"-nedir-ne-demek/");
    QNetworkRequest request;
    request.setUrl(url);

    // Optionally, configure SSL (e.g., disable certificate validation)
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(sslConfig);

    // Send the request - will callback when finished
    networkAccessManager->get(request);
}

void AyikWeb::onFinished(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        lastHttpResponse = reply->readAll();
        qDebug() << "Response:" << lastHttpResponse;
        lastWordMeaning = DictUtil::parseSeslisozluk(lastHttpResponse);
        emit lookupDone();
    } else {
        qDebug() << "Error:" << reply->errorString();
    }
    reply->deleteLater();
}
