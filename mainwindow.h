#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <nlohmann/json.hpp>

#include "book.h"
#include "models.h"

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
    void setupUiSlots();

    void displayCosts();
    void displayStoreDisciption();
    void displayProps();
    void displayPageCount();
    void enableUi();
    void setupUi();
    void copyToBook();

public slots:
    //UI slots
    void onActionEdit();
    bool onActionReview();
    void onActionNew();
    void onActionSettings();
    void copyDiscription();

    //Logical slots
    void onSaveBook();
    void onRevertBook();
    void onLoadBook(bookdata::Book &incomingBook);
    void onSaveSettings();
private slots:
    void setModified();
};

#endif // MAINWINDOW_H
