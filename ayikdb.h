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
    int loadWordsXml(const QString& filename,bool append=false);
    void loadWords(const QString& filename,bool append=false);
    void loadWords(const QStringList& filenames,bool append=false);
    void loadWordMemdb(const QString& filename,bool append=false);
    static AyikDB* getInstance();
    void getRandomWord(AyikWord& w,const QString& tag,const QString& rating,const QString& ratingOperator,QString &err);
    void getWord(AyikWord& w,const QString& name,QString &err);
    void getWordList(QueryResult &res,QString &err);
    void getTagList(QStringList &list,QString &err);
    void regenerateShuffle();


    void createDbTables();
    void insertWord(const AyikWord& w);
    void insertWords(const QList<AyikWord>& w);
    void deleteWord(const AyikWord& w,QString &err);
    void deleteWords(const QList<AyikWord>& w,QString &err);
    void deleteWordsLike(const QString& filter,QString &err);
    void updateWord(const AyikWord& w);
    void truncateWords();
    void testDb();


    int connect();
    int execute(const QString& sqlstr, QueryResult& qr);
    int execute(const QStringList& sqlstr);
    /*int update(QString &sqlstr);*/
    QString error();

    QSqlDatabase db;
    QString errstr;

private:
    static AyikDB* instance;

    QList<AyikWord> words;
    QList<int> seq;//random sequence

};

#endif // AYIKDB_H
