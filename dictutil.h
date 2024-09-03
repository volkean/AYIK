#ifndef DICTUTIL_H
#define DICTUTIL_H

#include <QtGui>
#include <QList>

class DictUtil
{
public:
    static QString getUrlContent(const QString& strhost,const QString& strpath,const QString& encoding);
    static QString parseSeslisozluk(const QString& content);
    static void randomSequence(int seq[], int size);//1'den size'a kadar tekrarsiz seri
    static void randomSequence(QList<int>& seq, int N);//1'den N'e kadar tekrarsiz seri
};

#endif
