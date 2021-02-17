#include <QDebug>

#include "savedialog.h"
#include "ui_savedialog.h"
#include "book.h"

SaveDialog::SaveDialog(Book newBook, Book oldBook, QWidget *parent) : QDialog(parent), ui(new Ui::ChangeDialog), model(nullptr)
{
    ui->setupUi(this);
    setupModel(newBook, oldBook);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &SaveDialog::handleButtons);
}

SaveDialog::~SaveDialog()
{
    delete ui;
    delete model;
}

void SaveDialog::setupModel(Book newBook, Book oldBook)
{
    if(!newBook.isValid())
        return;

    RootItem *root = new RootItem(3);

    if(!oldBook.isValid())
    {
        //only truely new books...
        PropItem *item = new PropItem();
        item->appendData("New Entry");
        item->appendData("-");
        item->appendData("-");
        root->appendItem(item);
    }
    else if(!(newBook == oldBook))
    {

        //List all props except for bookID, lastEdited, and creation.
        if(newBook.batchID != oldBook.batchID)
        {
            PropItem *item = new PropItem();
            item->appendData("Batch ID");
            item->appendData(QString::number(newBook.batchID));
            item->appendData(QString::number(oldBook.batchID));
            root->appendItem(item);
        }

        if(newBook.bookType != oldBook.bookType)
        {
            PropItem *item = new PropItem();
            item->appendData("Book Type");
            item->appendData(QString::fromStdString(getString(newBook.bookType)));
            item->appendData(QString::fromStdString(getString(oldBook.bookType)));
            root->appendItem(item);
        }

        if(newBook.status != oldBook.status)
        {
            PropItem *item = new PropItem();
            item->appendData("Status");
            item->appendData(QString::fromStdString(getString(newBook.status)));
            item->appendData(QString::fromStdString(getString(oldBook.status)));
            root->appendItem(item);
        }

        if(!(newBook.coverDim == oldBook.coverDim))
        {
            PropItem *item = new PropItem();
            item->appendData("Cover Dimensions");
            item->appendData(QString::fromStdString(getString(newBook.coverDim)));
            item->appendData(QString::fromStdString(getString(oldBook.coverDim)));
            root->appendItem(item);
        }

        if(!(newBook.pageDim == oldBook.pageDim))
        {
            PropItem *item = new PropItem();
            item->appendData("Page Dimensions");
            item->appendData(QString::fromStdString(getString(newBook.pageDim)));
            item->appendData(QString::fromStdString(getString(oldBook.pageDim)));
            root->appendItem(item);
        }

        if(newBook.signitures != oldBook.signitures)
        {
            PropItem *item = new PropItem();
            item->appendData("Signitures");
            item->appendData(QString::number(newBook.signitures));
            item->appendData(QString::number(oldBook.signitures));
            root->appendItem(item);
        }

        if(newBook.pagesPerSigniture != oldBook.pagesPerSigniture)
        {
            PropItem *item = new PropItem();
            item->appendData("Each Signitures");
            item->appendData(QString::number(newBook.pagesPerSigniture));
            item->appendData(QString::number(oldBook.pagesPerSigniture));
            root->appendItem(item);
        }

        if(newBook.weight != oldBook.weight)
        {
            PropItem *item = new PropItem();
            item->appendData("Weight");
            item->appendData(QString::number(newBook.weight));
            item->appendData(QString::number(oldBook.weight));
            root->appendItem(item);
        }

        if(newBook.spine != oldBook.spine)
        {
            PropItem *item = new PropItem();
            item->appendData("Spine");
            item->appendData(QString::number(newBook.spine));
            item->appendData(QString::number(oldBook.spine));
            root->appendItem(item);
        }

        if(newBook.costExtra != oldBook.costExtra)
        {
            PropItem *item = new PropItem();
            item->appendData("Extra Cost");
            item->appendData(QString::number(newBook.costExtra));
            item->appendData(QString::number(oldBook.costExtra));
            root->appendItem(item);
        }

        if(newBook.coverMaterial != oldBook.coverMaterial)
        {
            PropItem *item = new PropItem();
            item->appendData("Cover Material");

            item->appendData(QString::fromStdString(newBook.coverMaterial));
            item->appendData(QString::fromStdString(oldBook.coverMaterial));
            root->appendItem(item);
        }

        if(newBook.pageMaterial != oldBook.pageMaterial)
        {
            PropItem *item = new PropItem();
            item->appendData("Page Material");

            item->appendData(QString::fromStdString(newBook.pageMaterial));
            item->appendData(QString::fromStdString(oldBook.pageMaterial));
            root->appendItem(item);
        }

        if(newBook.box != oldBook.box)
        {
            PropItem *item = new PropItem();
            item->appendData("Box");

            item->appendData(QString::fromStdString(newBook.box));
            item->appendData(QString::fromStdString(oldBook.box));
            root->appendItem(item);
        }

        if(newBook.endpageColor != oldBook.endpageColor)
        {
            PropItem *item = new PropItem();
            item->appendData("Endpage Color");

            item->appendData(QString::fromStdString(newBook.endpageColor));
            item->appendData(QString::fromStdString(oldBook.endpageColor));
            root->appendItem(item);
        }

        if(newBook.threadColor != oldBook.threadColor)
        {
            PropItem *item = new PropItem();
            item->appendData("Spine/Ribbon/Thread Color");

            item->appendData(QString::fromStdString(newBook.threadColor));
            item->appendData(QString::fromStdString(oldBook.threadColor));
            root->appendItem(item);
        }

        if(newBook.section != oldBook.section)
        {
            PropItem *item = new PropItem();
            item->appendData("Section");

            item->appendData(QString::fromStdString(newBook.section));
            item->appendData(QString::fromStdString(oldBook.section));
            root->appendItem(item);
        }

        if(newBook.extra != oldBook.extra)
        {
            PropItem *item = new PropItem();
            item->appendData("Extra");

            item->appendData(QString::fromStdString(newBook.extra));
            item->appendData(QString::fromStdString(oldBook.extra));
            root->appendItem(item);
        }
    }

    model = new PropsModel(root, (QList<QString>() << "Property" << "New Value" << "Old Value"));
    ui->treeView->setModel(model);
}

void SaveDialog::handleButtons(QAbstractButton *btn)
{
    using buttons = QDialogButtonBox::StandardButton;
    auto type = ui->buttonBox->standardButton(btn);
    if (type == buttons::SaveAll)
    {
        emit save();
    }
    else if (type == buttons::Discard)
    {
        emit discard();
        reject();
    }
}
