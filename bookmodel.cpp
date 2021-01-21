#include "bookmodel.h"

#include <ctime>
#include <QUrl>

Item::Item()
{
    childList = QList<Item*>();
    dataList = QList<QString>();
}

Item::~Item()
{
    qDeleteAll(childList);
}

Item* Item::child(int row) const
{
    return this->childList.at(row);
}

int Item::childCount() const
{
    return this->childList.size();
}
Item* Item::parent() const
{
    return this->parentItem;
}
int Item::row() const
{
    return this->parentItem->childList.indexOf(const_cast<Item*>(this));
}
int Item::dataSize() const
{
    return this->dataList.size();
}
QString Item::data(int column) const
{
    return this->dataList.at(column);
}

BookItem::BookItem(const Book book)
{
    dataList.append(QString::fromStdString(book.box));
    dataList.append(QString::fromStdString(book.section));
    dataList.append(QString::fromStdString(book.pageMaterial));
    dataList.append(QString::fromStdString(book.coverMaterial));
}

BatchItem::BatchItem()
{
   lastEdit = time(0);
   dataList.append(QString::fromLocal8Bit(std::ctime(&lastEdit)));
}

BookModel::BookModel(QObject *parent)
{

}

BookModel::~BookModel()
{
    delete rootItem;
}

QVariant BookModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    Item *item = static_cast<Item*>(index.internalPointer());

    return item->data(index.column());
}

QModelIndex BookModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    Item *parentItem;

    if(!parent.isValid()){
        parentItem = rootItem;
    }else{
        parentItem = static_cast<Item*>(parent.internalPointer());
    }

    Item *childItem = parentItem->child(row);
    if(childItem != nullptr){
        createIndex(row, column, childItem); //index the child
    }

    return QModelIndex();
}

QModelIndex BookModel::parent(const QModelIndex &index) const
{
    if(!index.isValid()){
        return QModelIndex();
    }

    Item *childItem = static_cast<Item*>(index.internalPointer());
    Item *parentItem = childItem->parent();

    if(parentItem != rootItem){
        return createIndex(parentItem->row(), 0, parentItem);
    }

    return QModelIndex();
}

int BookModel::rowCount(const QModelIndex &parent) const
{
    Item *parentItem;
    if(parent.column() > 0){
        return 0;
    }

    if(!parent.isValid()){
        parentItem = rootItem;
    }else{
        parentItem = static_cast<Item*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int BookModel::columnCount(const QModelIndex &parent) const
{
    Item* parentItem;
    if(!parent.isValid()){
        parentItem = rootItem;
    }else{
        parentItem = static_cast<Item*>(parent.internalPointer());
    }

    return parentItem->dataSize();
}
