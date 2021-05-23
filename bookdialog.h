#ifndef BOOKDIALOG_H
#define BOOKDIALOG_H

#include "book.h"
#include "models.h"

#include <nlohmann/json.hpp>
#include <QDialog>
#include <QDir>
#include <QSortFilterProxyModel>

namespace Ui {
class BookDialog;
}

class BookSortProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    BookSortProxyModel();

};

class BookSelectionModel : public QItemSelectionModel
{
    Q_OBJECT
public:
    BookSelectionModel(int number);
private:
    int number;
public slots:
    void setCurrentIndex(const QModelIndex &index, QItemSelectionModel::SelectionFlags command) override;
signals:
    void selectionCurrent(const QModelIndex &index1, const QModelIndex &index2);
};

class BookDialog : public QDialog
{
    Q_OBJECT

public:
    BookDialog(QWidget* parent, const QString bookDirectory, const QString type);
    ~BookDialog();
    BookModel *getModel() const;
private:
    Ui::BookDialog *ui;
    BookModel *model;
    QSortFilterProxyModel *sortModel;
    QString type;

    void setupModel(QDir bookDirectory);

signals:
    void bookEdit(Book book);
};

#endif // BOOKDIALOG_H
