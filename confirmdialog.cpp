#include "confirmdialog.h"
#include "ui_confirmdialog.h"

ConfirmDialog::ConfirmDialog(QString str, QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmDialog)
{
    ui->setupUi(this);
    ui->msglable->setText(str);
    this->setWindowTitle(title);
}

ConfirmDialog::~ConfirmDialog()
{
    delete ui;
}
