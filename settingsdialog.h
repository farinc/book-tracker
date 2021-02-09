#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QDir>
#include <json.hpp>
#include <QAbstractButton>
#include <QStandardPaths>
#include "book.h"
#include "costitemsmodel.h"

namespace Ui {
class SettingsDialog;
}

struct Settings
{
    std::string configDirectory = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation).toStdString();
    std::string bookDirectory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toStdString();
    CostConstants bookconstants = CostConstants();
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Settings, bookDirectory, bookconstants);
};

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(Settings *settings, QWidget *parent = nullptr);
    ~SettingsDialog();

public slots:
    void setSettings();
    void onBrowse();
    void handleButtons(QAbstractButton *btn);

private:
    Ui::SettingsDialog *ui;
    CostItemsModel *model;
    Settings *orginalSettingsPt;
    Settings tempSettings;
    std::string tempDir;
    void setupModel();
};

#endif // SETTINGSDIALOG_H
