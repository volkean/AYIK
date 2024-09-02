#ifndef DICTUTIL_H
#define DICTUTIL_H

#include <QtGui>
#include <QList>

class dictutil
{
public:

    static QString parse_zargan(const QString& content);
    static QString getUrlContent(const QString& strhost,const QString& strpath,const QString& encoding);
    static QString parse_seslisozluk_bna(const QString& content);
    static QString parse_seslisozluk(const QString& content);
    static void randomSequence(int seq[], int size);//1'den size'a kadar tekrarsiz seri
    static void randomSequence(QList<int>& seq, int N);//1'den N'e kadar tekrarsiz seri
};

#endif
