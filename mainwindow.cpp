// qt imports

#include <QObject>
#include <QString>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QAction>
#include <QTextStream>
#include <QDebug>
#include <QStandardPaths>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QClipboard>
#include <QStandardItemModel>
#include <QDateTime>

//ui stuff

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "bookdialog.h"
#include "settingsdialog.h"
#include "reviewdialog.h"
#include "version_config.h"
#include "confirmdialog.h"

//logic stuff

#include "book.h"
#include "models.h"
#include "uilogic.h"

using json = nlohmann::json;
using namespace bookdata;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow), model(new BasicModel(2)), logic(new UiLogic()), isEditting(false)
{
    ui->setupUi(this);
    initUi();
    copyToUI();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(this->isWindowModified())
    {
        ConfirmDialog dialog(tr("Are you sure you want to quit?\nAll unsaved changes will be lost!"), tr("Close Book Tracker?"));
        if(!dialog.exec())
        {
            event->ignore();
        }
    }
}

void MainWindow::copyDiscription()
{
    QString text = ui->editDiscription->toPlainText();
    QClipboard *clipboard = qobject_cast<QApplication*>(this->parent())->clipboard();
    clipboard->setText(text);
}

void MainWindow::copyToUI()
{
    isEditting = true;

    auto book = logic->book;
    this->ui->spinPageDimX->setValue(book.pageDim.width);
    this->ui->spinPageDimY->setValue(book.pageDim.height);
    this->ui->spinCoverDimX->setValue(book.coverDim.width);
    this->ui->spinCoverDimY->setValue(book.coverDim.height);
    this->ui->spinSpineDim->setValue(book.spine);
    this->ui->spinWeight->setValue(book.weight);
    this->ui->spinSignitures->setValue(book.signitures);
    this->ui->spinExtra->setValue(book.costExtra);
    this->ui->spinPagesPerSig->setValue(book.pagesPerSigniture);
    this->ui->editEndPageColor->setText(QString::fromStdString(book.endpageColor));
    this->ui->editBox->setText(QString::fromStdString(book.box));
    this->ui->editSection->setText(QString::fromStdString(book.section));
    this->ui->editThreadColor->setText(QString::fromStdString(book.threadColor));
    this->ui->editCoverMaterial->setText(QString::fromStdString(book.coverMaterial));
    this->ui->editPageMaterial->setText(QString::fromStdString(book.pageMaterial));
    this->ui->comboBookType->setCurrentIndex(book.bookType);
    this->ui->comboStatus->setCurrentIndex(book.status);
    this->ui->editExtra->setPlainText(QString::fromStdString(book.extra));
    this->setWindowTitle(QString("Book %1 [*]").arg(logic->book.bookID));
    this->setWindowModified(false);

    display();

    isEditting = false;
}

void MainWindow::onSaveBook()
{
    logic->saveBook();
    this->setWindowModified(false);
}

void MainWindow::onRevertBook()
{
    logic->revertBook();
    copyToUI();
}

void MainWindow::onLoadBook(const int &incomingID)
{
    logic->loadBook(incomingID);
    copyToUI();
}

void MainWindow::onSaveSettings()
{
    logic->saveSettings();
    copyToUI();
}

void MainWindow::onDeleteBooks(std::vector<int> books)
{
    for(int id : books)
    {
        logic->deleteBook(id);
        if(id == logic->book.bookID)
        {
            //updating the ui since this book no longer exists
            logic->newBook();
            copyToUI();
        }
    }
}

void MainWindow::setModified()
{
    if(!isEditting)
    {
        copyToBook();
        if(!(logic->book == logic->oldBook)){
            this->setWindowModified(true);
        }else{
            this->setWindowModified(false);
        }
    }

    display();
}

void MainWindow::onActionEdit()
{
    if(onActionReview())
    {
        auto books = logic->getLoadedBooks();
        BookDialog dialog(books);
        connect(&dialog, &BookDialog::loadBook, this, &MainWindow::onLoadBook);
        connect(&dialog, &BookDialog::deleteBooks, this, &MainWindow::onDeleteBooks);
        dialog.exec();
    }
}

bool MainWindow::onActionReview()
{
    //at this point, the window must be modified if the book is different...
    if(this->isWindowModified())
    {
        ReviewDialog dialog(logic);
        connect(&dialog, &ReviewDialog::save, this, &MainWindow::onSaveBook);
        connect(&dialog, &ReviewDialog::discard, this, &MainWindow::onRevertBook);
        return dialog.exec(); //basically, this only reterns false if the "cancel" option is clicked
    }

    return true;
}

