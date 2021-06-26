#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmDialog;
}

class ConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDialog(QString str, QWidget *parent = nullptr);
    ~ConfirmDialog();

private:
    Ui::ConfirmDialog *ui;
};

#endif // CONFIRMDIALOG_H
