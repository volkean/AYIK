#ifndef AYIKDB_H
#define AYIKDB_H

#include "ayikword.h"
#include <QtGui>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

class QueryResult {
public:
    QList<QString> headers;
    QList< QList<QString> > records;
};

class AyikDB : public QObject
{
    Q_OBJECT

public:
    AyikDB();
    void addWords(const QString& filename,const QString& tag,const QString& rating);
    void loadWordMemdb(const QString& filename,bool append=false);
    static AyikDB* getInstance();
    void getRandomWord(AyikWord& w,const QString& tag,const QString& rating,const QString& ratingOperator,QString &err);
    void getWord(AyikWord& w,const QString& name,QString &err);
    void getWordList(QueryResult &res,QString &err);
    void getTagList(QStringList &list,QString &err);

    void createDbTables();
    void insertWord(const AyikWord& w);
    void insertWords(const QList<AyikWord>& w);
    void deleteWord(const AyikWord& w,QString &err);
    void deleteWords(const QList<AyikWord>& w,QString &err);
    void deleteWordsLike(const QString& filter,QString &err);
    void updateWord(const AyikWord& w);
    void truncateWords();

    int connect();
    int execute(const QString& sqlstr, QueryResult& qr);
    int execute(const QStringList& sqlstr);
    /*int update(QString &sqlstr);*/
    QString error();

    QSqlDatabase db;
    QString errstr;

private:
    static AyikDB* instance;

};

#endif // AYIKDB_H
