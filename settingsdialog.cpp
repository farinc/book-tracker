#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QPushButton>
#include <QFileDialog>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(ui->pushButtonBrowse, &QPushButton::clicked, this, &SettingsDialog::onBrowse);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::onBrowse()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Find Files"), QDir::currentPath());
    ui->lineEditBookDirectory->setText(directory);

}
