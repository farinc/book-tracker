#ifndef CHANGEDIALOG_H
#define CHANGEDIALOG_H

#include <QDialog>
#include <QAbstractButton>

#include "models.h"

namespace Ui {
class ReviewDialog;
}

class ReviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReviewDialog(const bookdata::Book &newBook, const bookdata::Book &oldBook, QWidget *parent = nullptr);
    ~ReviewDialog();
    void setupModel(const bookdata::Book &newBook, const bookdata::Book &oldBook);

public slots:
    void handleButtons(QAbstractButton *btn);

signals:
    void save();
    void discard();

private:
    Ui::ReviewDialog *ui;
    BasicModel *model;
};

#endif // CHANGEDIALOG_H
