#include "models.h"
#include "mainwindow.h"
#include <QUrl>
#include <QDebug>
#include <QDialog>
#include <QDateTime>

using json = nlohmann::json;

Item::Item(QString type): parentItem(nullptr)
{
    childList = QList<Item*>();
    dataList = QList<QVariant>();
    tp = type;
}

Item::~Item()
{
    qDeleteAll(childList);
    childList.clear();
}

int Item::row() const
{
    if(parentItem == nullptr)
        return 0;

    return this->parentItem->childList.indexOf(const_cast<Item*>(this));
}

Item* Item::parent() const
{
    return this->parentItem;
}

Item* Item::child(int row) const
{
    if (row >= this->childCount() || row < 0)
        return nullptr;
    return this->childList.at(row);
}

bool Item::hasChild(int row) const
{
    return this->child(row) != nullptr ? true : false;
}

int Item::childCount() const
{
    return this->childList.size();
}

void Item::appendChild(Item *item)
{
    if(item == nullptr)
        return;

    this->childList.append(item);
    item->parentItem = this;
}

void Item::removeChild(Item *item)
{
    if(!hasChild(this->childList.indexOf(item)))
        return;

    this->childList.removeOne(item);
    item->parentItem = nullptr;
}

void Item::removeChild(int row)
{
    if(!hasChild(row))
        return;

    this->childList.removeAt(row);
    child(row)->parentItem = nullptr;
}

void Item::setData(int column, QVariant data)
{
    this->dataList.insert(column, data);
}

void Item::appendData(QVariant data)
{
    dataList << data;
}

QVariant Item::data(int column) const
{
    if(column >= dataSize() || column < 0)
        return QVariant();
    return this->dataList.at(column);
}

int Item::dataSize() const
{
    return this->dataList.size();
}

QString Item::type() const
{
    return itemType;
}

PropItem::PropItem(): Item("prop")
{

}

void PropItem::appendData(QVariant in)
{
    Item::appendData(in);
}

RootItem::RootItem(const int &maxColumns): Item("root"), maxColumns(maxColumns)
{

}

void RootItem::appendItem(Item *item)
{
    Item::appendChild(item);
}

void RootItem::removeItem(int index)
{
    Item::removeChild(index);
}

void RootItem::removeItem(Item *item)
{
    Item::removeChild(item);
}

BatchItem::BatchItem(int batchID): Item("batch")
{
    this->setData(0, "Batch " + QString::number(batchID));
    this->setData(1, "");
    this->setData(2, "");
    this->setData(3, "");
    this->setData(4, "");

    this->batchID = batchID;

    itemType = "batch";
}

void BatchItem::appendBook(BookItem *book)
{
    this->appendChild(book);
}

void BatchItem::removeBook(BookItem *book)
{
    this->removeChild(book);
}

int BatchItem::id() const
{
    return this->batchID;
}

BookItem::BookItem(Book book): Item("book"), book(book)
{    
    QDateTime timestamp;
    timestamp.setTime_t(book.lastEdit);

    this->setData(0, "Book " + QString::number(book.bookID));
    this->setData(1, timestamp.toLocalTime());
    this->setData(2, QString::fromStdString(book.box));
    this->setData(3, QString::fromStdString(book.section));
    this->setData(4, QString::fromStdString(getString(book.status)));

    itemType = "book";
}

Model::Model(RootItem *rootItem, QObject *parent): QAbstractItemModel(parent), rootItem(rootItem)
{
}

