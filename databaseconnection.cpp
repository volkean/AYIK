#include <QtGui>
#include "databaseconnection.h"

DatabaseConnection* DatabaseConnection::instance;

DatabaseConnection::DatabaseConnection(const QString& id, const QString &type)
{
    setDatabaseId(id);
    setDatabaseType(type);
    //setConnectionParams(HOST,PORT,USER,PASSWD,DB);
}

DatabaseConnection* DatabaseConnection::getInstance()
{
    if(instance==NULL) instance = new DatabaseConnection();
    return instance;
}

int DatabaseConnection::connect()
{
    errstr="";
    if(sqldb.isValid()==false) {

        QSqlDatabase qDB = QSqlDatabase::addDatabase(type);

        qDB.setHostName(host);
        qDB.setDatabaseName(dbname);
        qDB.setPort(port.toInt());
        qDB.setUserName(user);
        qDB.setPassword(pass);

        /* Probably overkill ;) */
        //qDB.setConnectOptions("Prefetch_Memory=5242880;Prefetch_Rows=250");

        if (qDB.open() == false) {
            errstr = qDB.lastError().text();
            return -1;
        }
        sqldb = QSqlDatabase::cloneDatabase(qDB,id);
    }
    if(sqldb.isOpen()==false) {
        if (sqldb.open()==false) {
            errstr = sqldb.lastError().text();
            return -1;
        }
    }
    return 0;
}

int DatabaseConnection::execute(const QString& sqlstr, QueryResult& qr)
{
    errstr="";
    if(sqlstr.isEmpty()) {
        errstr = "Empty sql statement";
        return -1;
    }
    if(connect()==0) {

        //sqldb.transaction();
        QSqlQuery query(sqldb);        
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
        //sqldb.commit();
    } else {//error
        return -1;
    }
    return 0;
}
int DatabaseConnection::execute(const QStringList& sqlstr) {
    errstr="";
    if(sqlstr.isEmpty()) {
        errstr = "Empty sql statement";
        return -1;
    }
    if(connect()==0) {

        sqldb.transaction();
        QSqlQuery query(sqldb);
        for(int i=0;i<sqlstr.size();i++) {
            if (query.exec(sqlstr.at(i)) == false) {
                errstr = query.lastError().text();
                return -1;
            } else {
            }
        }
        sqldb.commit();
    } else {//error
        return -1;
    }
    return 0;
}
void DatabaseConnection::setDatabaseId(const QString& id)
{
    this->id=id;
}
void DatabaseConnection::setDatabaseType(const QString& type)
{
    this->type=type;
}
void DatabaseConnection::setConnectionParams(const QString& host,const QString& port,const QString& user,const QString& pass,const QString& dbname)
{
    this->host=host;
    this->port=port;
    this->user=user;
    this->pass=pass;
    this->dbname=dbname;
}

QString DatabaseConnection::error()
{
    return errstr;
}