void MainWindow::onActionNew()
{
    if(onActionReview())
    {
        logic->newBook();
        copyToUI();
    }
}

void MainWindow::onActionSettings()
{
    SettingsDialog dialog(logic);
    connect(&dialog, &SettingsDialog::accepted, this, &MainWindow::onSaveSettings);
    dialog.exec();
}

void MainWindow::copyToBook()
{
    logic->book.pageDim.width = ui->spinPageDimX->value();
    logic->book.pageDim.height = ui->spinPageDimY->value();
    logic->book.coverDim.width = ui->spinCoverDimX->value();
    logic->book.coverDim.height = ui->spinCoverDimY->value();
    logic->book.spine = ui->spinSpineDim->value();
    logic->book.weight = ui->spinWeight->value();
    logic->book.signitures = ui->spinSignitures->value();
    logic->book.costExtra = ui->spinExtra->value();
    logic->book.pagesPerSigniture = ui->spinPagesPerSig->value();

    logic->book.endpageColor = ui->editEndPageColor->text().trimmed().toStdString();
    logic->book.box = ui->editBox->text().trimmed().toStdString();
    logic->book.section = ui->editSection->text().trimmed().toStdString();
    logic->book.threadColor = ui->editThreadColor->text().trimmed().toStdString();
    logic->book.coverMaterial = ui->editCoverMaterial->text().trimmed().toStdString();
    logic->book.pageMaterial = ui->editPageMaterial->text().trimmed().toStdString();
    logic->book.extra = ui->editExtra->toPlainText().trimmed().toStdString();

    logic->book.status = static_cast<Status>(ui->comboStatus->currentIndex());
    logic->book.bookType = static_cast<BookType>(ui->comboBookType->currentIndex());
}

void MainWindow::display()
{
    displayCosts();
    displayStoreDisciption();
    displayProps();
    displayPageCount();
}

void MainWindow::displayCosts()
{
    auto book = logic->book;
    auto constants = logic->settings.bookconstants;
    if(Book::isCalculatable(book))
    {
        ui->spinBoard->setValue(Book::getBoardCost(book, constants));
        ui->spinCloth->setValue(Book::getClothCost(book, constants));
        ui->spinThread->setValue(Book::getThreadRibbonCost(book, constants));
        ui->spinHeadband->setValue(Book::getHeadbandCost(book, constants));
        ui->spinPaper->setValue(Book::getPageCost(book, constants));
        ui->spinSuper->setValue(Book::getSuperCost(book, constants));
        ui->spinMisc->setValue(Book::getExtraCosts(book, constants));
        ui->spinTotal->setValue(Book::getTotal(book, constants));
    }
    else
    {
        ui->spinBoard->setValue(0);
        ui->spinCloth->setValue(0);
        ui->spinThread->setValue(0);
        ui->spinHeadband->setValue(0);
        ui->spinPaper->setValue(0);
        ui->spinSuper->setValue(0);
        ui->spinMisc->setValue(0);
        ui->spinTotal->setValue(0);
    }
}

void MainWindow::displayStoreDisciption()
{
    auto book = logic->book;
    if (Book::canHaveDiscription(book))
    {
        QString endpageColor, spineType, threadColor, coverMaterial, pageMaterial;

        endpageColor = QString::fromStdString(book.endpageColor);
        threadColor = QString::fromStdString(book.threadColor);
        coverMaterial = QString::fromStdString(book.coverMaterial);
        pageMaterial = QString::fromStdString(book.pageMaterial);

        QString spineStr;
        spineType = QString::fromStdString(Book::getSpineType(book));

        if (spineType == "")
        {
            spineStr = "";
        }
        else
        {
            spineStr = QString("%1: %2").arg(spineType, threadColor);
        }

        QString str = QString(
            "Cover: %1\n"
            "%2\n"
            "Paper: %3\n"
            "Inside covers: %4\n\n"
            "Cover: %5 in. by %6 in.\n"
            "Spine: %7 in.\n"
            "Page: %8 in. by %9 in.\n"
            "%11 pages / %12 sides"
        ).arg(coverMaterial, spineStr, pageMaterial, endpageColor
        ).arg(book.coverDim.width
        ).arg(book.coverDim.height
        ).arg(book.spine
        ).arg(book.pageDim.width
        ).arg(book.pageDim.height
        ).arg(Book::calculatePageCount(book)
        ).arg(Book::calculatePageCount(book) * 2);

        ui->editDiscription->setPlainText(str);
    }
    else
    {
        ui->editDiscription->setPlainText("");
    }
}

