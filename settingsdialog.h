#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QDir>
#include <json.hpp>
#include <QAbstractButton>
#include "book.h"
#include "costitemsmodel.h"

namespace Ui {
class SettingsDialog;
}

struct Settings
{
    std::string bookDirectory = QDir::currentPath().toStdString();
    CostConstants bookconstants = CostConstants();
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings, bookDirectory, bookconstants);
};

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(Settings settings, QWidget *parent = nullptr);
    ~SettingsDialog();
    Settings settings() const;

public slots:
    void onBrowse();
    void handleButtons(QAbstractButton *btn);

signals:
    void settingsChanged(Settings newSettings);

private:
    Ui::SettingsDialog *ui;
    CostItemsModel *model;
    Settings tempSettings;
    std::string tempDir;
    void setupModel();
};

#endif // SETTINGSDIALOG_H
