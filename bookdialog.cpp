#include "bookdialog.h"
#include "./ui_bookdialog.h"
#include "confirmdialog.h"

#include <QDir>
#include <QUrl>
#include <QHeaderView>
#include <QDebug>
#include <QSortFilterProxyModel>

#include "models.h"

using namespace bookdata;
using json = nlohmann::json;

BookDialog::BookDialog(std::vector<bookdata::Book*> &books, QWidget *parent): QDialog(parent), ui(new Ui::BookDialog), sortModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);
    setupModel(books);
    connect(ui->treeView, &QTreeView::doubleClicked, this, &BookDialog::onDoubleClicked);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &BookDialog::onDelete);
    connect(this, &BookDialog::loadBook, this, &BookDialog::accept);
 }

BookDialog::~BookDialog()
{
    delete ui;
    delete sortModel;
}


void BookDialog::setupModel(std::vector<bookdata::Book*> &books)
{
    BasicModel *model = new BasicModel(4);
    connect(this, &BookDialog::reloadBooks, model, &BasicModel::reset);
    model->setHeaderData({tr("Book ID"), tr("Last Edited"), tr("Box"), tr("Section")});

    for(Book *book : books)
    {
        BookItem *item = new BookItem(*book);
        model->addItem(item);
    }

    sortModel->setSourceModel(model);
    sortModel->sort(1, Qt::AscendingOrder); //We sort first by ID...
    ui->treeView->setModel(sortModel);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void BookDialog::onDoubleClicked(const QModelIndex &index)
{
    if(index.column() == 0)
    {
        int bookid = sortModel->data(index, Qt::DisplayRole).toInt();
        emit loadBook(bookid);
    }
}

void BookDialog::onDelete()
{
    auto selected = ui->treeView->selectionModel()->selection().indexes();
    QString str("Are you sure you want to delete those book(s)?\n");
    QString bookstr{};
    std::vector<int> bks{};
    for(QModelIndex index : selected)
    {
        int id = sortModel->data(index, Qt::DisplayRole).toInt();
        QString bk = QString("Book %1 \n").arg(id);
        bks.push_back(id);
        bookstr.append(bk);
    }
    str.append(bookstr);

    ConfirmDialog dialog(str);
    if(dialog.exec())
    {
        emit deleteBooks(bks);
        accept();
    }
}
