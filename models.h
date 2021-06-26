#ifndef MODELS_H
#define MODELS_H

#include "book.h"
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>

class Item {
public:
    virtual ~Item() {};
    /**
     * @brief data Reterive data from the item.
     * @param column The column in question, this is maped 1-to-1 to veiw columns (if they exist)
     * @param role The role of the data
     * @return The data associated with the specified column and its role in the view
     */
    virtual QVariant data(int column, int role = Qt::DisplayRole) const {return QVariant();};
    /**
     * @brief flags Determines what traits this item has. This includes if the item is editable at different columns, readonly, wheather drag-n-drop is enabled, or a combination.
     * @param column The column selector.
     * @return The Qt ItemFlags traits of this item
     */
    virtual Qt::ItemFlags flags(int column) const {return Qt::ItemIsEnabled;};
    /**
     * @brief setData Sets the data in this item. Since this implies that the role is Qt::EditRole, be sure to reflect this in the flags method
     * @param column The column selector to where this data is being set.
     * @param data The imcoming data
     * @return If the data could be set, return true and otherwise false
     */
    virtual bool setData(int column, const QVariant &data) {return false;};

    void hello();
};

/**
 * @brief The BookItem class A item meant to display and edit (in certain cases only) book instances
 */
class BookItem: public Item {
public:
    BookItem (bookdata::Book &book);
    QVariant data(int column, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(int column) const override;
private:
    bookdata::Book &book;
};

/**
 * @brief The BookConstantItem class A item meant to display the book constants used in calculations. Values are editable for the constant
 */
class BookConstantItem: public Item {
public:
    BookConstantItem (const QString descritpion, double &constant);
    QVariant data(int column, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(int column) const override;
    bool setData(int column, const QVariant &data) override;
private:
    const QString descritpion;
    double &constant;
};

/**
 * @brief The PropItem class A basic and generitic item used for displaying data
 */
class PropItem: public Item {
public:
    PropItem (const QString descritpion, const std::initializer_list<QVariant> values);
    QVariant data(int column, int role = Qt::DisplayRole) const override;
private:
    const QString descritpion;
    const std::vector<QVariant> values;
};

class BasicModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit BasicModel(int columns, QWidget *parent = nullptr);
    ~BasicModel();
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    void setHeaderData(const std::initializer_list<QString> &list);
    void addItem(Item *item);
private:
    const int columns;
    std::vector<QString> header_strings;
    std::vector<Item*> items;
public slots:
    void reset();
};

#endif // MODELS_H
