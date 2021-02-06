#include "bookdialog.h"
#include "mainwindow.h"
#include "./ui_bookdialog.h"
#include <QDir>
#include <QUrl>
#include <QHeaderView>

#include <QDebug>

#include "bookmodel.h"

BookDialog::BookDialog(QWidget *parent, QString bookDirectory, QString type) : QDialog(parent), ui(new Ui::BookDialog), type(type)
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
    else if(type == "move")
    {
        this->ui->infoLabel->setText("Please select a batch to move the selected book into");
        this->setWindowTitle("Move Entry");
    }

    model = BookModel::generateModel(bookDirectory, type);

    ui->treeView->setModel(model);

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
