#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <nlohmann/json.hpp>

#include "book.h"
#include "models.h"
#include "settingsdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::MainWindow *ui;
    PropsModel *model;
    Book book;
    Book oldBook;
    Settings settings;

    void setupSlots();
    void copyToBook();
    void displayCosts();
    void displayStoreDisciption();
    void displayProps();
    void displayPageCount();
    void writeBook();
    void discardBook();
    void setupBook();
    void clearUi();
    void diableUi();

public:
    static bool writeFile(nlohmann::json jsonObj, QString directory, QString filename);
    static nlohmann::json readFile(QString directory, QString filename);

public slots:
    //UI slots
    void enableUi();
    void onActionEdit();
    bool onActionSave();
    void onActionMove();
    void onActionNew();
    void onActionSettings();
    void saveBook();
    void update();
    void copyDiscription();

    //Logical slots
    void onBookEdit(Book book);
    void onBookNew(Book book);
    void onBookMove(Book book, int newBatch);
    void saveSettings();
    void loadSettings();
};

#endif // MAINWINDOW_H
