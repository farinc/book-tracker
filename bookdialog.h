#ifndef BOOKDIALOG_H
#define BOOKDIALOG_H

#include "book.h"

#include <QDialog>
#include <QDir>

namespace Ui {
class BookDialog;
}

class BookDialog : public QDialog
{
    Q_OBJECT

public:
    BookDialog(QWidget* parent, QDir bookDirectory);
    ~BookDialog();

private:
    Ui::BookDialog *ui;

signals:
    void onBooksLoaded(QList<Book*>);
};

#endif // BOOKDIALOG_H
