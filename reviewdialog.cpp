#include <QDebug>

#include "reviewdialog.h"
#include "ui_reviewdialog.h"
#include "book.h"

using namespace bookdata;

ReviewDialog::ReviewDialog(const Book &newBook, const Book &oldBook, QWidget *parent) : QDialog(parent), ui(new Ui::ReviewDialog), model(new BasicModel(3))
{
    ui->setupUi(this);
    setupModel(newBook, oldBook);

    //Handle the buttons individually
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &ReviewDialog::handleButtons);
}

ReviewDialog::~ReviewDialog()
{
    delete ui;
    delete model;
}

void ReviewDialog::setupModel(const Book &newBook, const Book &oldBook)
{
    model->reset();
    model->setHeaderData({tr("Property"), tr("New Value"), tr("Old Value")});

    if(!Book::isValid(oldBook))
    {
        //only for new books...
        PropItem *item = new PropItem("New Entry", {"-", "-"});
        model->addItem(item);
    }
    else if(!(newBook == oldBook))
    {

        //List all props except for bookID, lastEdited, and creation.

        if(newBook.bookType != oldBook.bookType)
        {
            PropItem *item = new PropItem("Book Type", { QString::fromStdString(getString(newBook.bookType)), QString::fromStdString(getString(oldBook.bookType)) });
            model->addItem(item);
        }

        if(newBook.status != oldBook.status)
        {
            PropItem *item = new PropItem("Status", { QString::fromStdString(getString(newBook.status)), QString::fromStdString(getString(oldBook.status)) });
            model->addItem(item);
        }

        if(!(newBook.coverDim == oldBook.coverDim))
        {
            PropItem *item = new PropItem("Cover Dimensions", { QString::fromStdString(getString(newBook.coverDim)), QString::fromStdString(getString(oldBook.coverDim)) });
            model->addItem(item);
        }

        if(!(newBook.pageDim == oldBook.pageDim))
        {
            PropItem *item = new PropItem("Page Dimensions", { QString::fromStdString(getString(newBook.pageDim)), QString::fromStdString(getString(oldBook.pageDim)) });
            model->addItem(item);
        }

        if(newBook.signitures != oldBook.signitures)
        {
            PropItem *item = new PropItem("Signitures", { newBook.signitures, oldBook.signitures });
            model->addItem(item);
        }

        if(newBook.pagesPerSigniture != oldBook.pagesPerSigniture)
        {
            PropItem *item = new PropItem("Each Signitures", { newBook.pagesPerSigniture, oldBook.pagesPerSigniture });
            model->addItem(item);
        }

        if(newBook.weight != oldBook.weight)
        {
            PropItem *item = new PropItem("Weight", { newBook.weight, oldBook.weight });
            model->addItem(item);
        }

        if(newBook.spine != oldBook.spine)
        {
            PropItem *item = new PropItem("Spine", { newBook.spine, oldBook.spine });
            model->addItem(item);
        }

        if(newBook.costExtra != oldBook.costExtra)
        {
            PropItem *item = new PropItem("Extra Cost", { newBook.costExtra, oldBook.costExtra });
            model->addItem(item);
        }

        if(newBook.coverMaterial != oldBook.coverMaterial)
        {
            PropItem *item = new PropItem("Cover Material", { QString::fromStdString(newBook.coverMaterial), QString::fromStdString(oldBook.coverMaterial) });
            model->addItem(item);
        }

        if(newBook.pageMaterial != oldBook.pageMaterial)
        {
            PropItem *item = new PropItem("Page Material", { QString::fromStdString(newBook.pageMaterial), QString::fromStdString(oldBook.pageMaterial) });
            model->addItem(item);
        }

        if(newBook.box != oldBook.box)
        {
            PropItem *item = new PropItem("Box", { QString::fromStdString(newBook.box), QString::fromStdString(oldBook.box) });
            model->addItem(item);
        }

        if(newBook.endpageColor != oldBook.endpageColor)
        {
            PropItem *item = new PropItem("Endpage Color", { QString::fromStdString(newBook.endpageColor), QString::fromStdString(oldBook.endpageColor) });
            model->addItem(item);
        }

        if(newBook.threadColor != oldBook.threadColor)
        {
            PropItem *item = new PropItem("Spine/Ribbon/Thread Color", { QString::fromStdString(newBook.threadColor), QString::fromStdString(oldBook.threadColor) });
            model->addItem(item);
        }

        if(newBook.section != oldBook.section)
        {
            PropItem *item = new PropItem("Section", { QString::fromStdString(newBook.section), QString::fromStdString(oldBook.section) });
            model->addItem(item);
        }

        if(newBook.extra != oldBook.extra)
        {
            PropItem *item = new PropItem("Extra", { QString::fromStdString(newBook.extra), QString::fromStdString(oldBook.extra) });
            model->addItem(item);
        }
    }

    ui->treeView->setModel(model);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
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
