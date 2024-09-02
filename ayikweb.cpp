#include "ayikweb.h"
#include "dictutil.h"

ayikWeb::ayikWeb(QObject *parent) : QThread(parent)
{
}

void ayikWeb::run()
{
    while(true) {
        mutex.lock();
        if(!targetWord.isEmpty()) {
            //QString content = hw->getUrlContent("http://www.zargan.com/sozluk.asp?Sozcuk="+word+"");
            //QString content = dictutil::getUrlContent("www.zargan.com","/sozluk.asp?Sozcuk="+targetWord+"","ISO8859-9");
            //answer = dictutil::parse_zargan(content);
            QString content = dictutil::getUrlContent("www.seslisozluk.net","/"+targetWord+"-nedir-ne-demek/","UTF-8");
            QString answer = dictutil::parse_seslisozluk(content);
            targetWord.clear();
            emit lookupDone();
        }
        mutex.unlock();
        msleep(1000);
    }
}

void ayikWeb::lookupWord(QString word)
{
    mutex.lock();
    targetWord = word;
    mutex.unlock();
}

QString ayikWeb::getAnswer()
{
    return answer;
}
