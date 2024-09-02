#pragma once
#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QtSql>
#include <iostream>
using namespace std;

/* Change these to match your database */
#define HOST   "TEST"
#define DB     "TEST"
#define PORT   "1521"
#define USER   "TEST"
#define PASSWD "TEST"

class QueryResult {
public:
    QList<QString> headers;
    QList< QList<QString> > records;
};

class DatabaseConnection
{
public:
    DatabaseConnection(const QString& id="default", const QString& type="QSQLITE");//database type
    static DatabaseConnection* getInstance();//return last instance
    int connect();
    int execute(const QString& sqlstr, QueryResult& qr);
    int execute(const QStringList& sqlstr);
    /*int update(QString &sqlstr);*/
    void setDatabaseId(const QString& id);
    void setDatabaseType(const QString& type);
    void setConnectionParams(const QString& host,const QString& port,const QString& user,const QString& pass,const QString& dbname="");
    QString error();

private:
    static DatabaseConnection* instance;
    QSqlDatabase sqldb;
    QString errstr;

    //database id and type.
    QString id;
    QString type;
    //connection parameters
    QString host;
    QString port;
    QString user;
    QString pass;
    QString dbname;//oracle sid or service name or sqlite dbname
};

#endif // DATABASECONNECTION_H
