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

#include <fstream>
#include <iomanip>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "book.h"
#include "models.h"
#include "bookdialog.h"
#include "settingsdialog.h"
#include "savedialog.h"
#include "version_config.h"

using json = nlohmann::json;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow), model(nullptr)
{
    this->setWindowTitle(QString("Book Tracker %1 [*]").arg(PROJECT_VERSION_STRING));
    ui->setupUi(this);
    diableUi();

    loadSettings();
    this->setupSlots();

    auto header = new QHeaderView(Qt::Horizontal);
    header->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    ui->treeView->setHeader(header);
    ui->treeView->setSelectionMode(QAbstractItemView::NoSelection);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveBook(); //Assumes that changes made before closing are valuable
}

void MainWindow::setupSlots() 
{
    //Action connections
    connect(ui->actionEdit, &QAction::triggered, this, &MainWindow::onActionEdit);
    connect(ui->actionMove, &QAction::triggered, this, &MainWindow::onActionMove);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onActionNew);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onActionSettings);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onActionSave);

    //Copy Button
    connect(ui->pushButtonCopy, &QPushButton::pressed, this, &MainWindow::copyDiscription);

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
    connect(ui->editBox, &QLineEdit::editingFinished, this, &MainWindow::update);
    connect(ui->editSection, &QLineEdit::editingFinished, this, &MainWindow::update);
    connect(ui->editExtra, &QPlainTextEdit::modificationChanged, this, &MainWindow::update);
}

void MainWindow::update()
{
    //Copy ui values to the book if there is an update
    this->copyToBook();

    //Now update, display the new values...
    displayCosts();
    displayStoreDisciption();
    displayPageCount();

    //The current book is likely modified
    this->setWindowModified(true);
}

void MainWindow::copyDiscription()
{
    QString text = ui->editDiscription->toPlainText();
    QClipboard *clipboard = static_cast<QApplication*>(parent())->clipboard();
    clipboard->setText(text);
}

void MainWindow::onBookEdit(Book bk)
{
    this->oldBook = bk; //Give it the same book to compare
    this->book = bk;
    this->setupBook();
    this->setWindowModified(false);
}

void MainWindow::onBookNew(Book bk)
{
    this->oldBook = Book(); //explicitly give it a invalid book
    this->book = bk;
    this->setupBook();
    this->setWindowModified(false);
}

void MainWindow::onBookMove(Book bk, int newBatch)
{
    this->oldBook = bk;
    this->book = bk;
    this->book.batchID = newBatch; //Now the book is in a different batch
    this->setupBook();
    this->setWindowModified(true); //Marked because the batchID is different, signalling that the user must actually save that change
}

void MainWindow::setupBook()
{
    this->book.constants = settings.bookconstants;

    enableUi();
    this->ui->spinPageDimX->setValue(this->book.pageDim.width);
    this->ui->spinPageDimY->setValue(this->book.pageDim.height);
    this->ui->spinCoverDimX->setValue(this->book.coverDim.width);
    this->ui->spinCoverDimY->setValue(this->book.coverDim.height);
    this->ui->spinSpineDim->setValue(this->book.spine);
    this->ui->spinWeight->setValue(this->book.weight);
    this->ui->spinSignitures->setValue(this->book.signitures);
    this->ui->spinExtra->setValue(this->book.costExtra);
    this->ui->spinPagesPerSig->setValue(this->book.pagesPerSigniture);
    this->ui->editEndPageColor->setText(QString::fromStdString(this->book.endpageColor));
    this->ui->editBox->setText(QString::fromStdString(this->book.box));
    this->ui->editSection->setText(QString::fromStdString(this->book.section));
    this->ui->editThreadColor->setText(QString::fromStdString(this->book.threadColor));
    this->ui->editCoverMaterial->setText(QString::fromStdString(this->book.coverMaterial));
    this->ui->editPageMaterial->setText(QString::fromStdString(this->book.pageMaterial));
    this->ui->comboBookType->setCurrentIndex(this->book.bookType - 1); //shift back down
    this->ui->comboStatus->setCurrentIndex(this->book.status - 1);

    displayCosts();
    displayStoreDisciption();
    displayProps();
    displayPageCount();

    this->ui->editExtra->setPlainText(QString::fromStdString(book.extra)); //TODO: if this occurs before other sets, it somehow messs with the book instance. Should investigate
}

