#include "bookdialog.h"
#include "./ui_bookdialog.h"
#include <QDir>
#include <QUrl>

#include <QDebug>

#include "bookmodel.h"

BookDialog::BookDialog(QWidget *parent, QDir bookDirectory, QString type) :
    QDialog(parent),
    ui(new Ui::BookDialog)
{
    ui->setupUi(this);

    QMultiMap<int, Book> books = QMultiMap<int, Book>();
    QStringList files = bookDirectory.entryList(QStringList() << "*.json", QDir::Files);
    Book indexedBook;
    QString indexedPath;

    for(QString filename : files)
    {
        indexedPath = bookDirectory.path() + QDir::separator() + filename;
        indexedBook = Book::loadBook(indexedPath.toStdString());
        books.insert(indexedBook.batchID, indexedBook);
    }

    BookModel *model = new BookModel(books, type, this);
    //ui->treeView->setHeaderHidden(true);
    ui->treeView->setModel(model);
    ui->treeView->setAcceptDrops(true);

    //testing
    connect(ui->treeView, &QAbstractItemView::doubleClicked, [model] (const QModelIndex index)
    {
        qDebug() << "column: " << index.column();
        qDebug() << "row: " << index.row();
    });
}

BookDialog::~BookDialog()
{
    delete ui;
}
