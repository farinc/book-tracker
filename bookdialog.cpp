#include "bookdialog.h"
#include "./ui_bookdialog.h"
#include <QFile>
#include <QDir>
#include <QUrl>

#include "bookmodel.h"

BookDialog::BookDialog(QWidget *parent, QDir bookDirectory) :
    QDialog(parent),
    ui(new Ui::BookDialog)
{
    ui->setupUi(this);

    QList<Book*> books = QList<Book*>();
    QStringList files = bookDirectory.entryList(QStringList() << ".json", QDir::Files);
    QUrl indexedPath;
    Book indexedBook;
    json bookjson;
    for(QString filename : files){
        indexedPath = QUrl::fromLocalFile(bookDirectory.path() + filename);
        bookjson = Book::loadJson(indexedPath);
        indexedBook = bookjson;
        books.append(&indexedBook);
    }

    BookModel *model = new BookModel(files, bookDirectory, this);
    ui->treeView->setModel(model);
}

BookDialog::~BookDialog()
{
    delete ui;
}
