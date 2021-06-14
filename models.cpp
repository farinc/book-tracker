#include "mainwindow.h"
#include <QUrl>
#include <QDebug>
#include <QDialog>
#include <QDateTime>

#include "models.h"

using json = nlohmann::json;

BookItem::BookItem(bookdata::Book &book): book(book)
{}

QVariant BookItem::data(int column, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (column)
        {
            case 0:
                return book.bookID;
            case 1:
                return QDateTime::fromSecsSinceEpoch(book.lastEdit).toLocalTime().toString("MM/dd/yyyy h:mm ap");
            case 2:
                return QString::fromStdString(book.box);
            case 3:
                return QString::fromStdString(book.section);
        }
    }

    return QVariant();
}

BookConstantItem::BookConstantItem(const QString descritpion, double &constant): descritpion(descritpion), constant(constant)
{}

QVariant BookConstantItem::data(int column, int role) const
{
    if(role == Qt::DisplayRole)
    {
        switch (column)
        {
        case 0: return descritpion;
        case 1: return constant;
        }
    }else if(role == Qt::EditRole)
    {
        switch (column)
        {
        case 1: return constant;
        }
    }

    return QVariant();
}

Qt::ItemFlags BookConstantItem::flags(int column) const
{
    auto flags = Item::flags(column);
    return column == 1 ? flags | Qt::ItemIsEditable : flags;
}

bool BookConstantItem::setData(int column, const QVariant &data)
{
    //should always be column 1...
    constant = data.toDouble();
    return true;
}

PropItem::PropItem(const QString descritpion, const std::initializer_list<QVariant> values): descritpion(descritpion), values(values)
{}

QVariant PropItem::data(int column, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(column == 0)
        {
            return descritpion;
        }
        else if(column > 0)
        {
            if(column - 1 < values.size())
            {
                return values[column - 1];
            }
        }
    }

    return QVariant();
}

BasicModel::BasicModel(int columns, QWidget *parent): QAbstractItemModel(parent), columns(columns)
{}

BasicModel::~BasicModel()
{
    this->reset();
}

QModelIndex BasicModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row >= items.size() || row < 0)
        return QModelIndex();

    Item *item = items.at(row);
    return createIndex(row, column, item);
}

QModelIndex BasicModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int BasicModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return items.size();
}

int BasicModel::columnCount(const QModelIndex &parent) const
{   
    return columns;
}

QVariant BasicModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        Item *item = static_cast<Item*>(index.internalPointer());
        return item->data(index.column(), role);
    }
    return QVariant();
}

bool BasicModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(data(index, role) != value && role == Qt::EditRole)
    {
        Item *item = static_cast<Item*>(index.internalPointer());
        if(item->setData(index.column(), value))
        {
            emit dataChanged(index, index, QVector<int>() << role);
            return true;
        }
    }
    return false;
}

QVariant BasicModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(header_strings.size() <= section || section < 0)
        return QVariant();

    if(role == Qt::DisplayRole)
        return header_strings.at(section);

    return QVariant();
}

void BasicModel::setHeaderData(const std::initializer_list<QString> &list)
{
    this->header_strings = list;
}

Qt::ItemFlags BasicModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    Item *item = static_cast<Item*>(index.internalPointer());
    return item->flags(index.column());
}

void BasicModel::reset()
{
    qDeleteAll(items);
    items.clear();
    header_strings.clear();
}

void BasicModel::addItem(Item *item)
{
    this->items.push_back(item);
}
