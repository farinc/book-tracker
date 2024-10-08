#include "uilogic.h"

#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QApplication>

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace bookdata;

UiLogic::UiLogic()
{
    loadSettings();
    getBooksOnDisks();
    newBook();
}

UiLogic::~UiLogic()
{
    for(std::pair<int, Book*> pair : books)
    {
        delete pair.second;
    }
}

bool UiLogic::writeFile(json data, QString directory, QString filename)
{
    QDir dir(directory); //Qt io cant create parent directories on file write...
    if(!dir.exists())
    {
        if(!dir.mkpath(directory))
            return false;
    }

    QFile file(directory + "/" + filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    QString jsonText = QString::fromStdString(data.dump(4));
    out << jsonText;
    out.flush();
    return true;
}

json UiLogic::readFile(QFile &file)
{
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return json();
    }

    QTextStream in(&file);
    QString str = in.readAll();
    try {
        return json::parse(str.toStdString()); 
    } catch (const json::parse_error& e) {
        qDebug() << e.what();
    }
    return json();
}

bool UiLogic::deleteFile(QFile &file)
{
    return file.moveToTrash();
}

void UiLogic::getBooksOnDisks()
{
    QString dirStr = QString::fromStdString(settings.bookDirectory);
    QDir dir = QDir(dirStr);
    QStringList files = dir.entryList(QStringList() << "*.json", QDir::Files);

    for(const QString &filename : qAsConst(files))
    {
        QFile bookFile(dirStr + "/" + filename);
        json bookJson = readFile(bookFile);
        if(!bookJson.is_null())
        {
            try {
                Book indexedBook = bookJson;
                if(Book::isValid(indexedBook))
                {
                    books[indexedBook.bookID] = new Book(indexedBook);
                }
            } catch (const json::exception& e) {
                qDebug() << e.what();
            }
        }
    }
}


void UiLogic::loadSettings()
{
    Settings defaultSet = loadDefaultSettings();
    QFile file(QString::fromStdString(defaultSet.configDirectory) + "/settings.json");
    json obj = readFile(file);
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

Settings UiLogic::loadDefaultSettings()
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
    set.style = "system";
    set.configDirectory = configPath.path().toStdString();
    set.bookconstants = bookdata::constants;

    return set;
}

void UiLogic::saveSettings()
{
    writeFile(settings, QString::fromStdString(settings.configDirectory), "settings.json");
}

std::vector<Book *> UiLogic::getLoadedBooks()
{
    std::vector<Book*> listbooks;

    for(std::pair<int, Book*> b : books)
    {
        listbooks.push_back(b.second);
    }

    return listbooks;
}

void UiLogic::changeStyle(std::string style)
{
    if(style == "dark" || style == "light")
    {
        QFile file(QString(":/qdarkstyle/%1/style.qss").arg(QString::fromStdString(style)));
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream stream(&file);
        qobject_cast<QApplication *>(QApplication::instance())->setStyleSheet(stream.readAll());
    }
    else
    {
        qobject_cast<QApplication *>(QApplication::instance())->setStyleSheet("");
    }
}

void UiLogic::changeStyle()
{
    UiLogic::changeStyle(settings.style);
}

void UiLogic::saveBook()
{
    Book::updateTimestamp(book);
    writeFile(book, QString::fromStdString(settings.bookDirectory), QString("book-%1.json").arg(book.bookID));
    books[book.bookID] = new Book(book); //add to map
    oldBook = book;
}

void UiLogic::revertBook()
{
    book = oldBook;
}

void UiLogic::newBook()
{
    oldBook = Book(nextBookID());
    book = oldBook;
}

int UiLogic::nextBookID()
{
    if (books.empty())
        return 1;

    return books.rbegin()->first + 1;
}

void UiLogic::loadBook(const int &incomingID)
{
    if(books.count(incomingID))
    {
        oldBook = *books[incomingID];
        book = oldBook;
    }
}

void UiLogic::deleteBook(int id)
{
    QFile file(QString::fromStdString(settings.bookDirectory) + "/" + QString("book-%1.json").arg(id));
    if(deleteFile(file))
    {
        delete books[id];
        books.erase(id);
    }
}


