#include "ayikdb.h"
#include "dictutil.h"
#include <QtXml>
#include <QtWidgets/QProgressDialog>

#define ERRCHK(x) if(x.lastError().type() != QSqlError::NoError) { \
qDebug() << x.lastError().text(); \
        return; \
    }

ayikDB* ayikDB::instance;

ayikDB::ayikDB()
{
    db = new DatabaseConnection("litil","QSQLITE");
    db->setConnectionParams("","","","","litil");
    instance = this;

    db->connect();
    createDbTables();
}
ayikDB* ayikDB::getInstance()
{
    if(instance==NULL) instance = new ayikDB();
    return instance;
}
int ayikDB::loadWordsXml(const QString& filename,bool append)
{
    QDomDocument doc;
    QFile file( filename );
    if( !file.open( QIODevice::ReadOnly ) )
        return -1;

    QString errmsg;
    if( !doc.setContent( &file,&errmsg ) )
    {
        qDebug()<<errmsg;
        file.close();
        return -2;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if( root.tagName() != "AYIK" )
        return -3;

    QDomNode n = root.firstChild();
    while( !n.isNull() )
    {
        QDomElement e = n.toElement();
        if( !e.isNull() )
        {
            if( e.tagName() == "word" )
            {
                QString name = e.attribute("name","");
                QString tag = e.attribute("tag","");
                QString rating = e.attribute("rating","");
                QString meaning = e.text();
                ayikWord a_word(name,meaning,rating,tag);
                words.append(a_word);
            }
        }

        n = n.nextSibling();
    }
    return 0;
}
void ayikDB::loadWords(const QString& filename,bool append)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    if(!append) words.clear();

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList list1 = line.split("=");
        if(list1.length() > 1) {
            ayikWord a_word(list1.at(0),list1.at(1));
            words.append(a_word);
        }
    }
}
void ayikDB::loadWordMemdb(const QString& filename,bool append)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    if(append==false) truncateWords();

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList list1 = line.split("=");
        if(list1.length() > 1) {
            ayikWord a_word(list1.at(0),list1.at(1));
            insertWord(a_word);
        }
    }
}
void ayikDB::loadWords(const QStringList& filenames,bool append)
{
    if(!append) words.clear();

    for(int i=0;i<filenames.size();i++) {
        QString filename = "data/"+filenames.at(i);
        loadWordMemdb(filename,true);
    }
    //testDb();
    regenerateShuffle();
}
void ayikDB::getRandomWord(ayikWord& w)
{
    static int i=0;
    //    qDebug() <<"i:"<<i << "seqsize:"<< seq.size() << "wordsize:"<<words.size();
    if(i >= seq.size()) i=0;
    if(seq.size() > 0) {
        int index = seq.at(i) - 1;
        w = words.at(index);
        i++;
    }
}
void ayikDB::regenerateShuffle()
{
    //lets create a shuffle list
    dictutil::randomSequence(seq,words.size());
}
void ayikDB::createDbTables()
{
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS words ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "tag TEXT, "
               "name TEXT, "
               "meaning TEXT, "
               "rating INTEGER"
               ")"
               );
    ERRCHK(query);
}
void ayikDB::insertWord(const ayikWord& w)
{
    createDbTables();
    QSqlQuery query;
    query.exec("INSERT OR IGNORE INTO words(tag,name,meaning,rating) VALUES("
               "'"+w.getTag()+"',"
               "'"+w.getName()+"',"
               "'"+w.getMeaning()+"',"
               "'"+w.getRating()+"')");

    /*if(query.lastError().type() != QSqlError::NoError) {
       QMessageBox::critical(0, qApp->tr("Query failed"), query.lastError().text(), QMessageBox::Cancel);
     }*/

    ERRCHK(query);
}
/*void ayikDB::insertWords(const QList<ayikWord>& wlist)
{
    QString err="";
    QString sqlstr;
    QueryResult res;
    db->execute("BEGIN TRANSACTION",res);
    for(int i=0;i<wlist.size();i++) {
        ayikWord w = wlist.at(i);
        sqlstr = "INSERT INTO words(tag,name,meaning,rating) VALUES("
                  "'"+w.getTag()+"',"
                  "'"+w.getName()+"',"
                  "'"+w.getMeaning()+"',"
                  "'"+w.getRating()+"')";
        if(db->execute(sqlstr,res)==0) {
        } else {
            err = db->error();
            qDebug()<<err;
        }
    }
    db->execute("END",res);
}*/
void ayikDB::insertWords(const QList<ayikWord>& wlist)
{
    QString err="";
    QStringList sqlstr;
    for(int i=0;i<wlist.size();i++) {
        ayikWord w = wlist.at(i);
        sqlstr.append( "INSERT INTO words(tag,name,meaning,rating) VALUES("
                  "'"+w.getTag()+"',"
                  "'"+w.getName()+"',"
                  "'"+w.getMeaning()+"',"
                  "'"+w.getRating()+"')");
    }
    if(db->execute(sqlstr)==0) {
    } else {
        err = db->error();
        qDebug()<<err;
    }
}

