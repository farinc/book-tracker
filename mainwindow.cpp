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

#include <fstream>
#include <json.hpp>
#include <iomanip>

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
    this->settings = new Settings();

    loadSettings();
    this->setupSlots();

    auto header = new QHeaderView(Qt::Horizontal);
    header->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    ui->treeView->setHeader(header);
    ui->treeView->setSelectionMode(QAbstractItemView::NoSelection);
    model = new BookPropsModel();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete book;
    delete settings;
    delete model;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveBook();
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

    //Ui refresh book changes, go back to "save every change." However, in this case, we are just copying ui values to the book pointer, not writing to file
    connect(ui->spinCoverDimX, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinCoverDimY, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinPageDimX, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinPageDimY, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinWeight, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinSpineDim, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinExtra, &QDoubleSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinSignitures, &QSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->spinPagesPerSig, &QSpinBox::editingFinished, this, &MainWindow::update);
    connect(ui->comboBookType, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::update);
    connect(ui->comboStatus, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::update);
    connect(ui->editCoverMaterial, &QLineEdit::editingFinished, this, &MainWindow::update);
    connect(ui->editPageMaterial, &QLineEdit::editingFinished, this, &MainWindow::update);
    connect(ui->editThreadColor, &QLineEdit::editingFinished, this, &MainWindow::update);
    connect(ui->editEndPageColor, &QLineEdit::editingFinished, this, &MainWindow::update);
}

void MainWindow::update()
{
    if (book != nullptr)
    {
        //Copy ui values to the book if there is an update
        this->copyToBook();

        //Now update, display the new values...
        displayPageCount();
        displayCosts();
        displayStoreDisciption();

        //Set the window to modified since the book is unsaved.
        this->setWindowModified(true);
    }
}

void MainWindow::onBookEdit(Book bk)
{
    if(this->book != nullptr)
    {
        this->oldBook = *this->book;
        *this->book = bk;
    }
    else
    {
        this->book = new Book(bk);
    }

    this->book->constants = settings->bookconstants;
    this->populateUi();
    this->setWindowModified(false);
}

void MainWindow::onBookMove(Book book, int batchID)
{
    book.batchID = batchID;
    onBookEdit(book);
    writeBook(); //dont do saveBook, since that recopies from the ui, which makes little sense
}

void MainWindow::populateUi()
{
    this->ui->spinPageDimX->setValue(this->book->pageDim.width);
    this->ui->spinPageDimY->setValue(this->book->pageDim.height);
    this->ui->spinCoverDimX->setValue(this->book->coverDim.width);
    this->ui->spinCoverDimY->setValue(this->book->coverDim.height);
    this->ui->spinSpineDim->setValue(this->book->spine);
    this->ui->spinWeight->setValue(this->book->weight);
    this->ui->spinSignitures->setValue(this->book->signitures);
    this->ui->spinExtra->setValue(this->book->costExtra);
    this->ui->spinPagesPerSig->setValue(this->book->pagesPerSigniture);

    this->ui->editEndPageColor->setText(QString::fromStdString(this->book->endpageColor));
    this->ui->editBox->setText(QString::fromStdString(this->book->box));
    this->ui->editSection->setText(QString::fromStdString(this->book->section));
    this->ui->editThreadColor->setText(QString::fromStdString(this->book->threadColor));
    this->ui->editCoverMaterial->setText(QString::fromStdString(this->book->coverMaterial));

    this->ui->editPageMaterial->setText(QString::fromStdString(this->book->pageMaterial));
    this->ui->editExtra->setPlainText(QString::fromStdString(this->book->extra));
    
    this->ui->comboBookType->setCurrentIndex(this->book->bookType - 1);
    this->ui->comboStatus->setCurrentIndex(this->book->status - 1);

    displayCosts();
    displayStoreDisciption();
    displayProps();
}

void MainWindow::writeFile(json jsonObj, QString directory, QString filename)
{
    QDir dir(directory); //Qt io cant create parent directories on file write...
    if(!dir.exists())
        dir.mkpath(directory);

    QFile file(directory + QDir::separator() + filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream out(&file);
    QString jsonText = QString::fromStdString(jsonObj.dump(4));
    out << jsonText;
    out.flush();
}

json MainWindow::readFile(QString directory, QString filename)
{
    QFile file(directory + QDir::separator() + filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return json();
    }

    QTextStream in(&file);
    QString str = in.readAll();
    qDebug() << str;
    json jsonObj = json::parse(str.toStdString());
    return jsonObj;
}

void MainWindow::saveBook()
{
    if (book == nullptr)
        return;

    this->copyToBook();
    writeBook();
    this->setWindowModified(false);
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
    writeFile(*settings, QString::fromStdString(settings->configDirectory), "settings.json");
}

void MainWindow::loadSettings()
{
    json obj = readFile(QString::fromStdString(settings->configDirectory), "settings.json");
    if(!obj.is_null())
    {
        *this->settings = obj;
    }
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
    this->book->pagesPerSigniture = ui->spinPagesPerSig->value();

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
        ui->spinMisc->setValue(book->getExtraCosts());
        ui->spinTotal->setValue(book->getTotal());
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
    if (book->canHaveDiscription())
    {
        QString endpageColor, spineType, threadColor, coverMaterial, pageMaterial;

        endpageColor = QString::fromStdString(this->book->endpageColor);
        threadColor = QString::fromStdString(this->book->threadColor);
        coverMaterial = QString::fromStdString(this->book->coverMaterial);
        pageMaterial = QString::fromStdString(this->book->pageMaterial);
        spineType = QString::fromStdString(book->getSpineType());

        QString str = QString(
            "Cover: %1\n"
            "%2: %3\n"
            "Paper: %4\n"
            "Inside covers: %5\n\n"
            "Cover: %6 in. by %7 in.\n"
            "Spine: %8 in.\n"
            "Page: %9 in. by %10 in.\n"
            "%11 pages / %12 sides"
        ).arg(coverMaterial, spineType, threadColor, pageMaterial, endpageColor
        ).arg(this->book->coverDim.width
        ).arg(this->book->coverDim.height
        ).arg(this->book->spine
        ).arg(this->book->pageDim.width
        ).arg(this->book->pageDim.height
        ).arg(this->book->calculatePageCount()
        ).arg(this->book->calculatePageCount() * 2);

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
    char lastEditStr[80];
    char creationStr[80];

    strftime(lastEditStr, 80, "%b %d %Y %I:%M%p", localtime(&book->lastEdit));
    strftime(creationStr, 80, "%b %d %Y %I:%M%p", localtime(&book->creation));

    PropItem *bookID_prop = new PropItem(tr("Book ID"), book->bookID);
    PropItem *batchID_prop = new PropItem(tr("Batch ID"), book->batchID);
    PropItem *lastEdit_prop = new PropItem(tr("Last Edited"), QString::fromLocal8Bit(lastEditStr));
    PropItem *creation_prop = new PropItem(tr("Created"), QString::fromLocal8Bit(creationStr));

    model->addItem(bookID_prop);
    model->addItem(batchID_prop);
    model->addItem(lastEdit_prop);
    model->addItem(creation_prop);
    ui->treeView->setModel(model);
}

void MainWindow::displayPageCount()
{
    this->ui->spinPages->setValue(this->book->calculatePageCount());
}

void MainWindow::writeBook()
{
    json Obj = *book;
    writeFile(Obj, QString::fromStdString(this->settings->bookDirectory), QString("book-%1.json").arg(this->book->bookID));
}
