#include "bookdialog.h"
#include "mainwindow.h"
#include "./ui_bookdialog.h"
#include <QDir>
#include <QUrl>
#include <QHeaderView>

#include <QDebug>

#include "bookmodel.h"

BookDialog::BookDialog(QWidget *parent, QString bookDirectory, QString type) :
    QDialog(parent),
    ui(new Ui::BookDialog)
{
    ui->setupUi(this);

    if(type == "edit")
    {
        this->ui->infoLabel->setText("Please select a book to be edited");
        this->setWindowTitle("Edit Entry");
    }
    else if(type == "new")
    {
        this->ui->infoLabel->setText("Please select a batch to create a new book in");
        this->setWindowTitle("New Entry");
    }

    this->setupModel(parent, bookDirectory, type);
    auto header = new QHeaderView(Qt::Horizontal);
    header->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    ui->treeView->setHeader(header);
}

BookDialog::~BookDialog()
{
    delete ui;
}

void BookDialog::setupModel(QWidget *parent, QDir bookDirectory, QString type)
{
    QMultiMap<int, Book> books = QMultiMap<int, Book>();
    QStringList files = bookDirectory.entryList(QStringList() << "*.json", QDir::Files);
    QString indexedPath;

    int maxBookID = 0;

    for(QString filename : files)
    {
        indexedPath = bookDirectory.path() + QDir::separator() + filename;
        Book indexedBook = Book::loadBook(indexedPath.toStdString());
        if (maxBookID < indexedBook.bookID)
            maxBookID = indexedBook.bookID;
        books.insert(indexedBook.batchID, indexedBook);
    }

    BookModel *model = new BookModel(books, type, ++maxBookID, this);
    ui->treeView->setModel(model);
    if (type == "edit" || type == "new")
    {
        ui->treeView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    }
    else if(type == "move")
    {
        ui->treeView->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    }

    connect(this->ui->treeView, &QAbstractItemView::doubleClicked, model, &BookModel::onDoubleClicked);
    connect(model, &BookModel::bookLoad, static_cast<MainWindow*>(parent), &MainWindow::onBookEdit);
    connect(model, &BookModel::done, this, &QDialog::done);
}
