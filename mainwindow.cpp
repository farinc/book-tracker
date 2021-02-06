#include <QObject>
#include <QString>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QAction>

#include <fstream>
#include <json.hpp>
#include <iomanip>

#include <QDebug>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "book.h"
#include "bookmodel.h"
#include "bookdialog.h"
#include "settingsdialog.h"

using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow), book(nullptr), settings(nullptr)
{
    ui->setupUi(this);
    loadSettings();
    this->setupSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete book;
    delete settings;
}

void MainWindow::setupSlots() 
{
    //Action connections
    connect(ui->actionEdit, &QAction::triggered, this, &MainWindow::onActionEdit);
    connect(ui->actionMove, &QAction::triggered, this, &MainWindow::onActionMove);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onActionNew);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onActionSettings);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveBook);
    connect(ui->actionSaveQuit, &QAction::triggered, this, &MainWindow::saveBook); //first...
    connect(ui->actionSaveQuit, &QAction::triggered, this, &MainWindow::close); //then second...

    //Ui refresh cost connections
    connect(ui->spinCoverDimX, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinCoverDimY, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinPageDimX, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinPageDimY, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinWeight, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinSpineDim, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinExtra, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinSignitures, &QSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinPagesPerSig, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::update);
    connect(ui->comboBookType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::update);
    connect(ui->comboStatus, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::update);
}

void MainWindow::update()
{
    if (book != nullptr)
    {
        //Copy ui values to the book if there is an update
        this->copyToBook();

        //Update the page count...
        this->ui->spinPages->setValue(this->book->calculatePageCount());

        //Recalculate the book prices...
        displayCosts();
    }
}

void MainWindow::onBookEdit(Book book)
{
    this->book = new Book(book);
    this->book->constants = settings->bookconstants;
    this->populateUi();
}

void MainWindow::onBookMove(Book book, int batchID)
{
    book.batchID = batchID;
    onBookEdit(book);
    saveBook(); //since the batchID is changed, the changes need to save first
}

void MainWindow::populateUi()
{
    this->ui->labelBookID->setText(QString::number(this->book->bookID));
    this->ui->labelBatchID->setText(QString::number(this->book->batchID));

    this->ui->spinPageDimX->setValue(this->book->pageDim.width);
    this->ui->spinPageDimY->setValue(this->book->pageDim.height);
    this->ui->spinCoverDimX->setValue(this->book->coverDim.width);
    this->ui->spinCoverDimY->setValue(this->book->coverDim.height);
    this->ui->spinSpineDim->setValue(this->book->spine);
    this->ui->spinWeight->setValue(this->book->weight);
    this->ui->spinSignitures->setValue(this->book->signitures);
    this->ui->spinExtra->setValue(this->book->costExtra);
    this->ui->spinPagesPerSig->setValue(this->book->pagesPerSignitures);

    this->ui->editEndPageColor->setText(QString::fromStdString(this->book->endpageColor));
    this->ui->editBox->setText(QString::fromStdString(this->book->box));
    this->ui->editSection->setText(QString::fromStdString(this->book->section));
    this->ui->editThreadColor->setText(QString::fromStdString(this->book->threadColor));
    this->ui->editCoverMaterial->setText(QString::fromStdString(this->book->coverMaterial));
    this->ui->editPageMaterial->setText(QString::fromStdString(this->book->pageMaterial));
    this->ui->editExtra->setPlainText(QString::fromStdString(this->book->extra));
    
    this->ui->comboBookType->setCurrentIndex(this->book->bookType);
    this->ui->comboStatus->setCurrentIndex(this->book->status);

    displayCosts();
}

void MainWindow::saveBook()
{
    if (book == nullptr)
        return;

    this->copyToBook();
    Book::saveBook(*book, pathBook());
}

void MainWindow::onActionEdit()
{
    saveBook();
    BookDialog dialog(this, QString::fromStdString(settings->bookDirectory), "edit");
    BookModel *model = dialog.getModel();
    connect(model, &BookModel::bookLoad, this, &MainWindow::onBookEdit);
    dialog.exec();
}

void MainWindow::onActionMove()
{
    saveBook();
    BookDialog dialog(this, QString::fromStdString(settings->bookDirectory), "move");
    BookModel *model = dialog.getModel();
    connect(model, &BookModel::bookMove, this, &MainWindow::onBookMove);
    dialog.exec();
}

void MainWindow::onActionNew()
{
    saveBook();
    BookDialog dialog(this, QString::fromStdString(settings->bookDirectory), "new");
    BookModel *model = dialog.getModel();
    connect(model, &BookModel::bookLoad, this, &MainWindow::onBookEdit);
    dialog.exec();
}

void MainWindow::onActionSettings()
{
    saveBook();
    SettingsDialog dialog(settings, this);
    connect(&dialog, &SettingsDialog::accepted, this, &MainWindow::saveSettings);
    dialog.exec();
}

void MainWindow::saveSettings()
{
    std::ofstream t(pathSettings());
    json jsonObj = *settings;
    t << std::setw(4) << jsonObj << std::endl;
}

void MainWindow::loadSettings()
{
    std::ifstream t(pathSettings());

    if (t)
    {
        std::string str((std::istreambuf_iterator<char>(t)),
                         std::istreambuf_iterator<char>());
        Settings set = json::parse(str);
        settings = new Settings(set);
    }
    else
    {
        settings = new Settings();
    }
}

std::string MainWindow::pathSettings()
{
    return QString(QDir::currentPath() + QDir::separator()).toStdString() + "settings.json";
}

std::string MainWindow::pathBook()
{
    return settings->bookDirectory + QString(QDir::separator()).toStdString() +this->book->getName() + ".json";
}

void MainWindow::copyToBook()
{
    this->book->pageDim.width = ui->spinPageDimX->value();
    this->book->pageDim.height = ui->spinPageDimY->value();
    this->book->coverDim.width = ui->spinCoverDimX->value();
    this->book->coverDim.height = ui->spinCoverDimY->value();
    this->book->spine = ui->spinSpineDim->value();
    this->book->weight = ui->spinWeight->value();
    this->book->signitures = ui->spinSignitures->value();
    this->book->costExtra = ui->spinExtra->value();
    this->book->pagesPerSignitures = ui->spinPagesPerSig->value();

    this->book->endpageColor = ui->editEndPageColor->text().toStdString();
    this->book->box = ui->editBox->text().toStdString();
    this->book->section = ui->editSection->text().toStdString();
    this->book->threadColor = ui->editThreadColor->text().toStdString();
    this->book->coverMaterial = ui->editCoverMaterial->text().toStdString();
    this->book->pageMaterial = ui->editPageMaterial->text().toStdString();
    this->book->extra = ui->editExtra->toPlainText().toStdString();

    this->book->status = static_cast<Status>(ui->comboStatus->currentIndex() + 1);
    this->book->bookType = static_cast<BookType>(ui->comboBookType->currentIndex() + 1);
}

void MainWindow::displayCosts()
{
    if(book->isCalculatable())
    {
        ui->spinBoard->setValue(book->getBoardCost());
        ui->spinCloth->setValue(book->getClothCost());
        ui->spinThread->setValue(book->getThreadRibbonCost());
        ui->spinHeadband->setValue(book->getHeadbandCost());
        ui->spinPaper->setValue(book->getPageCost());
        ui->spinSuper->setValue(book->getSuperCost());
        ui->spinExtra->setValue(book->getExtraCosts());
        ui->spinTotal->setValue(book->getTotal());
    }
}
