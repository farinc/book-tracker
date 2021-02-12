#ifndef CHANGEDIALOG_H
#define CHANGEDIALOG_H

#include <QDialog>
#include <QAbstractButton>

#include "models.h"

namespace Ui {
class ChangeDialog;
}

static QString getString(BookType type)
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
    }
}
static QString getString(Status type)
{
    switch (type)
    {
    case 0: return "Not Specified";
    case 1: return "No Photo";
    case 2: return "Drafted";
    case 3: return "Drafted w/ Photo";
    case 4: return "Published";
    case 5: return "Sold";
    }
}

static QString getString(Dimension dim)
{
    return QString("%1 in by %2 in").arg(dim.width).arg(dim.height);
}

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDialog(Book newBook, Book oldBook, QWidget *parent = nullptr);
    ~SaveDialog();
    void setupModel(Book newBook, Book oldBook);

public slots:
    void handleButtons(QAbstractButton *btn);

signals:
    void save();
    void discard();

private:
    Ui::ChangeDialog *ui;
    PropsModel *model;
};

#endif // CHANGEDIALOG_H
