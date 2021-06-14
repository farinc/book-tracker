#ifndef UILOGIC_H
#define UILOGIC_H

#include <QStandardPaths>
#include <QFile>

#include <nlohmann/json.hpp>

#include "book.h"

namespace uilogic {

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
extern bookdata::Book oldBook;

/**
 * @brief book
 *
 * This is the active book. When the "review" action is executed, all values from the gui will be writen to this object.
 * This object will ONLY be writen to disk (json) when the "saveBook" slot is activated.
 *
 * Initialized with the value of oldbook, which is an invalid book
 */
extern bookdata::Book &book;

/**
 * @brief The Settings struct
 *
 *
 */
struct Settings
{
    std::string configDirectory;
    std::string bookDirectory;
    bookdata::CostConstants bookconstants;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings, bookDirectory, bookconstants);
};

extern Settings settings;

bool writeFile(nlohmann::json data, QString directory, QString filename);
nlohmann::json readFile(QFile &file);

void loadSettings();
Settings loadDefaultSettings();
void saveSettings();

void newBook();
void loadBook(bookdata::Book &incomingBook);
void saveBook();
void revertBook();

std::vector<bookdata::Book> getBooksOnDisks();
int nextNextID();

};

#endif // UILOGIC_H
