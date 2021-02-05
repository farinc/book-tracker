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
    BookDialog(QWidget* parent, QString bookDirectory, QString type);
    ~BookDialog();

private:
    Ui::BookDialog *ui;
    void setupModel(QWidget *parent, QDir bookDirectory, QString type);

signals:
    void bookEdit(Book book);
};

#endif // BOOKDIALOG_H
