#include <QDoubleSpinBox>

#include <QDebug>

#include "costitemsmodel.h"
#include "book.h"


CostItem::CostItem(QString info, double *value): value(value)
{
    this->info = info;
}

void CostItem::setData(double value)
{
    *this->value = value;
}

QVariant CostItem::data(int row) const
{
    switch (row)
    {
        case 0: return this->info;
        case 1: return *this->value;
        default: return QVariant();
    }
}

//Cost multipler model

CostItemsModel::CostItemsModel(QObject *parent): QAbstractItemModel(parent)
{
    list = QList<CostItem*>();
}

CostItemsModel::~CostItemsModel()
{
    qDeleteAll(list);
}

QVariant CostItemsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole)
    {
        if (section == 0)
        {
            return tr("Description");
        }
        if (section == 1)
        {
            return tr("Values");
        }
    }
    return QVariant();
}

QModelIndex CostItemsModel::index(int row, int column, const QModelIndex &parent) const
{
    CostItem *item = list.at(row);
    return createIndex(row, column, item);
}

QModelIndex CostItemsModel::parent(const QModelIndex &index) const
{
    return QModelIndex(); //There are no other parents...
}

int CostItemsModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return list.size();
}

int CostItemsModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant CostItemsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        CostItem *item = static_cast<CostItem*>(index.internalPointer());
        return item->data(index.column());
    }
    return QVariant();
}

bool CostItemsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        CostItem *item = static_cast<CostItem*>(index.internalPointer());
        item->setData(value.toDouble());
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags CostItemsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return index.column() > 0 ? Qt::ItemIsEditable | Qt::ItemIsEnabled : Qt::ItemIsEnabled;
}

void CostItemsModel::reset()
{
    beginResetModel();
    qDeleteAll(list.begin(), list.end());
    list.clear();
    endResetModel();
}

void CostItemsModel::addItem(CostItem *item)
{
    list.append(item);
}

SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
{

}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(0);
    editor->setMaximum(100);
    editor->setSingleStep(0.1);

    return editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   double value = index.model()->data(index, Qt::EditRole).toDouble();
   QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(editor);
   spinBox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
