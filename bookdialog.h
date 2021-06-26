#ifndef BOOKDIALOG_H
#define BOOKDIALOG_H

#include "book.h"
#include "models.h"

#include <nlohmann/json.hpp>
#include <QDialog>
#include <QDir>
#include <QSortFilterProxyModel>
#include <QAbstractButton>

namespace Ui {
class BookDialog;
}

class BookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookDialog(std::vector<bookdata::Book*> &books, QWidget *parent = nullptr);
    ~BookDialog();
    BasicModel *getModel() const;
private:
    Ui::BookDialog *ui;
    QSortFilterProxyModel *sortModel;
    void setupModel(std::vector<bookdata::Book*> &books);
private slots:
    void onDoubleClicked(const QModelIndex &index);
    void onDelete();
signals:
    void loadBook(const int &incomingID);
    void deleteBooks(std::vector<int> books);
    void reloadBooks();
};

#endif // BOOKDIALOG_H
