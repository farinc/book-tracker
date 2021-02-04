#include <json.hpp>

#include <QObject>
#include <QString>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QAction>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "book.h"
#include "bookdialog.h"
#include "settingsdialog.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setupSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSlots() 
{
    //General action slots
    connect(this->ui->actionNew_Entry, &QAction::triggered, [this] ()
        {
            BookDialog dialog(this, this->bookDirectory, "new");
            dialog.exec();
        });
    connect(this->ui->actionEdit_Entry, &QAction::triggered, [this] ()
        {
            BookDialog dialog(this, this->bookDirectory, "edit");
            dialog.exec();
        });
    connect(this->ui->actionSetting, &QAction::triggered, [this] ()
        {
            SettingsDialog dialog(this);
            this->connect(&dialog, &SettingsDialog::bookDirectoryChange, [this] (QString value)
                {
                    this->bookDirectory = value;
                });

            dialog.exec();
        });
    connect(this->ui->actionSaveClose, &QAction::triggered, [this] ()
        {
            saveBook();
            this->close();
        });
    connect(this->ui->actionSave, &QAction::triggered, [this] ()
        {
            saveBook();
        });

    //Integer inputs
    connect(this->ui->spinSignitures, QOverload<int>::of(&QSpinBox::valueChanged), [this](int const value)
        { 
            this->book.signitures = value;
            this->updatePages();
        });
    connect(this->ui->spinPagesPerSig, QOverload<int>::of(&QSpinBox::valueChanged), [this](int const value)
        { 
            this->book.pagesPerSignitures = value;
            this->updatePages();
        });
    connect(this->ui->comboStatus, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int const index)
        { 
            this->book.status = static_cast<Status>(index + 1);
        });
    connect(this->ui->comboBookType, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int const index)
        { 
            this->book.bookType = static_cast<BookType>(index + 1);
        });
    
    //Float inputs
    connect(this->ui->spinWeight, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double const value)
        { 
            this->book.weight = value;
        });
    connect(this->ui->spinSpineDim, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double const value)
        { 
            this->book.spine = value;
        });
    connect(this->ui->spinExtra, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double const value)
        { 
            this->book.costExtra = value;
        });
    connect(this->ui->spinCoverDimX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double const value)
        { 
            this->book.coverDim.width = value;
        });
    connect(this->ui->spinCoverDimY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double const value)
        { 
            this->book.coverDim.height = value;
        });
    connect(this->ui->spinPageDimX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this](double const value)
        { 
            this->book.pageDim.width = value;
        });
    
    //Text inputs
    connect(this->ui->editBox, &QLineEdit::editingFinished, [this]()
        { 
            this->book.box = this->ui->editBox->text().toStdString();
        });
    connect(this->ui->editSection, &QLineEdit::editingFinished, [this]()
        { 
            this->book.section = this->ui->editSection->text().toStdString();
        });
    connect(this->ui->editThreadColor, &QLineEdit::editingFinished, [this]()
        { 
            this->book.threadColor = this->ui->editThreadColor->text().toStdString();
        });
    connect(this->ui->editEndPageColor, &QLineEdit::editingFinished, [this]()
        { 
            this->book.endpageColor = this->ui->editEndPageColor->text().toStdString();
        });
    connect(this->ui->editPageMaterial, &QLineEdit::editingFinished, [this]()
        { 
            this->book.pageMaterial = this->ui->editPageMaterial->text().toStdString();
        });
    connect(this->ui->editCoverMaterial, &QLineEdit::editingFinished, [this]()
        { 
            this->book.coverMaterial = this->ui->editCoverMaterial->text().toStdString();
        });
    connect(this->ui->editExtra, &QPlainTextEdit::textChanged, [this]()
        { 
            this->book.extra = this->ui->editExtra->toPlainText().toStdString();
        });
}

void MainWindow::updateCosts()
{
    
}

void MainWindow::updatePages()
{
    this->ui->spinPages->setValue(this->book.calculatePageCount());
}

void MainWindow::onBookEdit(Book book)
{
    this->book = book;
    this->populateUi();
}

void MainWindow::populateUi()
{
    this->ui->labelBookID->setText(QString::number(this->book.bookID));
    this->ui->labelBatchID->setText(QString::number(this->book.batchID));

    this->ui->actionSaveClose->setEnabled(true);
    this->ui->actionNewCurrent->setEnabled(false);
    this->ui->actionNewNew->setEnabled(false);
    this->ui->actionLoad->setEnabled(false);
    this->ui->actionMove->setEnabled(true);

    this->ui->spinPageDimX->setValue(this->book.pageDim.width);
    this->ui->spinPageDimY->setValue(this->book.pageDim.height);
    this->ui->spinCoverDimX->setValue(this->book.coverDim.width);
    this->ui->spinCoverDimY->setValue(this->book.coverDim.height);
    this->ui->spinSpineDim->setValue(this->book.spine);
    this->ui->spinWeight->setValue(this->book.weight);
    this->ui->spinSignitures->setValue(this->book.signitures);
    this->ui->spinExtra->setValue(this->book.costExtra);
    this->ui->spinPagesPerSig->setValue(this->book.pagesPerSignitures);

    this->ui->editEndPageColor->setText(QString::fromStdString(this->book.endpageColor));
    this->ui->editBox->setText(QString::fromStdString(this->book.box));
    this->ui->editSection->setText(QString::fromStdString(this->book.section));
    this->ui->editThreadColor->setText(QString::fromStdString(this->book.threadColor));
    this->ui->editCoverMaterial->setText(QString::fromStdString(this->book.coverMaterial));
    this->ui->editPageMaterial->setText(QString::fromStdString(this->book.pageMaterial));
    this->ui->editExtra->setPlainText(QString::fromStdString(this->book.extra));
    
    this->ui->comboBookType->setCurrentIndex(this->book.bookType);
    this->ui->comboStatus->setCurrentIndex(this->book.status);
}

void MainWindow::saveBook()
{
    QString path = this->bookDirectory + QDir::separator() + QString::fromStdString(this->book.getName()) + ".json";
    Book::saveBook(this->book, path.toStdString());
}
