#ifndef BOOKMODEL_H
#define BOOKMODEL_H

#include "book.h"
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QDir>

class Item {
public:
    Item();
    ~Item ();
    Item* parent() const;
    void setParent(Item*);
    int row() const;
    Item* child(int row) const;
    void setChild(int row, Item*);
    bool hasChild(int row) const;
    int childCount() const;
    QString data(int column);
    int dataSize();
    QString type() const;

protected:
    void appendChild(Item *item);
    void removeChild(Item *item);
    void setData(int column, QString data);
    QString itemType;

private:
    QList<Item*> childList;
    QList<QString> dataList;
    Item* parentItem;
};

class BookItem: public Item
{

public:
    BookItem (Book &book);
    Book book;
};

class BatchItem: public Item
{

public:
    BatchItem(int batchID);
    void appendBook(BookItem *book);
    void setBook(int row, BookItem* book);
    void removeBook(BookItem *book);
    int id() const;
private:
    int batchID;
};

class RootItem: public Item
{
public:
    RootItem();
    void appendBatch(BatchItem *batch);
    void removeBatch(BatchItem *batch);
};

class BookModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit BookModel(QMultiMap<int, Book> data, QString type, int nextBookID, QObject *parent = nullptr);
    ~BookModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void populateModel(QMultiMap<int, Book> data);
    RootItem *rootItem;
public slots:
    void onDoubleClicked(const QModelIndex &index);
private:
    QString type;
    int nextBookID;

signals:
    void done(int r);
    void bookLoad(Book &book);
    void bookMove(Book &book, int batchID);
};

#endif // BOOKMODEL_H
