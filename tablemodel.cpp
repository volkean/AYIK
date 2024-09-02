#include "tablemodel.h"
#include <QColor>

TableModel::TableModel(QObject *parent)
        : QAbstractTableModel(parent)
{
}

TableModel::TableModel(QStringList &headers, QObject *parent)
        : QAbstractTableModel(parent)
{
    setHeaders(headers);
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return datalist.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
     return headers.size();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= datalist.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        QList<QVariant> row = datalist.at(index.row());

        if ( (index.column() < row.size()) && (index.column() >= 0) )
            return row.at(index.column()).toString();
    }

    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
         if( (section >=0) && (section < headers.size()))
            return headers.at(section);
    }
    return QVariant();
}

bool TableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; row++) {
        datalist.insert(row,QList<QVariant>());
    }

    endInsertRows();
    return true;
}

bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; ++row) {
        datalist.removeAt(position);
    }

    endRemoveRows();
    return true;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        QList<QVariant> r = datalist.value(row);
        if(index.column() >= 0) {
            if(index.column() < r.size()) {
                r.replace(index.column(),value);
            } else {
                r.append(value);
            }
        }
        datalist.replace(row, r);
        emit(dataChanged(index, index));

        return true;
    }

    return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

void TableModel::setHeaders(QStringList &headers)
{
    this->headers=headers;

    for(int i=0;i<headers.size();i++)
        setHeaderData(i,Qt::Horizontal,headers.at(i));

    //reset();
    resetInternalData();
}

void TableModel::clear()
{
    datalist.clear();
    emit(dataChanged(QModelIndex(), QModelIndex()));
}

QString TableModel::getValue(int row,int col)
{
    return datalist.at(row).at(col).toString();
}

void TableModel::setValue(int row,int col,const QString& val)
{
    QList<QVariant> r = datalist.value(row);
    if(col >= 0) {
        if(col < r.size()) {
            r.replace(col,val);
        } else {
            r.append(val);
        }
    }
    datalist.replace(row, r);
    ///todo: emit datachanged
}
