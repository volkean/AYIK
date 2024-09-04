#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QtGui>
#include <QtWidgets/QTableView>
#include <QtWidgets/QHeaderView>
#include <QSqlTableModel>

class TableView : public QTableView
{
    Q_OBJECT

public:
    explicit TableView(QSqlDatabase &db);

    QSqlTableModel *model;
    // QSortFilterProxyModel *proxyModel;
};

#endif // TABLEVIEW_H
