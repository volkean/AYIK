#include "ayikdb.h"
#include "dictutil.h"
#include <QtXml>
#include <QtWidgets/QProgressDialog>

#define ERRCHK(x) if(x.lastError().type() != QSqlError::NoError) { \
qDebug() << x.lastError().text(); \
        return; \
    }

AyikDB* AyikDB::instance;

AyikDB::AyikDB()
{
    instance = this;
    connect();
    createDbTables();
}
AyikDB* AyikDB::getInstance()
{
    if(instance==NULL) instance = new AyikDB();
    return instance;
}
int AyikDB::loadWordsXml(const QString& filename,bool append)
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
                AyikWord a_word(name,meaning,rating,tag);
                words.append(a_word);
            }
        }

        n = n.nextSibling();
    }
    return 0;
}
void AyikDB::loadWords(const QString& filename,bool append)
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
            AyikWord a_word(list1.at(0),list1.at(1));
            words.append(a_word);
        }
    }
}
void AyikDB::loadWordMemdb(const QString& filename,bool append)
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
            AyikWord a_word(list1.at(0),list1.at(1));
            insertWord(a_word);
        }
    }
}
void AyikDB::loadWords(const QStringList& filenames,bool append)
{
    if(!append) words.clear();

    for(int i=0;i<filenames.size();i++) {
        QString filename = "data/"+filenames.at(i);
        loadWordMemdb(filename,true);
    }
    //testDb();
    regenerateShuffle();
}
void AyikDB::regenerateShuffle()
{
    //lets create a shuffle list
    DictUtil::randomSequence(seq,words.size());
}
void AyikDB::createDbTables()
{
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS words ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "tag TEXT, "
               "name TEXT, "
               "meaning TEXT, "
               "rating INTEGER"
               ")"
               );
    ERRCHK(query);
    query.exec("CREATE TABLE IF NOT EXISTS config ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT, "
               "value TEXT"
               ")"
               );
    ERRCHK(query);
    query.exec("INSERT INTO words(tag,name,meaning,rating) SELECT 'example','awake','',5 "
               "WHERE NOT EXISTS (SELECT 1 FROM config)"
               );
    ERRCHK(query);
    query.exec("INSERT INTO config(name,value) VALUES('example-loaded','true')"
               );
    ERRCHK(query);
}
void AyikDB::insertWord(const AyikWord& w)
{
    createDbTables();
    QSqlQuery query(db);
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
/*void AyikDB::insertWords(const QList<AyikWord>& wlist)
{
    QString err="";
    QString sqlstr;
    QueryResult res;
    execute("BEGIN TRANSACTION",res);
    for(int i=0;i<wlist.size();i++) {
        AyikWord w = wlist.at(i);
        sqlstr = "INSERT INTO words(tag,name,meaning,rating) VALUES("
                  "'"+w.getTag()+"',"
                  "'"+w.getName()+"',"
                  "'"+w.getMeaning()+"',"
                  "'"+w.getRating()+"')";
        if(execute(sqlstr,res)==0) {
        } else {
            err = error();
            qDebug()<<err;
        }
    }
    execute("END",res);
}*/
void AyikDB::insertWords(const QList<AyikWord>& wlist)
{
    QString err="";
    QStringList sqlstr;
    for(int i=0;i<wlist.size();i++) {
        AyikWord w = wlist.at(i);
        sqlstr.append( "INSERT INTO words(tag,name,meaning,rating) VALUES("
                  "'"+w.getTag()+"',"
                  "'"+w.getName()+"',"
                  "'"+w.getMeaning()+"',"
                  "'"+w.getRating()+"')");
    }
    if(execute(sqlstr)==0) {
    } else {
        err = error();
        qDebug()<<err;
    }
}

void AyikDB::deleteWord(const AyikWord& w,QString &err)
{
    err="";
    QueryResult res;
    if(execute("DELETE FROM words WHERE name='"+w.getName()+"'",res)==0) {
    } else {
        err = error();
    }
}
/*void AyikDB::deleteWords(const QList<AyikWord>& wlist,QString &err)
{
    err="";
    QString sqlstr;
    sqlstr = "DELETE FROM words WHERE name in (";
    for(int i=0;i<wlist.size();i++) {
        AyikWord w = wlist.at(i);
        sqlstr += "'"+w.getName()+"',";
    }
    sqlstr += "'')";
    //qDebug()<<sqlstr;
    QueryResult res;
    if(execute(sqlstr,res)==0) {
    } else {
        err = error();
        qDebug()<<err;
    }
}*/
void AyikDB::deleteWords(const QList<AyikWord>& wlist,QString &err)
{
    err="";
    QStringList sqlstr;
    for(int i=0;i<wlist.size();i++) {
        AyikWord w = wlist.at(i);
        sqlstr.append("DELETE FROM words WHERE name='"+w.getName()+"'");
    }
    QueryResult res;
    if(execute(sqlstr)==0) {
    } else {
        err = error();
        qDebug()<<err;
    }
}
void AyikDB::deleteWordsLike(const QString& filter,QString &err) {
    err="";
    QStringList sqlstr;
    sqlstr.append("DELETE FROM words WHERE tag like '%"+filter+"%' or name like '%"+filter+"%'");
    QueryResult res;
    if(execute(sqlstr)==0) {
    } else {
        err = error();
        qDebug()<<err;
    }
}
void AyikDB::updateWord(const AyikWord& w)
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
    if(execute(sqlstr,res)==0) {
    } else {
        err = error();
        qDebug()<<err;
    }
}
void AyikDB::testDb()
{
    createDbTables();
    AyikWord w1("insane","deli","4");
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
void AyikDB::truncateWords()
{
    createDbTables();
    QSqlQuery query;
    query.exec("DELETE FROM words");
    ERRCHK(query);
}
void AyikDB::getRandomWord(AyikWord& w,const QString& tag,const QString& rating,const QString& ratingOperator,QString &err)
{
    err="";
    QueryResult res;
    QString where=" WHERE ";
    if(tag!="") where += "tag='"+tag+"' AND ";
    where += "rating "+ratingOperator+" "+rating;
    QString sqlstr = "SELECT tag,name,meaning,rating FROM words"+where+" ORDER BY Random() LIMIT 1";
    //qDebug()<<sqlstr;
    if(execute(sqlstr,res)==0) {
        for(int i=0;i<res.records.size();i++) {
            QList<QString> row = res.records.at(i);
            w.setTag(row.at(0));
            w.setName(row.at(1));
            w.setMeaning(row.at(2));
            w.setRating(row.at(3));
            break;
        }
    } else {
        err = error();
    }
}
void AyikDB::getWord(AyikWord& w,const QString& name,QString &err)
{
    err="";
    QueryResult res;
    QString where=" WHERE ";
    where += "name='"+name+"'";
    QString sqlstr = "SELECT tag,name,meaning,rating FROM words"+where+" LIMIT 1";
    //qDebug()<<sqlstr;
    if(execute(sqlstr,res)==0) {
        for(int i=0;i<res.records.size();i++) {
            QList<QString> row = res.records.at(i);
            w.setTag(row.at(0));
            w.setName(row.at(1));
            w.setMeaning(row.at(2));
            w.setRating(row.at(3));
            break;
        }
    } else {
        err = error();
    }
}
void AyikDB::getWordList(QueryResult &res,QString &err)
{
    err="";
    if(execute("SELECT tag,name,rating,meaning FROM words ORDER BY name DESC",res)==0);
    else {
        err = error();
    }
}

void AyikDB::addWords(const QString& filename,const QString& tag,const QString& rating)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString text = in.readAll();
    QStringList lines = text.split("\n");
    QProgressDialog progressDialog("Adding..", "Cancel", 0, lines.size());
    progressDialog.setWindowModality(Qt::WindowModal);
    int steps=0;
    QList<AyikWord> wordList;
    for(int i=0;i<lines.size();i++) {
        if (progressDialog.wasCanceled()) break;
        QString line = lines.at(i);
        QStringList splittedStringList = line.split("=");//word=meaning
        if(splittedStringList.length() > 1) {
            AyikWord ayikWord(splittedStringList.at(0),splittedStringList.at(1),rating,tag);
            wordList.append(ayikWord);
        } else if(splittedStringList.length() > 0) {
            AyikWord ayikWord(splittedStringList.at(0),"",rating,tag);
            wordList.append(ayikWord);
        }
        if(wordList.size()>50) {
            insertWords(wordList);
            steps+=wordList.size();
            wordList.clear();
            progressDialog.setValue(steps);
        }
    }
    if(wordList.size()>0) {
        insertWords(wordList);
        steps+=wordList.size();
        wordList.clear();
        progressDialog.setValue(steps);
    }
}

