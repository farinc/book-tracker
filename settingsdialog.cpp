#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "uilogic.h"
#include "models.h"

#include <QPushButton>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QDebug>
#include <QApplication>


SettingsDialog::SettingsDialog(UiLogic *logic, QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog), logic(logic), copySettings(logic->settings), model(new QSortFilterProxyModel())
{
    ui->setupUi(this);

    ui->lineEditBookDirectory->setText(QString::fromStdString(copySettings.bookDirectory));
    ui->lineEditSettings->setText(QString::fromStdString(copySettings.configDirectory));

    if(copySettings.style == "dark")
    {
        ui->radioButton_dark->setChecked(true);
    }
    else if(copySettings.style == "light")
    {
        ui->radioButton_light->setChecked(true);
    }
    else if(copySettings.style == "system")
    {
        ui->radioButton_system->setChecked(true);
    }

    auto header = new QHeaderView(Qt::Horizontal);
    header->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    ui->treeView->setHeader(header);

    auto delegate = new SpinBoxDelegate();
    ui->treeView->setItemDelegateForColumn(1,delegate);

    setupModel();

    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &SettingsDialog::handleButtons);
    connect(this, &SettingsDialog::accepted, this, &SettingsDialog::onSetSettings);
    connect(ui->radioButton_dark, &QRadioButton::clicked, this, &SettingsDialog::onStyleSwitchDark);
    connect(ui->radioButton_light, &QRadioButton::clicked, this, &SettingsDialog::onStyleSwitchLight);
    connect(ui->radioButton_system, &QRadioButton::clicked, this, &SettingsDialog::onStyleSwitchSystem);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
    delete model;
}

void SettingsDialog::onBrowse()
{
    QString directory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), QDir::currentPath());
    ui->lineEditBookDirectory->setText(directory);
    copySettings.bookDirectory = directory.toStdString();
}

void SettingsDialog::handleButtons(QAbstractButton *btn)
{
    auto sb = ui->buttonBox->standardButton(btn);
    if (sb == QDialogButtonBox::Reset)
    {
        copySettings = logic->settings;
        setupModel();
    }
    else if(sb == QDialogButtonBox::RestoreDefaults)
    {
        copySettings = logic->loadDefaultSettings();
        setupModel();
    }
}

void SettingsDialog::onSetSettings()
{
    logic->settings = copySettings;
}

void SettingsDialog::onStyleChange(std::string style)
{
    logic->changeStyle(style);
    copySettings.style = style;
}

void SettingsDialog::onStyleSwitchDark()
{
    onStyleChange("dark");
}

void SettingsDialog::onStyleSwitchLight()
{
    onStyleChange("light");
}

void SettingsDialog::onStyleSwitchSystem()
{
    onStyleChange("system");
}

void SettingsDialog::setupModel()
{
    bookdata::CostConstants &constants = copySettings.bookconstants;
    BasicModel *dmodel = new BasicModel(3);

    dmodel->reset();
    dmodel->setHeaderData({tr("Property"), tr("Value"), tr("Unit")});

    BookConstantItem *item1 = new BookConstantItem(tr("Padding width for board"), constants.paddingWidthBoard, "in");
    dmodel->addItem(item1);

    BookConstantItem *item2 = new BookConstantItem(tr("Padding height for board"), constants.paddingHeightBoard, "in");
    dmodel->addItem(item2);

    BookConstantItem *item3 = new BookConstantItem(tr("Padding spine for long traditional bound"), constants.paddingSpineLongTrad, "in");
    dmodel->addItem(item3);

    BookConstantItem *item4 = new BookConstantItem(tr("Padding spine for quarter bound"), constants.paddingSpineQuarter, "in");
    dmodel->addItem(item4);

    BookConstantItem *item5 = new BookConstantItem(tr("Padding spine for super"), constants.paddingSpineForSuper, "in");
    dmodel->addItem(item5);

    BookConstantItem *item6 = new BookConstantItem(tr("Board price per square inch"), constants.sqInchBoardPrice, "$");
    dmodel->addItem(item6);

    BookConstantItem *item7 = new BookConstantItem(tr("Price per sheet"), constants.sheetPrice, "$");
    dmodel->addItem(item7);

    BookConstantItem *item8 = new BookConstantItem(tr("Cloth price per inch"), constants.sqInchClothPrice, "$");
    dmodel->addItem(item8);

    BookConstantItem *item9 = new BookConstantItem(tr("Thread length price per inch"), constants.threadLengthPrice, "$");
    dmodel->addItem(item9);

    BookConstantItem *item10 = new BookConstantItem(tr("Price of headband per inch"), constants.headbandPrice, "$");
    dmodel->addItem(item10);

    BookConstantItem *item11 = new BookConstantItem(tr("Price of super per inch"), constants.superPrice, "$");
    dmodel->addItem(item11);

    BookConstantItem *item12 = new BookConstantItem(tr("Price of ribbon per inch"), constants.ribbonPrice, "$");
    dmodel->addItem(item12);

    BookConstantItem *item13 = new BookConstantItem(tr("Price of PVA glue"), constants.pvaCost, "$");
    dmodel->addItem(item13);

    BookConstantItem *item14 = new BookConstantItem(tr("Price of endpages"), constants.endpageCost, "$");
    dmodel->addItem(item14);

    model->setSourceModel(dmodel);
    this->ui->treeView->setModel(model);
}

SpinBoxDelegate::SpinBoxDelegate(QObject *parent): QStyledItemDelegate(parent)
{}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(0);
    editor->setMaximum(100);
    editor->setSingleStep(0.1);

    return editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   double value = index.model()->data(index, Qt::EditRole).toDouble();
   QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(editor);
   spinBox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(editor);
    spinBox->interpretText();
    double value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}


