#ifndef AYIKDB_H
#define AYIKDB_H

#include "ayikword.h"
#include <QtGui>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "databaseconnection.h"

class ayikDB : public QObject
{
    Q_OBJECT

public:
    ayikDB();
    void addWords(const QString& filename,const QString& tag,const QString& rating);
    int loadWordsXml(const QString& filename,bool append=false);
    void loadWords(const QString& filename,bool append=false);
    void loadWords(const QStringList& filenames,bool append=false);
    void loadWordMemdb(const QString& filename,bool append=false);
    static ayikDB* getInstance();
    void getRandomWord(ayikWord& w);
    void getRandomWordMemdb(ayikWord& w,const QString& tag,const QString& rating,const QString& ratingOperator,QString &err);
    void getWordMemdb(ayikWord& w,const QString& name,QString &err);
    void getWordList(QueryResult &res,QString &err);
    void getTagList(QStringList &list,QString &err);
    void regenerateShuffle();


    void createDbTables();
    void insertWord(const ayikWord& w);
    void insertWords(const QList<ayikWord>& w);
    void deleteWord(const ayikWord& w,QString &err);
    void deleteWords(const QList<ayikWord>& w,QString &err);
    void updateWord(const ayikWord& w);
    void truncateWords();
    void testDb();

private:
    static ayikDB* instance;

    DatabaseConnection *db;

    QList<ayikWord> words;
    QList<int> seq;//random sequence

};

#endif // AYIKDB_H
