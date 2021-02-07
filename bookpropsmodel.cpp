#include "bookpropsmodel.h"

BookPropsModel::BookPropsModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    list = QList<PropItem*>();
}

BookPropsModel::~BookPropsModel()
{
    qDeleteAll(list);
}

QModelIndex BookPropsModel::index(int row, int column, const QModelIndex &parent) const
{
    PropItem *item = list.at(row);
    return createIndex(row, column, item);
}

QModelIndex BookPropsModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int BookPropsModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid()) //because no other parents exists...
        return 0;

    return list.size();
}

int BookPropsModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant BookPropsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole)
    {
        if (section == 0)
        {
            return tr("Properity");
        }
        if (section == 1)
        {
            return tr("Value");
        }
    }
    return QVariant();
}

QVariant BookPropsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        PropItem *item = static_cast<PropItem*>(index.internalPointer());
        switch (index.column())
        {
        case 0: return item->title;
        case 1: return item->data;
        default: QVariant();
        }
    }
    return QVariant();
}

void BookPropsModel::addItem(PropItem *item)
{
    this->list.append(item);
}

void BookPropsModel::reset()
{
    beginResetModel();
    qDeleteAll(list.begin(), list.end());
    list.clear();
    endResetModel();
}

PropItem::PropItem(QString title, QVariant data): title(title), data(data)
{
}
