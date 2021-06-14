#include "bookdialog.h"
#include "./ui_bookdialog.h"

#include <QDir>
#include <QUrl>
#include <QHeaderView>
#include <QDebug>
#include <QSortFilterProxyModel>

#include "models.h"

using namespace bookdata;
using json = nlohmann::json;

BookDialog::BookDialog(std::vector<Book> &books, QWidget *parent): QDialog(parent), ui(new Ui::BookDialog), sortModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    auto header = new QHeaderView(Qt::Horizontal);
    header->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->treeView->setHeader(header);

    setupModel(books);
}

BookDialog::~BookDialog()
{
    delete ui;
    delete sortModel;
}


void BookDialog::setupModel(std::vector<Book> &books)
{
    BasicModel *model = new BasicModel(4);
    model->setHeaderData({tr("Book ID"), tr("Last Edited"), tr("Box"), tr("Section")});

    qDebug() << books.size();

    for(Book &book : books)
    {
        BookItem *item = new BookItem(book);
        model->addItem(item);
    }

    sortModel->setSourceModel(model);
    sortModel->sort(1, Qt::AscendingOrder); //We sort first by ID...
    ui->treeView->setModel(sortModel);
}