void ayikDB::deleteWord(const ayikWord& w,QString &err)
{
    err="";
    QueryResult res;
    if(db->execute("DELETE FROM words WHERE name='"+w.getName()+"'",res)==0) {
    } else {
        err = db->error();
    }
}
/*void ayikDB::deleteWords(const QList<ayikWord>& wlist,QString &err)
{
    err="";
    QString sqlstr;
    sqlstr = "DELETE FROM words WHERE name in (";
    for(int i=0;i<wlist.size();i++) {
        ayikWord w = wlist.at(i);
        sqlstr += "'"+w.getName()+"',";
    }
    sqlstr += "'')";
    //qDebug()<<sqlstr;
    QueryResult res;
    if(db->execute(sqlstr,res)==0) {
    } else {
        err = db->error();
        qDebug()<<err;
    }
}*/
void ayikDB::deleteWords(const QList<ayikWord>& wlist,QString &err)
{
    err="";
    QStringList sqlstr;
    for(int i=0;i<wlist.size();i++) {
        ayikWord w = wlist.at(i);
        sqlstr.append("DELETE FROM words WHERE name='"+w.getName()+"'");
    }
    QueryResult res;
    if(db->execute(sqlstr)==0) {
    } else {
        err = db->error();
        qDebug()<<err;
    }
}
void ayikDB::updateWord(const ayikWord& w)
{
    QString err="";
    QString sqlstr;
    QString meaning = w.getMeaning().replace("'","&#39;");
    sqlstr = "UPDATE words SET "
             "tag='"+w.getTag()+"', "
             "meaning='"+meaning+"', "
             "rating='"+w.getRating()+"' "
             "WHERE name='"+w.getName()+"'";
    QueryResult res;
    if(db->execute(sqlstr,res)==0) {
    } else {
        err = db->error();
        qDebug()<<err;
    }
}
void ayikDB::testDb()
{
    createDbTables();
    ayikWord w1("insane","deli","4");
    insertWord(w1);
    insertWord(w1);
    insertWord(w1);

    QSqlQuery query("SELECT name,meaning,rating FROM words");
    while (query.next()) {        
        qDebug() << query.value(0).toString()
                << query.value(1).toString()
                << query.value(2).toString();
    }
}
void ayikDB::truncateWords()
{
    createDbTables();
    QSqlQuery query;
    query.exec("DELETE FROM words");
    ERRCHK(query);
}
void ayikDB::getRandomWordMemdb(ayikWord& w,const QString& tag,const QString& rating,const QString& ratingOperator,QString &err)
{
    err="";
    QueryResult res;
    QString where=" WHERE ";
    if(tag!="") where += "tag='"+tag+"' AND ";
    where += "rating "+ratingOperator+" "+rating;
    QString sqlstr = "SELECT tag,name,meaning,rating FROM words"+where+" ORDER BY Random() LIMIT 1";
    //qDebug()<<sqlstr;
    if(db->execute(sqlstr,res)==0) {
        for(int i=0;i<res.records.size();i++) {
            QList<QString> row = res.records.at(i);
            w.setTag(row.at(0));
            w.setName(row.at(1));
            w.setMeaning(row.at(2));
            w.setRating(row.at(3));
            break;
        }
    } else {
        err = db->error();
    }
}
void ayikDB::getWordMemdb(ayikWord& w,const QString& name,QString &err)
{
    err="";
    QueryResult res;
    QString where=" WHERE ";
    where += "name='"+name+"'";
    QString sqlstr = "SELECT tag,name,meaning,rating FROM words"+where+" LIMIT 1";
    //qDebug()<<sqlstr;
    if(db->execute(sqlstr,res)==0) {
        for(int i=0;i<res.records.size();i++) {
            QList<QString> row = res.records.at(i);
            w.setTag(row.at(0));
            w.setName(row.at(1));
            w.setMeaning(row.at(2));
            w.setRating(row.at(3));
            break;
        }
    } else {
        err = db->error();
    }
}
void ayikDB::getWordList(QueryResult &res,QString &err)
{
    err="";
    if(db->execute("SELECT tag,name,rating,meaning FROM words ORDER BY name DESC",res)==0);
    else {
        err = db->error();
    }
}

void ayikDB::addWords(const QString& filename,const QString& tag,const QString& rating)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString text = in.readAll();
    QStringList lines = text.split("\n");
    QProgressDialog pd("Adding..", "Cancel", 0, lines.size());
    pd.setWindowModality(Qt::WindowModal);
    int steps=0;
    QList<ayikWord> wordlist;
    for(int i=0;i<lines.size();i++) {
        if (pd.wasCanceled()) break;
        QString line = lines.at(i);
        QStringList list1 = line.split("=");
        if(list1.length() > 1) {
            ayikWord a_word(list1.at(0),list1.at(1),rating,tag);
            wordlist.append(a_word);
        }
        if(wordlist.size()>50) {
            insertWords(wordlist);
            steps+=wordlist.size();
            wordlist.clear();
            pd.setValue(steps);
        }
    }
    if(wordlist.size()>0) {
        insertWords(wordlist);
        steps+=wordlist.size();
        wordlist.clear();
        pd.setValue(steps);
    }
}

void ayikDB::getTagList(QStringList &list,QString &err)
{
    err="";
    QueryResult res;
    if(db->execute("SELECT distinct(tag) FROM words",res)==0) {
        for(int i=0;i<res.records.size();i++) {
            list << res.records.at(i).at(0);
        }
    } else {
        err = db->error();
    }
}
