#include "mainwindow.h"
#include "version_config.h"

#include <QApplication>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("book-tracker");
    a.setApplicationDisplayName(QString("Book Tracker %1").arg(PROJECT_VERSION_STRING));

    MainWindow w;
    w.show();

    return a.exec();
}
