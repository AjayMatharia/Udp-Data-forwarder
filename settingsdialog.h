#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QDebug>

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);

private:
    QLabel *header;
    QCheckBox *darkModeCheck;
    QCheckBox *notificationsCheck;
    QPushButton *okButton;
};

#endif // SETTINGSDIALOG_H
