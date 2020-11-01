#ifndef BOOKDIALOG_H
#define BOOKDIALOG_H

#include <QDialog>

namespace Ui {
class BookDialog;
}

class BookDialog : public QDialog
{
    Q_OBJECT

public:
    BookDialog(QWidget* parent);
    ~BookDialog();

private:
    Ui::BookDialog *ui;
};

#endif // BOOKDIALOG_H
