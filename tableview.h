#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QtGui>
#include "tablemodel.h"
#include "itemdelegate.h"
#include <QtWidgets/QTableView>
#include <QtWidgets/QHeaderView>

class TableView
{
public:
    TableView();
    QTableView* getView();
    void test();

    QTableView *view;
    TableModel *model;
    QSortFilterProxyModel *proxyModel;
    //ItemDelegate *delegate;//not used yet
};

#endif // TABLEVIEW_H