void MainWindow::diableUi()
{
    ui->actionSave->setEnabled(false);
    ui->groupBoxCostBreakdown->setEnabled(false);
    ui->groupBoxDims->setEnabled(false);
    ui->groupBoxExtra->setEnabled(false);
    ui->groupBoxProps->setEnabled(false);
    ui->groupBoxPropsEdit->setEnabled(false);
    ui->groupBoxStore->setEnabled(false);
}

bool MainWindow::writeFile(json jsonObj, QString directory, QString filename)
{
    QDir dir(directory); //Qt io cant create parent directories on file write...
    if(!dir.exists())
    {
        if(!dir.mkpath(directory))
            return false;
    }

    QFile file(directory + QDir::separator() + filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    QString jsonText = QString::fromStdString(jsonObj.dump(4));
    out << jsonText;
    out.flush();
    return true;
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
    json jsonObj = json::parse(str.toStdString());
    return jsonObj;
}

void MainWindow::enableUi()
{
    ui->actionSave->setEnabled(true);
    ui->groupBoxCostBreakdown->setEnabled(true);
    ui->groupBoxDims->setEnabled(true);
    ui->groupBoxExtra->setEnabled(true);
    ui->groupBoxProps->setEnabled(true);
    ui->groupBoxPropsEdit->setEnabled(true);
    ui->groupBoxStore->setEnabled(true);
}

void MainWindow::writeBook()
{
    if(writeFile(book, QString::fromStdString(this->settings.bookDirectory), QString("book-%1.json").arg(this->book.bookID))) oldBook = book;
}

void MainWindow::saveBook()
{
    if(!book.isValid())
        return;

    book.updateTimestamp();
    writeBook();
    this->setWindowModified(false);
}

void MainWindow::discardBook()
{
    if(oldBook.isValid())
    {
        book = oldBook; //basically, we reset the current book, if the old book is valid, otherwise...
    }
    else
    {
        //...only copy the bookID and batchID over.
        this->book = Book(this->book.bookID, this->book.batchID);
    }
    this->setupBook();
    this->setWindowModified(false);
}

void MainWindow::onActionEdit()
{
    if(onActionSave())
    {
        BookDialog dialog(this, QString::fromStdString(settings.bookDirectory), "edit");
        BookModel *model = dialog.getModel();
        connect(model, &BookModel::bookLoad, this, &MainWindow::onBookEdit);
        dialog.exec();
    }
}

bool MainWindow::onActionSave()
{
    if(book.isValid())
    {
        qDebug() << "is valid";

        if(!(book == oldBook))
        {
            qDebug() << "different";

            SaveDialog dialog(book, oldBook);
            connect(&dialog, &SaveDialog::save, this, &MainWindow::saveBook);
            connect(&dialog, &SaveDialog::discard, this, &MainWindow::discardBook);
            return dialog.exec();
        }
        saveBook();
    }

    qDebug() << "hello";

    return true;
}

void MainWindow::onActionMove()
{
    if(onActionSave())
    {
        BookDialog dialog(this, QString::fromStdString(settings.bookDirectory), "move");
        BookModel *model = dialog.getModel();
        connect(model, &BookModel::bookMove, this, &MainWindow::onBookMove);
        dialog.exec();
    }
}

void MainWindow::onActionNew()
{
    if(onActionSave())
    {
        BookDialog dialog(this, QString::fromStdString(settings.bookDirectory), "new");
        BookModel *model = dialog.getModel();
        connect(model, &BookModel::bookLoad, this, &MainWindow::onBookNew);
        dialog.exec();
    }
}

void MainWindow::onActionSettings()
{
    SettingsDialog dialog(&settings);
    connect(&dialog, &SettingsDialog::accepted, this, &MainWindow::saveSettings);
    dialog.exec();
}

void MainWindow::saveSettings()
{
    if(writeFile(settings, QString::fromStdString(settings.configDirectory), "settings.json"))
    {
        if(book.isValid())
            this->update(); //update all the calculations
    }
}

void MainWindow::loadSettings()
{
    json obj = readFile(QString::fromStdString(settings.configDirectory), "settings.json");
    if(!obj.is_null())
    {
        this->settings = obj;
    }
}

void MainWindow::copyToBook()
{
    this->book.pageDim.width = ui->spinPageDimX->value();
    this->book.pageDim.height = ui->spinPageDimY->value();
    this->book.coverDim.width = ui->spinCoverDimX->value();
    this->book.coverDim.height = ui->spinCoverDimY->value();
    this->book.spine = ui->spinSpineDim->value();
    this->book.weight = ui->spinWeight->value();
    this->book.signitures = ui->spinSignitures->value();
    this->book.costExtra = ui->spinExtra->value();
    this->book.pagesPerSigniture = ui->spinPagesPerSig->value();

    this->book.endpageColor = ui->editEndPageColor->text().toStdString();
    this->book.box = ui->editBox->text().toStdString();
    this->book.section = ui->editSection->text().toStdString();
    this->book.threadColor = ui->editThreadColor->text().toStdString();
    this->book.coverMaterial = ui->editCoverMaterial->text().toStdString();
    this->book.pageMaterial = ui->editPageMaterial->text().toStdString();
    this->book.extra = ui->editExtra->toPlainText().toStdString();

    this->book.status = static_cast<Status>(ui->comboStatus->currentIndex() + 1); //shift from -1, 0, 1, 2, 3, 4 to 0, 1, 2, 3, 4, 5
    this->book.bookType = static_cast<BookType>(ui->comboBookType->currentIndex() + 1);
}

void MainWindow::displayCosts()
{
    if(book.isCalculatable())
    {
        ui->spinBoard->setValue(book.getBoardCost());
        ui->spinCloth->setValue(book.getClothCost());
        ui->spinThread->setValue(book.getThreadRibbonCost());
        ui->spinHeadband->setValue(book.getHeadbandCost());
        ui->spinPaper->setValue(book.getPageCost());
        ui->spinSuper->setValue(book.getSuperCost());
        ui->spinMisc->setValue(book.getExtraCosts());
        ui->spinTotal->setValue(book.getTotal());
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
    if (book.canHaveDiscription())
    {
        QString endpageColor, spineType, threadColor, coverMaterial, pageMaterial;

        endpageColor = QString::fromStdString(this->book.endpageColor);
        threadColor = QString::fromStdString(this->book.threadColor);
        coverMaterial = QString::fromStdString(this->book.coverMaterial);
        pageMaterial = QString::fromStdString(this->book.pageMaterial);

        QString spineStr;
        spineType = QString::fromStdString(book.getSpineType());

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
        ).arg(this->book.coverDim.width
        ).arg(this->book.coverDim.height
        ).arg(this->book.spine
        ).arg(this->book.pageDim.width
        ).arg(this->book.pageDim.height
        ).arg(this->book.calculatePageCount()
        ).arg(this->book.calculatePageCount() * 2);

        ui->editDiscription->setPlainText(str);
    }
    else
    {
        ui->editDiscription->setPlainText("");
    }
}

void MainWindow::displayProps()
{
    char lastEditStr[80];
    char creationStr[80];

    RootItem *root = new RootItem(2);

    strftime(lastEditStr, 80, "%b %d %Y %I:%M%p", localtime(&book.lastEdit));
    strftime(creationStr, 80, "%b %d %Y %I:%M%p", localtime(&book.creation));


    PropItem *bookID_prop = new PropItem();
    bookID_prop->appendData(tr("Book ID"));
    bookID_prop->appendData(QString::number(book.bookID));
    root->appendItem(bookID_prop);

    PropItem *batchID_prop = new PropItem();
    batchID_prop->appendData(tr("Batch ID"));
    batchID_prop->appendData(QString::number(book.batchID));
    root->appendItem(batchID_prop);

    PropItem *lastEdit_prop = new PropItem();
    lastEdit_prop->appendData(tr("Last Edited"));
    lastEdit_prop->appendData(QString::fromLocal8Bit(lastEditStr));
    root->appendItem(lastEdit_prop);

    PropItem *creation_prop = new PropItem();
    creation_prop->appendData(tr("Created"));
    creation_prop->appendData(QString::fromLocal8Bit(creationStr));
    root->appendItem(creation_prop);

    if (model == nullptr)
    {
        model = new PropsModel(root, (QList<QString>() << "Property" << "Value"));
    }
    else
    {
        model->setRootItem(root);
    }

    ui->treeView->setModel(model);

}

void MainWindow::displayPageCount()
{
    this->ui->spinPages->setValue(this->book.calculatePageCount());
}
