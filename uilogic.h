#ifndef UILOGIC_H
#define UILOGIC_H

#include <QStandardPaths>
#include <QFile>

#include <nlohmann/json.hpp>

#include "book.h"

struct Settings
{
    std::string configDirectory;
    std::string bookDirectory;
    bookdata::CostConstants bookconstants;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings, bookDirectory, bookconstants);
};

static QString getString(bookdata::BookType type)
{
    switch (type)
    {
    case 0: return "Not Specified";
    case 1: return "Traditional";
    case 2: return "Coptic";
    case 3: return "Two Needle Coptic";
    case 4: return "Stab Stich";
    case 5: return "Quarter";
    case 6: return "Long Stich";
    default: return QString();
    }
}
static QString getString(bookdata::Status type)
{
    switch (type)
    {
    case 0: return "Not Specified";
    case 1: return "No Photo";
    case 2: return "Drafted";
    case 3: return "Drafted w/ Photo";
    case 4: return "Published";
    case 5: return "Sold";
    default: return QString();
    }
}

static QString getString(bookdata::Dimension dim)
{
    return QString("%1 in by %2 in").arg(dim.width, 0, 'f', 3).arg(dim.height, 0, 'f', 3);
}

class UiLogic {

public:

UiLogic();
~UiLogic();

/**
 * @brief oldbook
 *
 * This is the original book. Used to compare to the active book and for reverting. Reverting is defined by the
 * replacement of the value of the active book with this one and only occurs when the "revertBook" slot is invoked
 * (Thus, there are two conditions in which this occurs:
 *  - The "revert" action is taken, in which reverting occurs
 *  - The "discard" option is pressed in the "review" dialog
 * ).
 *
 * This object only changes value when a new book is pulled from the "book" dialog (here we assume that unsaved values from the gui are dealt with,
 * either by discarding them or keeping them already). It is NOT changed when a book is writen to disk!
 */
bookdata::Book oldBook;

/**
 * @brief book
 *
 * This is the active book. When the "review" action is executed, all values from the gui will be writen to this object.
 * This object will ONLY be writen to disk (json) when the "saveBook" slot is activated.
 *
 */
bookdata::Book book;

Settings settings;

void loadSettings();
Settings loadDefaultSettings();
void newBook();
void loadBook(const int &incomingID);
void deleteBook(int id);
void saveBook();
void revertBook();
void saveSettings();
std::vector<bookdata::Book*> getLoadedBooks();

private:

/**
 * @brief books
 *
 * This is the repository of books on disk. The following is the properties of this repository:
 * 1) A book only changes in value per id (same mem addr. for a given id)
 * 2) an id can be entirely deleted (map.remove) and its relavent data. This occurs only when a book is deleted from disk
 * 3) an id can be added (map.) and relativent data. This occurs when a books is saved to disk.
 */
std::map<int, bookdata::Book*> books;

bool writeFile(nlohmann::json data, QString directory, QString filename);
nlohmann::json readFile(QFile &file);
bool deleteFile(QFile &file);
int nextBookID();
void getBooksOnDisks();

};

#endif // UILOGIC_H
