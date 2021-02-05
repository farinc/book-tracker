#ifndef COSTITEMSMODEL_H
#define COSTITEMSMODEL_H

#include <QAbstractItemModel>
#include <QStandardItem>
#include <QStyledItemDelegate>

class CostItem
{
public:
    CostItem(QString info, double *value);
    void setData(double value);
    QVariant data(int row) const;
private:
    QString info;
    double *value;
};

class CostItemsModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit CostItemsModel(QObject *parent = nullptr);
    ~CostItemsModel();
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    void reset();
    void addItem(CostItem *item);
private:
    QList<CostItem*> list;
};

class SpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    SpinBoxDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // COSTITEMSMODEL_H
