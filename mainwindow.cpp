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

//logic stuff

#include "book.h"
#include "models.h"
#include "uilogic.h"

using json = nlohmann::json;
using namespace bookdata;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow), model(new BasicModel(2)), logic(new UiLogic()), isEditting(false)
{
    ui->setupUi(this);
    setupUi();
    setupUiSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //saveBook(); //Assumes that changes made before closing are valuable
}

void MainWindow::setupUiSlots()
{
    connect(ui->actionEdit, &QAction::triggered, this, &MainWindow::onActionEdit);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onActionNew);
    connect(ui->actionReview, &QAction::triggered, this, &MainWindow::onActionReview);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onActionSettings);
}

void MainWindow::copyDiscription()
{
    QString text = ui->editDiscription->toPlainText();
    QClipboard *clipboard = qobject_cast<QApplication*>(this->parent())->clipboard();
    clipboard->setText(text);
}

void MainWindow::setupBook()
{
    isEditting = true;

    this->ui->spinPageDimX->setValue(logic->book.pageDim.width);
    this->ui->spinPageDimY->setValue(logic->book.pageDim.height);
    this->ui->spinCoverDimX->setValue(logic->book.coverDim.width);
    this->ui->spinCoverDimY->setValue(logic->book.coverDim.height);
    this->ui->spinSpineDim->setValue(logic->book.spine);
    this->ui->spinWeight->setValue(logic->book.weight);
    this->ui->spinSignitures->setValue(logic->book.signitures);
    this->ui->spinExtra->setValue(logic->book.costExtra);
    this->ui->spinPagesPerSig->setValue(logic->book.pagesPerSigniture);
    this->ui->editEndPageColor->setText(QString::fromStdString(logic->book.endpageColor));
    this->ui->editBox->setText(QString::fromStdString(logic->book.box));
    this->ui->editSection->setText(QString::fromStdString(logic->book.section));
    this->ui->editThreadColor->setText(QString::fromStdString(logic->book.threadColor));
    this->ui->editCoverMaterial->setText(QString::fromStdString(logic->book.coverMaterial));
    this->ui->editPageMaterial->setText(QString::fromStdString(logic->book.pageMaterial));
    this->ui->comboBookType->setCurrentIndex(logic->book.bookType);
    this->ui->comboStatus->setCurrentIndex(logic->book.status);
    this->ui->editExtra->setPlainText(QString::fromStdString(logic->book.extra));

    displayCosts();
    displayStoreDisciption();
    displayProps();
    displayPageCount();
    displayTitle();

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
    setupBook();
}

void MainWindow::onLoadBook(const int &incomingID)
{
    logic->loadBook(incomingID);
    setupBook();
}

void MainWindow::onSaveSettings()
{
    logic->saveSettings();
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
            displayProps();
            displayTitle();
            setupBook();
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

    displayCosts();
    displayPageCount();
    displayStoreDisciption();
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
        ReviewDialog dialog(logic->book, logic->oldBook);
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

void MainWindow::displayCosts()
{
    if(Book::isCalculatable(logic->book))
    {
        ui->spinBoard->setValue(Book::getBoardCost(logic->book));
        ui->spinCloth->setValue(Book::getClothCost(logic->book));
        ui->spinThread->setValue(Book::getThreadRibbonCost(logic->book));
        ui->spinHeadband->setValue(Book::getHeadbandCost(logic->book));
        ui->spinPaper->setValue(Book::getPageCost(logic->book));
        ui->spinSuper->setValue(Book::getSuperCost(logic->book));
        ui->spinMisc->setValue(Book::getExtraCosts(logic->book));
        ui->spinTotal->setValue(Book::getTotal(logic->book));
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
    if (Book::canHaveDiscription(logic->book))
    {
        QString endpageColor, spineType, threadColor, coverMaterial, pageMaterial;

        endpageColor = QString::fromStdString(logic->book.endpageColor);
        threadColor = QString::fromStdString(logic->book.threadColor);
        coverMaterial = QString::fromStdString(logic->book.coverMaterial);
        pageMaterial = QString::fromStdString(logic->book.pageMaterial);

        QString spineStr;
        spineType = QString::fromStdString(Book::getSpineType(logic->book));

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
        ).arg(logic->book.coverDim.width
        ).arg(logic->book.coverDim.height
        ).arg(logic->book.spine
        ).arg(logic->book.pageDim.width
        ).arg(logic->book.pageDim.height
        ).arg(Book::calculatePageCount(logic->book)
        ).arg(Book::calculatePageCount(logic->book) * 2);

        ui->editDiscription->setPlainText(str);
    }
    else
    {
        ui->editDiscription->setPlainText("");
    }
}

void MainWindow::displayProps()
{
    model->reset();
    model->setHeaderData({tr("Property"), tr("Value")});

    PropItem *item1 = new PropItem(tr("Book ID"), {logic->book.bookID});
    model->addItem(item1);

    PropItem *item2 = new PropItem(tr("Created On"), {QDateTime::fromSecsSinceEpoch(logic->book.creation)});
    model->addItem(item2);

    PropItem *item3 = new PropItem(tr("Last Edited"), {QDateTime::fromSecsSinceEpoch(logic->book.lastEdit)});
    model->addItem(item3);

    ui->treeView->setModel(model);
    ui->treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void MainWindow::displayPageCount()
{
    this->ui->spinPages->setValue(Book::calculatePageCount(logic->book));
}

void MainWindow::displayTitle()
{
    this->setWindowTitle(QString("Book %1 [*]").arg(logic->book.bookID));
    this->setWindowModified(false);
}

void MainWindow::setupUi()
{
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
    //Now add some props

    displayProps();
    displayTitle();
}
