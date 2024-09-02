#include "dictutil.h"
#include <QTcpSocket>
#include <QtGui>
#include <QTextCodec>

QString dictutil::parse_zargan(const QString& content) {

    const QString delimStartWord="class=\"fi\">";
    const QString delimStartMeaning="class=\"se\">";
    const QString delimEnd="<a";
    QString ret="<table cellspacing=0 cellpadding=2>";
    int fromIndex=0;
    while(true) {
        int indexWord = content.indexOf(delimStartWord,fromIndex);
        if(indexWord < 0) break;
        int toIndexWord = content.indexOf(delimEnd,indexWord);
        if(toIndexWord < 0) break;
        QString word = content.mid(indexWord+delimStartWord.length(),toIndexWord-indexWord-delimStartWord.length());

        int indexMeaning = content.indexOf(delimStartMeaning,fromIndex);
        if(indexMeaning < 0) break;
        int toIndexMeaning = content.indexOf(delimEnd,indexMeaning);
        if(toIndexMeaning < 0) break;
        QString meaning = content.mid(indexMeaning+delimStartMeaning.length(),toIndexMeaning-indexMeaning-delimStartMeaning.length());
        //ret += "<tr><td>"+word+":</td><td>"+meaning+"</td></tr>";
        ret += "<tr><td>"+word+": "+meaning+"</td></tr>";
        fromIndex = toIndexMeaning;
    }
    return "</table>"+ret;
}
QString dictutil::getUrlContent(const QString& strhost,const QString& strpath,const QString& encoding) {

    QTcpSocket socket;
    socket.connectToHost(strhost,80);
    QString req="GET "+ strpath +" HTTP/1.1\r\n"
                "Host: "+ strhost +"\r\n\r\n";
    socket.write(req.toStdString().c_str());
    QByteArray buffer;
    while (true) {
        bool r = socket.waitForReadyRead(3000);
        if(r==false) break;        
        buffer += socket.readAll();
    }
    //QTextCodec *codec = QTextCodec::codecForName("ISO8859-9");
    //QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec *codec = QTextCodec::codecForName(encoding.toStdString().c_str());
    QString content = codec->toUnicode( buffer.data() );
//    QMessageBox::information(0,"",content);
    return content;
}

QString dictutil::parse_seslisozluk_bna(const QString& content) {

    const QString ref = "ne ar";
    const QString endRef = "<br/>";
    const QString delimStart="\">";
    const QString delimEnd="</a>";
    QString ret="";
    int fromIndex=0;
    int endIndex = 0;
    fromIndex = content.indexOf(ref,fromIndex);
    if(fromIndex > 0) fromIndex = fromIndex + ref.length() + 30;//30 ne?
    else return ret;
    endIndex = content.indexOf(endRef,fromIndex);
    while(true) {
        int index = content.indexOf(delimStart,fromIndex);
        if( (index < 0) || (index >= endIndex) ) break;
        int toIndex = content.indexOf(delimEnd,index);
        if(toIndex < 0) break;
        QString meaning = content.mid(index+delimStart.length(),toIndex-index-delimStart.length());
        ret += meaning + "\n";
        fromIndex = toIndex;
    }

    return ret;
}


QString dictutil::parse_seslisozluk(const QString& content) {

    //const QString selector = "a.definition-link";//$("a.definition-link")[0].innerText
    static const QRegularExpression REGEX_SESLISOZLUK(R"(<a class="definition-link".*>(.*)<\/a>)");
    QRegularExpressionMatchIterator i = REGEX_SESLISOZLUK.globalMatch(content);

    QString ret="";

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        QString matchedText = match.captured(1); // captured(1) refers to the content inside the parentheses
        qDebug() << "Matched text:" << matchedText;
        ret += matchedText+"\n";
    }

    return ret;
}

void dictutil::randomSequence(int seq[], int size) {

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

void dictutil::randomSequence(QList<int>& seq, int N) {

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

