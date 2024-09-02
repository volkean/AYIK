#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QtGui>
#include <QtWidgets/QItemDelegate>
#include <QtWidgets/QStyleOptionViewItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

class ItemDelegate : public QItemDelegate
{
    Q_OBJECT

public:

    ItemDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // ITEMDELEGATE_H
