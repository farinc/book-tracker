#ifndef BOOKMODEL_H
#define BOOKMODEL_H

#include "book.h"
#include <QAbstractItemModel>
#include <QDir>

class Item {
    friend class BookModel;
public:
    Item ();
    ~Item ();
    Item* child(int row) const;
    int childCount() const;
    Item* parent() const;
    int row() const;
    int dataSize() const;
    QString data(int column) const;

protected:
    QList<Item*> childList;
    QList<QString> dataList;
    Item* parentItem;
};

class BookItem: protected Item
{

public:
    BookItem (Book book);
};

class BatchItem: protected Item
{

private:
    time_t lastEdit;
public:
    BatchItem ();
    void appendBook(int bookID, BookItem);
    void removeBook(int bookID);
};

class BookModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit BookModel(QStringList data, QDir directory, QObject *parent = nullptr);
    ~BookModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Item *rootItem;
    void update(int batchID);
};

#endif // BOOKMODEL_H
