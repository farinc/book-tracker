#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <json.hpp>

#include "book.h"

using json = nlohmann::json;

namespace gui {

enum State: int {
    active, inactive
};

}

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
    using State = gui::State;
    Ui::MainWindow *ui;
    
    void setupSlots();
    
    CostConstants constants;
    Book book;
    State state;
    QString bookDirectory;

    QMap<int, Book*> loadedBooks;
    
    void updateCosts();
    void updatePages();
    
    void populateUi();
    void clearUi();

public slots:
    //once a book is selected for edit slots
    void onEntryLoad(const json);
    
    //MainWindow slots
    void onEntryClose();

    void onBookDirectoryChange(QUrl directory);
};

#endif // MAINWINDOW_H
