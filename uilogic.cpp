#include "uilogic.h"

#include <QDir>
#include <QTextStream>
#include <QDebug>

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace bookdata;

namespace uilogic {

Book oldBook = Book();
Book &book = oldBook;
Settings settings = Settings();

bool writeFile(json data, QString directory, QString filename)
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
    QString jsonText = QString::fromStdString(data.dump(4));
    out << jsonText;
    out.flush();
    return true;
}

json readFile(QFile &file)
{
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return json();
    }

    QTextStream in(&file);
    QString str = in.readAll();
    json jsonObj = json::parse(str.toStdString());
    return jsonObj;
}

std::vector<Book> getBooksOnDisks()
{
    QString dirStr = QString::fromStdString(settings.bookDirectory);
    QDir dir = QDir(dirStr);
    QStringList files = dir.entryList(QStringList() << "*.json", QDir::Files);
    std::vector<Book> books;

    for(const QString &filename : files)
    {
        QFile bookFile(dirStr + QDir::separator() + filename);
        json bookJson = uilogic::readFile(bookFile);
        if(!bookJson.is_null())
        {
            Book indexedBook = bookJson;
            if(Book::isValid(indexedBook))
            {
                books.push_back(indexedBook);
            }
        }
    }

    return books;
}

void loadSettings()
{
    Settings defaultSet = loadDefaultSettings();
    QFile file(QString::fromStdString(defaultSet.configDirectory) + QDir::separator() + "settings.json");
    json obj = uilogic::readFile(file);
    if(!obj.is_null())
    {
        settings = obj;
        settings.configDirectory = defaultSet.configDirectory; //does not really change...
    }
    else
    {
        settings = defaultSet;
    }
}

Settings loadDefaultSettings()
{
    Settings set;
    QDir dataPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir configPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    if(!dataPath.exists())
    {
        dataPath.mkpath(dataPath.path());
    }
    if(!configPath.exists())
    {
        configPath.mkpath(configPath.path());
    }
    set.bookDirectory = dataPath.path().toStdString();
    set.configDirectory = configPath.path().toStdString();
    set.bookconstants = bookdata::constants;

    return set;
}

void saveSettings()
{
    uilogic::writeFile(settings, QString::fromStdString(settings.configDirectory), "settings.json");
}

void saveBook()
{
    Book::updateTimestamp(book);
    uilogic::writeFile(book, QString::fromStdString(settings.bookDirectory), QString("book-%1.json").arg(book.bookID));
}

void revertBook()
{
    book = oldBook;
}

void newBook()
{
    oldBook = Book(nextNextID());
    book = oldBook;
}

int nextNextID()
{
    //Here, there is no attempt to predict the value, we are just going to seek the value again.
    std::vector<Book> books = uilogic::getBooksOnDisks();
    int max = 0;
    for (const Book &book : books)
    {
        if(book.bookID > max) max = book.bookID;
    }
    return ++max;
}

void loadBook(Book &incomingBook)
{
    oldBook = incomingBook;
    book = oldBook;
}

};