void AyikDB::getTagList(QStringList &list,QString &err)
{
    err="";
    QueryResult res;
    if(execute("SELECT distinct(tag) FROM words",res)==0) {
        for(int i=0;i<res.records.size();i++) {
            list << res.records.at(i).at(0);
        }
    } else {
        err = error();
    }
}


int AyikDB::connect()
{
    errstr="";
    if(!db.isOpen()) {
        db = QSqlDatabase::addDatabase("QSQLITE", "ayik.sqlite");
        db.setHostName("test");
        db.setDatabaseName("ayik.sqlite");
        db.setUserName("test");
        db.setPassword("test");

        if(!db.open() || db.isOpenError()) {
            errstr = db.lastError().text();
            qDebug()<<db.lastError()<<" "<<errstr;
            return -1;
        }
    }
    return 0;
}

int AyikDB::execute(const QString& sqlstr, QueryResult& qr)
{
    errstr="";
    if(sqlstr.isEmpty()) {
        errstr = "Empty sql statement";
        return -1;
    }
    if(connect()==0) {
        //db.transaction();
        QSqlQuery query(db);
        if (query.exec(sqlstr) == false) {
            errstr = query.lastError().text();
            return -1;
        } else {

            QSqlRecord rec = query.record();
            for (int c = 0; c < rec.count(); c++) {
                qr.headers.push_back(rec.fieldName(c));
            }

            while (query.next()) {
                QList<QString> row;
                for (int c = 0; c < rec.count(); c++) {
                    row.push_back(query.value(c).toString());
                }
                qr.records.push_back(row);
            }
        }
        //db.commit();
    } else {//error
        return -1;
    }
    return 0;
}
int AyikDB::execute(const QStringList& sqlstr) {
    errstr="";
    if(sqlstr.isEmpty()) {
        errstr = "Empty sql statement";
        return -1;
    }
    if(connect()==0) {
        db.transaction();
        QSqlQuery query(db);
        for(int i=0;i<sqlstr.size();i++) {
            if (query.exec(sqlstr.at(i)) == false) {
                errstr = query.lastError().text();
                return -1;
            } else {
            }
        }
        db.commit();
    } else {//error
        return -1;
    }
    return 0;
}

QString AyikDB::error()
{
    return errstr;
}
