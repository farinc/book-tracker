#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <nlohmann/json.hpp>
#include <QAbstractButton>

#include "book.h"
#include "models.h"
#include "uilogic.h"

namespace Ui {
class SettingsDialog;
}

class SpinBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    SpinBoxDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(uilogic::Settings &settings, QWidget *parent = nullptr);
    ~SettingsDialog();

private slots:
    void onBrowse();
    void handleButtons(QAbstractButton *btn);
    void onSetSettings();

private:
    Ui::SettingsDialog *ui;
    uilogic::Settings &settings;
    uilogic::Settings copySettings;
    QSortFilterProxyModel *model;
    std::string tempDir;
    void setupModel();
};

#endif // SETTINGSDIALOG_H
