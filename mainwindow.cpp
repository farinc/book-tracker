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
using namespace uilogic;
using namespace bookdata;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Untiltled [*]"));
    setupUiSlots();
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
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
    QClipboard *clipboard = static_cast<QApplication*>(this->parent())->clipboard();
    clipboard->setText(text);
}

void MainWindow::setupUi()
{
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
    this->ui->comboBookType->setCurrentIndex(book.bookType - 1); //shift back down
    this->ui->comboStatus->setCurrentIndex(book.status - 1);

    displayCosts();
    displayStoreDisciption();
    displayProps();
    displayPageCount();

    this->ui->editExtra->setPlainText(QString::fromStdString(book.extra)); //TODO: if this occurs before other sets, it somehow messs with the book instance. Should investigate
}

void MainWindow::enableUi()
{
    ui->groupBoxCostBreakdown->setEnabled(true);
    ui->groupBoxDims->setEnabled(true);
    ui->groupBoxExtra->setEnabled(true);
    ui->groupBoxProps->setEnabled(true);
    ui->groupBoxPropsEdit->setEnabled(true);
    ui->groupBoxStore->setEnabled(true);
}

void MainWindow::onSaveBook()
{
    saveBook();
    this->setWindowModified(false);
}

void MainWindow::onRevertBook()
{
    revertBook();
    setupUi();
    this->setWindowModified(false); //set back to an unmodified state...
}

void MainWindow::onLoadBook(Book &incomingBook)
{
    loadBook(incomingBook);
    this->setWindowTitle(QString("Book %1 [*]").arg(incomingBook.bookID));
}

void MainWindow::onSaveSettings()
{
    saveSettings();
}

void MainWindow::setModified()
{
    this->setWindowModified(true);
}

void MainWindow::onActionEdit()
{
    if(onActionReview())
    {
        auto books = getBooksOnDisks();
        BookDialog dialog(books);
        connect(&dialog, &BookDialog::loadBook, this, &MainWindow::onLoadBook);
        dialog.exec();
    }
}

bool MainWindow::onActionReview()
{
    copyToBook(); //important, otherwise book is exactly the same as oldbook

    if(!(book == oldBook))
    {
        ReviewDialog dialog(book, oldBook);
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
        newBook();
    }
}

void MainWindow::onActionSettings()
{
    SettingsDialog dialog(settings);
    connect(&dialog, &SettingsDialog::accepted, this, &MainWindow::onSaveSettings);
    dialog.exec();
}

void MainWindow::copyToBook()
{
    book.pageDim.width = ui->spinPageDimX->value();
    book.pageDim.height = ui->spinPageDimY->value();
    book.coverDim.width = ui->spinCoverDimX->value();
    book.coverDim.height = ui->spinCoverDimY->value();
    book.spine = ui->spinSpineDim->value();
    book.weight = ui->spinWeight->value();
    book.signitures = ui->spinSignitures->value();
    book.costExtra = ui->spinExtra->value();
    book.pagesPerSigniture = ui->spinPagesPerSig->value();

    book.endpageColor = ui->editEndPageColor->text().toStdString();
    book.box = ui->editBox->text().toStdString();
    book.section = ui->editSection->text().toStdString();
    book.threadColor = ui->editThreadColor->text().toStdString();
    book.coverMaterial = ui->editCoverMaterial->text().toStdString();
    book.pageMaterial = ui->editPageMaterial->text().toStdString();
    book.extra = ui->editExtra->toPlainText().toStdString();

    book.status = static_cast<Status>(ui->comboStatus->currentIndex() + 1); //shift from -1, 0, 1, 2, 3, 4 to 0, 1, 2, 3, 4, 5
    book.bookType = static_cast<BookType>(ui->comboBookType->currentIndex() + 1);
}

void MainWindow::displayCosts()
{
    if(Book::isCalculatable(book))
    {
        ui->spinBoard->setValue(Book::getBoardCost(book));
        ui->spinCloth->setValue(Book::getClothCost(book));
        ui->spinThread->setValue(Book::getThreadRibbonCost(book));
        ui->spinHeadband->setValue(Book::getHeadbandCost(book));
        ui->spinPaper->setValue(Book::getPageCost(book));
        ui->spinSuper->setValue(Book::getSuperCost(book));
        ui->spinMisc->setValue(Book::getExtraCosts(book));
        ui->spinTotal->setValue(Book::getTotal(book));
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

}

void MainWindow::displayPageCount()
{
    this->ui->spinPages->setValue(Book::calculatePageCount(book));
}
