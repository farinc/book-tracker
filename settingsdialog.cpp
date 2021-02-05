#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QPushButton>
#include <QFileDialog>
#include <QDialogButtonBox>

#include <fstream>
#include <iomanip>
#include <json.hpp>

#include <QDebug>

#include "costitemsmodel.h"

using json = nlohmann::json;

SettingsDialog::SettingsDialog(Settings settings, QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog), tempSettings(settings)
{
    ui->setupUi(this);
    this->setWindowTitle("Settings");
    model = new CostItemsModel(this);
    ui->lineEditBookDirectory->setText(QString::fromStdString(settings.bookDirectory));

    auto header = new QHeaderView(Qt::Horizontal);
    header->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    ui->treeView->setHeader(header);

    auto delegate = new SpinBoxDelegate();
    ui->treeView->setItemDelegateForColumn(1, delegate);

    connect(ui->pushButtonBrowse, &QPushButton::clicked, this, &SettingsDialog::onBrowse);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &SettingsDialog::handleButtons);

    setupModel();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
    delete model;
}

Settings SettingsDialog::settings() const
{
    return tempSettings;
}

void SettingsDialog::onBrowse()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), QDir::currentPath());
    ui->lineEditBookDirectory->setText(directory);
    tempDir = tempSettings.bookDirectory;
    tempSettings.bookDirectory = directory.toStdString();
}

void SettingsDialog::handleButtons(QAbstractButton *btn)
{
    auto sb = ui->buttonBox->standardButton(btn);
    if (sb == QDialogButtonBox::Reset)
    {
        if(!tempDir.empty())
        {
            tempSettings.bookDirectory = tempDir;
        }

        tempSettings.bookconstants = CostConstants();
        ui->lineEditBookDirectory->setText(QString::fromStdString(tempSettings.bookDirectory));
        ui->treeView->reset();
        ui->treeView->model()->revert();
        setupModel();
    }
    else if(sb == QDialogButtonBox::RestoreDefaults)
    {
        this->tempSettings = Settings();
        ui->lineEditBookDirectory->setText(QString::fromStdString(tempSettings.bookDirectory));
        setupModel();
    }
}

void SettingsDialog::setupModel()
{
    CostConstants &constants = tempSettings.bookconstants;

    model->reset();

    CostItem *item1 = new CostItem(tr("Padding width for board"), &constants.paddingWidthBoard);
    model->addItem(item1);

    CostItem *item2 = new CostItem(tr("Padding height for board"), &constants.paddingHeightBoard);
    model->addItem(item2);

    CostItem *item3 = new CostItem(tr("Padding spine for long traditional bound"), &constants.paddingSpineLongTrad);
    model->addItem(item3);

    CostItem *item4 = new CostItem(tr("Padding spine for quarter bound"), &constants.paddingSpineQuarter);
    model->addItem(item4);

    CostItem *item5 = new CostItem(tr("Padding spine for super"), &constants.paddingSpineForSuper);
    model->addItem(item5);

    CostItem *item6 = new CostItem(tr("Board price per square inch"), &constants.sqInchBoardPrice);
    model->addItem(item6);

    CostItem *item7 = new CostItem(tr("Price per sheet"), &constants.sheetPrice);
    model->addItem(item7);

    CostItem *item8 = new CostItem(tr("Cloth price per inch"), &constants.sqInchClothPrice);
    model->addItem(item8);

    CostItem *item9 = new CostItem(tr("Thread length price per inch"), &constants.threadLengthPrice);
    model->addItem(item9);

    CostItem *item10 = new CostItem(tr("Price of headband per inch"), &constants.headbandPrice);
    model->addItem(item10);

    CostItem *item11 = new CostItem(tr("Price of super per inch"), &constants.superPrice);
    model->addItem(item11);

    CostItem *item12 = new CostItem(tr("Price of ribbon per inch"), &constants.ribbonPrice);
    model->addItem(item12);

    CostItem *item13 = new CostItem(tr("Price of PVA glue"), &constants.pvaCost);
    model->addItem(item13);

    CostItem *item14 = new CostItem(tr("Price of endpages"), &constants.endpageCost);
    model->addItem(item14);

    this->ui->treeView->setModel(model);
}


