#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <json.hpp>

#include "book.h"

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
    
    CostConstants constants;
    Book book;
    QString bookDirectory;
    
    void updateCosts();
    void updatePages();
    
    void populateUi();
    void clearUi();

public slots:
    void onBookEdit(const Book book);
    
    //MainWindow slots
    void onEntryClose();
};

#endif // MAINWINDOW_H
