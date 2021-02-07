#ifndef BOOKPROPSMODEL_H
#define BOOKPROPSMODEL_H

#include <QAbstractItemModel>

class PropItem
{
    friend class BookPropsModel;
public:
    PropItem(QString title, QVariant data);
private:
    QString title;
    QVariant data;
};

class BookPropsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit BookPropsModel(QObject *parent = nullptr);
    ~BookPropsModel();
    // Basic functionality:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void addItem(PropItem *item);
    void reset();

private:
    QList<PropItem*> list;
};

#endif // BOOKPROPSMODEL_H
