#include "tableview.h"

TableView::TableView(QSqlDatabase &db) : QTableView()
{
    this->model = new QSqlTableModel(NULL, db);
    this->setModel(model);
    this->setSortingEnabled(true);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->verticalHeader()->setVisible(true);
}
