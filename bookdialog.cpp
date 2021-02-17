#include "models.h"
#include "bookdialog.h"
#include "mainwindow.h"
#include "./ui_bookdialog.h"

#include <json.hpp>

#include <QDir>
#include <QUrl>
#include <QHeaderView>
#include <QDebug>

using json = nlohmann::json;

BookDialog::BookDialog(QWidget *parent, const QString bookDirectory, const QString type) : QDialog(parent), ui(new Ui::BookDialog), type(type)
{
    ui->setupUi(this);

    if(type == "edit")
    {
        this->setWindowTitle("Edit Book");
        this->ui->infoLabel->setText("Please select a book to be edited");
        this->setWindowTitle("Edit Entry");
    }
    else if(type == "new")
    {
        this->setWindowTitle("New Book");
        this->ui->infoLabel->setText("Please select a batch to create a new book in");
        this->setWindowTitle("New Entry");
    }
    else if(type == "move")
    {
        this->setWindowTitle("Move Book");
        this->ui->infoLabel->setText("Please select a batch to move the selected book into");
        this->setWindowTitle("Move Entry");
    }

    setupModel(bookDirectory);

    auto header = new QHeaderView(Qt::Horizontal);
    header->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    ui->treeView->setHeader(header);
    auto selectionModel = new BookSelectionModel(2);

    selectionModel->setModel(model);
    ui->treeView->setSelectionModel(selectionModel);

    if(type == "edit" || type == "new")
    {
        ui->treeView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
        connect(this->ui->treeView, &QAbstractItemView::doubleClicked, model, &BookModel::onItemSelectionSingle);
    }
    else if(type == "move")
    {
        ui->treeView->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);
        connect(selectionModel, &BookSelectionModel::selectionCurrent, model, &BookModel::onItemSelectionDuo);
    }

    connect(model, &BookModel::done, this, &QDialog::done);

}

BookDialog::~BookDialog()
{
    delete ui;
    delete model;
}

BookModel *BookDialog::getModel() const
{
    return this->model;
}

void BookDialog::setupModel(QDir bookDirectory)
{
    QStringList files = bookDirectory.entryList(QStringList() << "*.json", QDir::Files);

    json bookObj;
    Book indexedBook;
    BookItem *bookItem;

    RootItem *rootItem = new RootItem(5); //our root item in our book model
    BatchItem *batchItem;

    int batchID;
    int bookID;
    int maxBookID = 0;
    int maxBatchID = 0;

    /*  First, parse through book JSONs to find valid books and load them into "indexedBook"
     *  Then add the book to the proper batch by either 1) creating a new batch or 2) adding to an existing batch, in the "rootItem"
     *  Repeat this with all the books until finished
     *  Then instantate a BookModel and pass it the ref to the rootItem.
     */

    for(QString filename : files)
    {

        json bookObj = MainWindow::readFile(bookDirectory.path(), filename);
        if(!bookObj.is_null())
        {
            indexedBook = bookObj;

            if(indexedBook.isValid())
            {
                batchID = indexedBook.batchID;
                bookID = indexedBook.bookID;

                if(bookID == 0) qDebug() << indexedBook.status;

                if(batchID > maxBatchID) maxBatchID = batchID;
                if(bookID > maxBookID) maxBookID = bookID;

                bookItem = new BookItem(indexedBook);
                batchItem = static_cast<BatchItem*>(rootItem->child(batchID));

                if(batchItem == nullptr)
                {
                    //make the batch with "batchID"
                    batchItem = new BatchItem(batchID);
                    rootItem->appendItem(batchItem);
                }

                batchItem->appendBook(bookItem);
            }
            else
            {
                qDebug() << QString("The json \"%1\" is invalid!").arg(filename);
            }
        }
        else
        {
            qDebug() << QString("json is null, because \"%1\" could not be opened!").arg(filename);
        }
    }

    if(type == "new" || type == "move")
    {
        rootItem->appendItem(new BatchItem(maxBatchID + 1));
    }

    model = new BookModel(rootItem,type, maxBookID + 1);
    ui->treeView->setModel(model);
}

BookSelectionModel::BookSelectionModel(int number): QItemSelectionModel(), number(number)
{

}

void BookSelectionModel::setCurrentIndex(const QModelIndex &index, QItemSelectionModel::SelectionFlags command)
{
    if(command == QItemSelectionModel::Rows || command == QItemSelectionModel::Columns)
        return;

    QModelIndexList list = this->selectedIndexes();

    if(command == QItemSelectionModel::Select || command == QItemSelectionModel::NoUpdate)
    {
        if(list.size() == number - 1)
        {
            QModelIndex oldIndex = list.first();
            emit selectionCurrent(index, oldIndex);
            QItemSelectionModel::select(oldIndex, QItemSelectionModel::Deselect); //if its not the right one (the model doesn't emit accept), deselect it.
        }
    }
}
