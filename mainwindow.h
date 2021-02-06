#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>

#include "book.h"
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

private:
    Ui::MainWindow *ui;
    
    void setupSlots();
    std::string pathSettings();
    std::string pathBook();
    
    Book *book;
    Settings *settings;

    void copyToBook();
    void displayCosts();
    void populateUi();
    void clearUi();

public slots:
    //UI slots
    void onActionEdit();
    void onActionMove();
    void onActionNew();
    void onActionSettings();
    void saveBook();
    void update();

    //Logical slots
    void onBookEdit(Book book);
    void onBookMove(Book book, int batchID);
    void saveSettings();
    void loadSettings();
};

#endif // MAINWINDOW_H
