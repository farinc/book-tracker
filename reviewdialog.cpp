#include <QDebug>

#include "reviewdialog.h"
#include "ui_reviewdialog.h"
#include "uilogic.h"

using namespace bookdata;

ReviewDialog::ReviewDialog(const UiLogic *logic, QWidget *parent) : QDialog(parent), ui(new Ui::ReviewDialog), model(new BasicModel(3)), logic(logic)
{
    ui->setupUi(this);
    setupModel();
    //Handle the buttons individually
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &ReviewDialog::handleButtons);
}

ReviewDialog::~ReviewDialog()
{
    delete ui;
    delete model;
}

void ReviewDialog::setupModel()
{
    model->reset();
    model->setHeaderData({tr("Property"), tr("New Value"), tr("Old Value")});

    if(!Book::isValid(logic->oldBook))
    {
        //only for new books...
        PropItem *item = new PropItem("New Entry", {"-", "-"});
        model->addItem(item);
    }
    else if(!(logic->book == logic->oldBook))
    {

        //List all props except for bookID, lastEdited, and creation.

        if(logic->book.bookType != logic->oldBook.bookType)
        {
            PropItem *item = new PropItem("Book Type", { getString(logic->book.bookType), getString(logic->oldBook.bookType) });
            model->addItem(item);
        }

        if(logic->book.status != logic->oldBook.status)
        {
            PropItem *item = new PropItem("Status", { getString(logic->book.status), getString(logic->oldBook.status) });
            model->addItem(item);
        }

        if(!(logic->book.coverDim == logic->oldBook.coverDim))
        {
            PropItem *item = new PropItem("Cover Dimensions", { getString(logic->book.coverDim), getString(logic->oldBook.coverDim) });
            model->addItem(item);
        }

        if(!(logic->book.pageDim == logic->oldBook.pageDim))
        {
            PropItem *item = new PropItem("Page Dimensions", { getString(logic->book.pageDim), getString(logic->oldBook.pageDim) });
            model->addItem(item);
        }

        if(logic->book.signitures != logic->oldBook.signitures)
        {
            PropItem *item = new PropItem("Signitures", { logic->book.signitures, logic->oldBook.signitures });
            model->addItem(item);
        }

        if(logic->book.pagesPerSigniture != logic->oldBook.pagesPerSigniture)
        {
            PropItem *item = new PropItem("Each Signitures", { logic->book.pagesPerSigniture, logic->oldBook.pagesPerSigniture });
            model->addItem(item);
        }

        if(logic->book.weight != logic->oldBook.weight)
        {
            PropItem *item = new PropItem("Weight", { QString::number(logic->book.weight, 'f', 3).append(" oz"), QString::number(logic->oldBook.weight, 'f', 3).append(" oz") });
            model->addItem(item);
        }

        if(logic->book.spine != logic->oldBook.spine)
        {
            PropItem *item = new PropItem("Spine", { QString::number(logic->book.spine, 'f', 3).append(" in"), QString::number(logic->oldBook.spine, 'f', 3).append(" in") });
            model->addItem(item);
        }

        if(logic->book.costExtra != logic->oldBook.costExtra)
        {
            PropItem *item = new PropItem("Extra Cost", { QString::number(logic->book.costExtra, 'f', 2).prepend('$'), QString::number(logic->oldBook.costExtra, 'f', 2).prepend('$') });
            model->addItem(item);
        }

        if(logic->book.coverMaterial != logic->oldBook.coverMaterial)
        {
            PropItem *item = new PropItem("Cover Material", { QString::fromStdString(logic->book.coverMaterial), QString::fromStdString(logic->oldBook.coverMaterial) });
            model->addItem(item);
        }

        if(logic->book.pageMaterial != logic->oldBook.pageMaterial)
        {
            PropItem *item = new PropItem("Page Material", { QString::fromStdString(logic->book.pageMaterial), QString::fromStdString(logic->oldBook.pageMaterial) });
            model->addItem(item);
        }

        if(logic->book.box != logic->oldBook.box)
        {
            PropItem *item = new PropItem("Box", { QString::fromStdString(logic->book.box), QString::fromStdString(logic->oldBook.box) });
            model->addItem(item);
        }

        if(logic->book.endpageColor != logic->oldBook.endpageColor)
        {
            PropItem *item = new PropItem("Endpage Color", { QString::fromStdString(logic->book.endpageColor), QString::fromStdString(logic->oldBook.endpageColor) });
            model->addItem(item);
        }

        if(logic->book.threadColor != logic->oldBook.threadColor)
        {
            PropItem *item = new PropItem("Spine/Ribbon/Thread Color", { QString::fromStdString(logic->book.threadColor), QString::fromStdString(logic->oldBook.threadColor) });
            model->addItem(item);
        }

        if(logic->book.section != logic->oldBook.section)
        {
            PropItem *item = new PropItem("Section", { QString::fromStdString(logic->book.section), QString::fromStdString(logic->oldBook.section) });
            model->addItem(item);
        }

        if(logic->book.extra != logic->oldBook.extra)
        {
            PropItem *item = new PropItem("Extra", { QString::fromStdString(logic->book.extra), QString::fromStdString(logic->oldBook.extra) });
            model->addItem(item);
        }
    }

    ui->treeView->setModel(model);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
}

void ReviewDialog::handleButtons(QAbstractButton *btn)
{
    using buttons = QDialogButtonBox::StandardButton;
    auto type = ui->buttonBox->standardButton(btn);
    if (type == buttons::SaveAll)
    {
        emit save();
        accept();
    }
    else if (type == buttons::Discard)
    {
        emit discard();
        accept(); //Discarding should succeed, because its "OK" just with reverting the values of the active book
    }

    reject(); //implicitly, this is the "cancel" option
}
