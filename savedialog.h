#ifndef CHANGEDIALOG_H
#define CHANGEDIALOG_H

#include <QDialog>
#include <QAbstractButton>

#include "models.h"

namespace Ui {
class ChangeDialog;
}

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDialog(Book newBook, Book oldBook, QWidget *parent = nullptr);
    ~SaveDialog();
    void setupModel(Book newBook, Book oldBook);

public slots:
    void handleButtons(QAbstractButton *btn);

signals:
    void save();
    void discard();

private:
    Ui::ChangeDialog *ui;
    PropsModel *model;
};

#endif // CHANGEDIALOG_H