void MainWindow::displayProps()
{
    auto book = logic->book;
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    model->reset();
    model->setHeaderData({tr("Property"), tr("Value")});

    PropItem *item1 = new PropItem(tr("Book ID"), {book.bookID});
    model->addItem(item1);

    PropItem *item2 = new PropItem(tr("Created On"), {QDateTime::fromSecsSinceEpoch(book.creation).toLocalTime().toString("h:m AP on M/d/yyyy")});
    model->addItem(item2);

    PropItem *item3 = new PropItem(tr("Last Edited"), {QDateTime::fromSecsSinceEpoch(book.lastEdit).toLocalTime().toString("h:m AP on M/d/yyyy")});
    model->addItem(item3);

    ui->treeView->setModel(model);
}

void MainWindow::displayPageCount()
{
    this->ui->spinPages->setValue(Book::calculatePageCount(logic->book));
}

void MainWindow::initUi()
{
    logic->changeStyle();

    //Setup the combo boxes with a default, non-selectable entry

    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->comboBookType->model());

    QStandardItem* defaultItem = new QStandardItem();
    defaultItem->setSelectable(false);
    defaultItem->setText("Choose Type");
    model->insertRow(0, defaultItem);

    model = qobject_cast<QStandardItemModel*>(ui->comboStatus->model());

    defaultItem = new QStandardItem();
    defaultItem->setSelectable(false);
    defaultItem->setText("Choose Status");
    model->insertRow(0, defaultItem);

    ui->comboBookType->setCurrentIndex(0);
    ui->comboStatus->setCurrentIndex(0);

    // Setup actions
    connect(ui->actionEdit, &QAction::triggered, this, &MainWindow::onActionEdit);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onActionNew);
    connect(ui->actionReview, &QAction::triggered, this, &MainWindow::onActionReview);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onActionSettings);

    // Setup modification signal logic
    connect(ui->spinExtra, &QDoubleSpinBox::valueChanged, this, &MainWindow::setModified);
    connect(ui->spinPageDimX, &QDoubleSpinBox::valueChanged, this, &MainWindow::setModified);
    connect(ui->spinPageDimY, &QDoubleSpinBox::valueChanged, this, &MainWindow::setModified);
    connect(ui->spinCoverDimX, &QDoubleSpinBox::valueChanged, this, &MainWindow::setModified);
    connect(ui->spinCoverDimY, &QDoubleSpinBox::valueChanged, this, &MainWindow::setModified);
    connect(ui->editPageMaterial, &QLineEdit::textChanged, this, &MainWindow::setModified);
    connect(ui->editCoverMaterial, &QLineEdit::textChanged, this, &MainWindow::setModified);
    connect(ui->editThreadColor, &QLineEdit::textChanged, this, &MainWindow::setModified);
    connect(ui->editEndPageColor, &QLineEdit::textChanged, this, &MainWindow::setModified);
    connect(ui->editBox, &QLineEdit::textChanged, this, &MainWindow::setModified);
    connect(ui->editSection, &QLineEdit::textChanged, this, &MainWindow::setModified);
    connect(ui->spinWeight, &QDoubleSpinBox::valueChanged, this, &MainWindow::setModified);
    connect(ui->spinSpineDim, &QDoubleSpinBox::valueChanged, this, &MainWindow::setModified);
    connect(ui->comboBookType, &QComboBox::currentIndexChanged, this, &MainWindow::setModified);
    connect(ui->comboStatus, &QComboBox::currentIndexChanged, this, &MainWindow::setModified);
    connect(ui->spinSignitures, &QSpinBox::valueChanged, this, &MainWindow::setModified);
    connect(ui->spinPagesPerSig, &QSpinBox::valueChanged, this, &MainWindow::setModified);
    connect(ui->editExtra, &QPlainTextEdit::textChanged, this, &MainWindow::setModified);

    connect(ui->pushButtonCopy, &QPushButton::pressed, this, &MainWindow::copyDiscription);
}
