#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <nlohmann/json.hpp>

#include "book.h"

using json = nlohmann::json;
using namespace books;

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
    BookEntry book;
    State state;
    
    void updateCosts();
    void updatePages();
    
    json loadJson(QUrl);
    void saveJson(json, QUrl);
    
    void populateUi();
    void clearUi();

public slots:
    void onEntryLoad(const QUrl* path);
    void onEntryClose();
    void toggleUi(const int index);
};

#endif // MAINWINDOW_H