QModelIndex Model::index(int row, int column, const QModelIndex &parent) const
{
    Item *parentItem;
    if(!parent.isValid())
    {
        parentItem = rootItem;
    }
    else
    {
        parentItem = static_cast<Item*>(parent.internalPointer());
    }

    Item *childItem = parentItem->child(row);

    if(childItem != nullptr)
    {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex Model::parent(const QModelIndex &index) const
{
    Item *item = static_cast<Item*>(index.internalPointer());
    Item *parentItem = item->parent();

    if(parentItem != nullptr && parentItem->type() != "root")
    {
        return createIndex(parentItem->row(), 0, parentItem);
    }
    return QModelIndex();
}

int Model::rowCount(const QModelIndex &parent) const
{
    const Item *parentItem;

    if(!parent.isValid())
    {
        parentItem = rootItem;
    }
    else
    {
        parentItem = static_cast<Item*>(parent.internalPointer());
    }

    if(parentItem != nullptr)
        return parentItem->childCount();

    return 0;
}

int Model::columnCount(const QModelIndex &parent) const
{   
    if(!parent.isValid())
    {
        return rootItem->maxColumns;
    }
    else
    {
        Item *parentItem = static_cast<Item*>(parent.internalPointer());
        return parentItem->dataSize();
    }
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    Item *item = static_cast<Item*>(index.internalPointer());
    return item->data(index.column());
}

void Model::setRootItem(RootItem *rt)
{
    beginResetModel();
    delete rootItem;
    rootItem = rt;
    endResetModel();
}
BookModel::BookModel(RootItem *rootItem, QString type, int nextBookID, QObject *parent): Model(rootItem, parent), type(type), nextBookID(nextBookID)
{

}

void BookModel::onItemSelectionSingle(const QModelIndex &index1)
{
    if (type == "edit")
    {
        Item *item = static_cast<Item*>(index1.internalPointer());

        if(item->type() == "book")
        {
            Book book = static_cast<BookItem*>(item)->book;
            emit bookLoad(book);
            emit done(QDialog::Accepted);
        }
    }
    else if(type == "new")
    {
        Item *item = static_cast<Item*>(index1.internalPointer());

        if(item->type() == "batch")
        {
            BatchItem *batchItem = static_cast<BatchItem*>(item);
            Book book = Book(nextBookID, batchItem->id());
            emit bookLoad(book);
            emit done(QDialog::Accepted);
        }
    }
}

void BookModel::onItemSelectionDuo(const QModelIndex &index1, const QModelIndex &index2)
{
    if(type == "move")
    {
        Item *item1 = static_cast<Item*>(index1.internalPointer());
        Item *item2 = static_cast<Item*>(index2.internalPointer());

        BatchItem *batchItem;
        BookItem *bookItem;

        bool flag1 = item1->type() == "book" && item2->type() == "batch";
        bool flag2 = item1->type() == "batch" && item2->type() == "book";

        if(flag1)
        {
            batchItem = static_cast<BatchItem*>(index2.internalPointer());
            bookItem = static_cast<BookItem*>(index1.internalPointer());
        }
        else if(flag2)
        {
            batchItem = static_cast<BatchItem*>(index1.internalPointer());
            bookItem = static_cast<BookItem*>(index2.internalPointer());
        }
        else return;

        if(batchItem->id() != bookItem->book.batchID)
        {
            Book book = bookItem->book;
            emit bookMove(book, batchItem->id());
            emit done(QDialog::Accepted);
        }
    }
}

QVariant BookModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0: return tr("Discription");
        case 1: return tr("Last Edited");
        case 2: return tr("Box");
        case 3: return tr("Section");
        case 4: return tr("Status");
        }
    }

    return QVariant();
}

Qt::ItemFlags BookModel::flags(const QModelIndex &index) const
{
    if(index.column() > 0)
        return Qt::ItemIsEnabled;

    QString typeItem = static_cast<Item*>(index.internalPointer())->type();

    //select only books and allow the books to be dragable
    if(type == "edit")
    {
        if(typeItem == "batch")
        {
             return Qt::ItemIsEnabled;
        }
        else if(typeItem == "book")
        {
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
    }
    //select only batchs
    else if(type == "new")
    {
        if(typeItem == "batch")
        {
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        else if(typeItem == "book")
        {
            return Qt::ItemIsEnabled;
        }
    }
    else if(type == "move")
    {
        if(typeItem == "batch")
        {
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
        else if(typeItem == "book")
        {
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
        }
    }

    return Qt::NoItemFlags;
}

PropsModel::PropsModel(RootItem *rootItem, QList<QString> header, QObject *parent): Model(rootItem, parent), header(header)
{

}

QVariant PropsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section < rootItem->maxColumns && section >= 0)
            return header.at(section);
    }

    return QVariant();
}

Qt::ItemFlags PropsModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled;
}
