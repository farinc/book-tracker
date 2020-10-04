#include <nlohmann/json.hpp>

#include <QUrl>
#include <QObject>
#include <QString>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QDebug>
#include <fstream>
#include <iomanip>

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "book.h"

using namespace books;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->state = State::inactive;
    this->toggleUi(1);
    this->setupSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSlots() 
{
    //General action slots
    connect(this->ui->comboMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::toggleUi);
    
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
            this->book.status = static_cast<Status>(index);
        });
    connect(this->ui->comboBookType, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int const index)
        { 
            this->book.bookType = static_cast<BookType>(index);
        });
    
    //Float inputs (Technically doubles, but they are down-casted)
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

json MainWindow::loadJson(QUrl path)
{
    if(path.isLocalFile()){
        std::ifstream t(path.toString().toStdString());
        json jsonObj;
        t >> jsonObj;
        t.close();
        return jsonObj;
    }
    
    return nullptr;
}

void MainWindow::updateCosts()
{
    
}

void MainWindow::updatePages()
{
    this->ui->spinPages->setValue(this->book.calculatePageCount());
}

void MainWindow::saveJson(json jsonObj, QUrl path)
{
    if(path.isValid()){
        std::ofstream t(path.toString().toStdString());
        t << std::setw(4) << jsonObj << std::endl;
        t.close();
    }
}

void MainWindow::onEntryLoad(const QUrl* path)
{
    json bookJson = this->loadJson(*path);
    this->book = bookJson;
    this->populateUi();
}

void MainWindow::onEntryClose()
{
    this->clearUi();
    delete &(this->book);
}

void MainWindow::toggleUi(const int index)
{
    this->state = static_cast<State>(index);
    if(this->state == State::active){
        this->ui->discriptionFrame->setEnabled(true);
    }else if(this->state == State::inactive){
        this->ui->discriptionFrame->setEnabled(false);
    }
}

void MainWindow::populateUi()
{
    this->ui->labelBookID->setText(QString(this->book.bookID));
    this->ui->labelBatchID->setText(QString(this->book.batchID));

    this->ui->comboMode->setEnabled(true);
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

void MainWindow::clearUi()
{
    this->ui->labelBookID->setText(" <None> ");
    this->ui->labelBatchID->setText(" <None>");

    this->ui->comboMode->setEnabled(false);
    this->ui->actionSaveClose->setEnabled(false);
    this->ui->actionNewCurrent->setEnabled(true);
    this->ui->actionNewNew->setEnabled(true);
    this->ui->actionLoad->setEnabled(true);
    this->ui->actionMove->setEnabled(false);

    this->ui->spinPageDimX->setValue(0.0);
    this->ui->spinPageDimY->setValue(0.0);
    this->ui->spinCoverDimX->setValue(0.0);
    this->ui->spinCoverDimY->setValue(0.0);
    this->ui->spinSpineDim->setValue(0.0);
    this->ui->spinWeight->setValue(0.0);
    this->ui->spinSignitures->setValue(0);
    this->ui->spinPagesPerSig->setValue(0);
    this->ui->spinExtra->setValue(0.0);

    this->ui->editEndPageColor->setText("");
    this->ui->editBox->setText("");
    this->ui->editSection->setText("");
    this->ui->editThreadColor->setText("");
    this->ui->editCoverMaterial->setText("");
    this->ui->editPageMaterial->setText("");
    this->ui->editExtra->setPlainText("");

    this->ui->comboBookType->setCurrentIndex(0);
    this->ui->comboStatus->setCurrentIndex(0);

    this->ui->spinCost->setValue(0.0);
    this->ui->spinPages->setValue(0);
}
