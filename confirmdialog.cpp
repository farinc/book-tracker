#include "confirmdialog.h"
#include "ui_confirmdialog.h"

ConfirmDialog::ConfirmDialog(QString str, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmDialog)
{
    ui->setupUi(this);
    ui->msglable->setText(str);
}

ConfirmDialog::~ConfirmDialog()
{
    delete ui;
}
