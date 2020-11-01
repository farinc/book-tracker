#include "bookdialog.h"
#include "./ui_bookdialog.h"
#include <QDebug>

BookDialog::BookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookDialog)
{
    ui->setupUi(this);
}

BookDialog::~BookDialog()
{
    delete ui;
}
