#include "tableview.h"

TableView::TableView()
{
    model = new TableModel();
    view = new QTableView;


    proxyModel = new QSortFilterProxyModel(view);
    proxyModel->setSourceModel(model);
    proxyModel->setDynamicSortFilter(true);

    view->setModel(proxyModel);
    view->setSortingEnabled(true);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //proxyModel->setFilterKeyColumn(-1);//all columns
    proxyModel->setFilterKeyColumn(1);//name column
    proxyModel->sort(1, Qt::AscendingOrder);

    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    view->verticalHeader()->setVisible(false);

    //delegate = new ItemDelegate();//not used
    //view->setItemDelegate(delegate);//not used
}

QTableView* TableView::getView()
{
    return view;
}

void TableView::test()
{
    //qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QRandomGenerator::global()->seed(QRandomGenerator::global()->generate());
    //int headcount = qrand() % 10+1;
    int headcount = QRandomGenerator::global()->generate() % 10 + 1;
    QStringList headers;
    for(int i=0;i<headcount;i++) headers<<QString::number(i);
    model->setHeaders(headers);

    for(int i=0;i<10;i++) {
        model->insertRows(0, 1, QModelIndex());

        QModelIndex index = model->index(0, 0, QModelIndex());
        model->setData(index, QString::number(i), Qt::EditRole);
        index = model->index(0, 1, QModelIndex());
        model->setData(index, QString::number(i*i), Qt::EditRole);
    }
}
