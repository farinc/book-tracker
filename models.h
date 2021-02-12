#ifndef MODELS_H
#define MODELS_H

#include "book.h"
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QDir>

class Item {
public:
    Item(QString type);
    ~Item ();
    Item* parent() const;
    int row() const;
    Item* child(int row) const;
    bool hasChild(int row) const;
    int childCount() const;
    QString data(int column) const ;
    int dataSize() const ;
    QString type() const;

protected:
    void appendChild(Item *item);
    void removeChild(Item *item);
    void removeChild(int row);
    void setData(int column, QString data);
    void appendData(QString data);
    QString itemType;
    Item* parentItem;

private:
    QString tp;
    QList<Item*> childList;
    QList<QString> dataList;
};

class RootItem : public Item
{
public:
    RootItem(const int &maxColumns);
    void appendItem(Item *item);
    void removeItem(int index);
    void removeItem(Item *item);
    const int maxColumns;
};


class PropItem : public Item
{
public:
    PropItem();
    void appendData(QString in);
};

class BookItem : public Item
{

public:
    BookItem (Book book);
    Book book;
};

class BatchItem: public Item
{

public:
    BatchItem(int batchID);
    void appendBook(BookItem *book);
    void removeBook(BookItem *book);
    int id() const;
private:
    int batchID;
};

class Model : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit Model(RootItem *rootItem, QObject *parent = nullptr);
    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void setRootItem(RootItem *rootItem);
protected:
    RootItem *rootItem;
};

class BookModel: public Model
{
    Q_OBJECT

public:
    BookModel(RootItem *rootItem, QString type, int nextbookID, QObject *parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
public slots:
    void onItemSelectionDuo(const QModelIndex &index1 = QModelIndex(), const QModelIndex &index2 = QModelIndex());
    void onItemSelectionSingle(const QModelIndex &index1 = QModelIndex());
private:
    QString type;
    int nextBookID;
signals:
    void done(int r);
    void bookLoad(Book &book);
    void bookMove(Book &book, int newbatch);
};

class PropsModel : public Model
{
public:
    PropsModel(RootItem *rootItem, QList<QString> header, QObject *parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
private:
    QList<QString> header;
};

#endif // MODELS_H
