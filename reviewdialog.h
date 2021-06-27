#ifndef CHANGEDIALOG_H
#define CHANGEDIALOG_H

#include <QDialog>
#include <QAbstractButton>

#include "models.h"
#include "uilogic.h"

namespace Ui {
class ReviewDialog;
}

class ReviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReviewDialog(const UiLogic *logic, QWidget *parent = nullptr);
    ~ReviewDialog();
    void setupModel();

public slots:
    void handleButtons(QAbstractButton *btn);

signals:
    void save();
    void discard();

private:
    const UiLogic *logic;
    Ui::ReviewDialog *ui;
    BasicModel *model;
};

#endif // CHANGEDIALOG_H
