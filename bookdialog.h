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

class BookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookDialog(std::vector<bookdata::Book> &books, QWidget *parent = nullptr);
    ~BookDialog();
    BasicModel *getModel() const;
private:
    Ui::BookDialog *ui;
    QSortFilterProxyModel *sortModel;
    void setupModel(std::vector<bookdata::Book> &books);
signals:
    void loadBook(bookdata::Book &incomingBook);
};

#endif // BOOKDIALOG_H
