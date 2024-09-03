#include "dictutil.h"
#include <QTcpSocket>
#include <QtGui>
#include <QTextCodec>
#include <QSslSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QString DictUtil::getUrlContent(const QString& strhost,const QString& strpath,const QString& encoding) {
    //QTcpSocket socket;
    QSslSocket socket;
    socket.connectToHostEncrypted(strhost, 443);
    if (!socket.waitForEncrypted()) {
        qDebug() << "Failed to connect to host securely:" << socket.errorString();
        return "";
    }
    QByteArray request;    
    request.append(QString("GET "+strpath+" HTTP/1.1\r\n").toUtf8());
    request.append(QString("Host: "+strhost+"\r\n").toUtf8());
    request.append("Connection: close\r\n\r\n");

    socket.write(request);
    socket.flush();
    QByteArray buffer;    
    if (!socket.waitForReadyRead()) {
        qDebug() << "Failed to receive a response:" << socket.errorString();
        return "";
    }

    while (socket.bytesAvailable()) {
        buffer.append(socket.readAll());
        qDebug() << buffer;
    }

    socket.disconnectFromHost();
    //QTextCodec *codec = QTextCodec::codecForName("ISO8859-9");
    //QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec *codec = QTextCodec::codecForName(encoding.toStdString().c_str());
    QString content = codec->toUnicode( buffer.data() );
//    QMessageBox::information(0,"",content);
    return content;
}

QString DictUtil::parseSeslisozluk(const QString& content) {

    //const QString selector = "a.definition-link";//$("a.definition-link")[0].innerText
    static const QRegularExpression REGEX_SESLISOZLUK(R"(<a class="definition-link".*?>(.*?)<\/a>)");
    QRegularExpressionMatchIterator i = REGEX_SESLISOZLUK.globalMatch(content);

    QString ret="";

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString matchedText = match.captured(1); // captured(1) refers to the content inside the parentheses
        qDebug() << "Matched text:" << matchedText;
        ret += matchedText+"<br>";
    }

    return ret;
}

void DictUtil::randomSequence(int seq[], int size) {

    //init seq
    for(int i=0;i<size;i++) {
        seq[i] = (i+1);
    }
    //shuffle
    for(int i=size;i>0;i--) {
        //qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
        QRandomGenerator::global()->seed(QRandomGenerator::global()->generate());
        //int index = (qrand() % i);
        int index = QRandomGenerator::global()->generate() % i;
        int temp = seq[index];
        seq[index] = seq[i-1];//swap with last
        seq[i-1] = temp;
    }
}

void DictUtil::randomSequence(QList<int>& seq, int N) {

    seq.clear();

    //init seq
    for(int i=1;i<=N;i++) {
        seq.append(i);
    }
    //shuffle
    for(int i=N;i>0;i--) {
        //qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
        QRandomGenerator::global()->seed(QRandomGenerator::global()->generate());
        //int index = (qrand() % i);
        int index = QRandomGenerator::global()->generate() % i;
        seq.swapItemsAt((i-1),index);
    }
}

