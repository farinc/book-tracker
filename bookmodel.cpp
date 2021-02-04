#include "bookmodel.h"

#include <ctime>
#include <QUrl>

#include <QDebug>
#include <QDialog>

Item::Item()
{
    childList = QList<Item*>();
}

Item::~Item()
{
    qDeleteAll(childList);
    delete parentItem;
}

int Item::row() const
{
    return this->parentItem->childList.indexOf(const_cast<Item*>(this));
}

Item* Item::parent() const
{
    return this->parentItem;
}

void Item::setParent(Item *parent)
{
    this->parentItem = parent;
}

Item* Item::child(int row) const
{
    if (row >= this->childCount() || row < 0)
        return nullptr;
    return this->childList.at(row);
}

void Item::setChild(int row, Item *child)
{
    this->childList.insert(row, child);
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
    this->childList.append(item);
    if (item != nullptr)
        item->setParent(this);
}

void Item::removeChild(Item *item)
{
    this->childList.removeOne(item);
    if (item != nullptr)
        item->setParent(nullptr);
}

void Item::setData(int column, QString data)
{
    this->dataList.insert(column, data);
}

QString Item::data(int column)
{
    if(column >= dataSize() || column < 0)
        return nullptr;
    return this->dataList.at(column);
}

int Item::dataSize()
{
    return this->dataList.size();
}

QString Item::type() const
{
    return itemType;
}

BatchItem::BatchItem(int batchID): Item()
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

void BatchItem::setBook(int row, BookItem *book)
{
    this->setChild(row, book);
}

void BatchItem::removeBook(BookItem *book)
{
    this->removeChild(book);
}

int BatchItem::id() const
{
    return this->batchID;
}

BookItem::BookItem(Book &book): Item(), book(book)
{    
    char lastEditStr[80];
    char creationStr[80];

    strftime(lastEditStr, 80, "%b %d %Y %I:%M%p", localtime(&book.lastEdit));
    strftime(creationStr, 80, "%b %d %Y %I:%M%p", localtime(&book.creation));

    this->setData(0, "Book " + QString::number(book.bookID));
    this->setData(1, QString::fromLocal8Bit(lastEditStr));
    this->setData(2, QString::fromLocal8Bit(creationStr));
    this->setData(3, QString::fromStdString(book.box));
    this->setData(4, QString::fromStdString(book.section));

    itemType = "book";
}

RootItem::RootItem(): Item()
{
    this->setData(0, "Books");
    this->setData(1, "Last Edited");
    this->setData(2, "First Created");
    this->setData(3, "Box");
    this->setData(4, "Section");

    itemType = "root";
}

void RootItem::appendBatch(BatchItem *batch)
{
    this->appendChild(batch);
}

void RootItem::removeBatch(BatchItem *batch)
{
    this->removeChild(batch);
}

BookModel::BookModel(QMultiMap<int, Book> data, QString type, QObject *parent): QAbstractItemModel(parent)
{
    rootItem = new RootItem();
    this->type = type;
    populateModel(data);
}

void BookModel::populateModel(QMultiMap<int, Book> data)
{
    //iterate through the possible batch ids starting at 0. We stop once there is no key in the map that correspods to the batch id.
    bool flag = true;

    for(int batch = 0; flag; batch++)
    {
        if(data.contains(batch)){
            //Ok, create a batch node (item) and append it to root
            BatchItem *batchItem = new BatchItem(batch);
            QList<Book> booksInBatch = data.values(batch);
            for(Book bk: booksInBatch)
            {
                batchItem->appendBook(new BookItem(bk));
            }
            rootItem->appendBatch(batchItem);
        }else{
            if (this->type == "new" || this->type == "move")
            {
                //At the end however, add a "batch" that repersents a new batch yet to be created.
                BatchItem *newBatchItem = new BatchItem(batch); //this particular batchID is outside of range, so it must be the future batchID
                rootItem->appendBatch(newBatchItem);
            }

            flag = false;
        }
    }
}

void BookModel::onDoubleClicked(const QModelIndex &selection)
{
    if (type == "edit")
    {
        Item *item = static_cast<Item*>(selection.internalPointer());

        if(item->type() == "book")
        {
            Book book = static_cast<BookItem*>(item)->book;
            emit bookLoad(book);
            emit done(QDialog::Accepted);
        }
    }
    else if(type == "new")
    {
        Item *item = static_cast<Item*>(selection.internalPointer());

        if(item->type() == "batch")
        {
            //do something!!
            emit done(QDialog::Accepted);
        }
    }
    emit done(QDialog::Rejected);
}

BookModel::~BookModel()
{
    //delete rootItem;
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

QVariant BookModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

Qt::ItemFlags BookModel::flags(const QModelIndex &index) const
{
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

    if(childItem != nullptr)
    {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex BookModel::parent(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return QModelIndex();
    }

    Item *childItem = static_cast<Item*>(index.internalPointer());
    Item *parentItem = childItem->parent();

    if(parentItem->type() != "root")
    {
        return createIndex(parentItem->row(), 0, parentItem);
    }

    return QModelIndex();
}

int BookModel::rowCount(const QModelIndex &parent) const
{
    Item *parentItem;
    if(parent.column() > 0)
        return 0;

    if(!parent.isValid())
    {
        parentItem = rootItem;
    }
    else
    {
        parentItem = static_cast<Item*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int BookModel::columnCount(const QModelIndex &parent) const
{
    Item* parentItem;
    if(!parent.isValid())
    {
        parentItem = rootItem;
    }
    else
    {
        parentItem = static_cast<Item*>(parent.internalPointer());
    }

    return parentItem->dataSize();
}
