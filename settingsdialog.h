#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QApplication>
#include <QMainWindow>
// #include <QMenuBar>
// #include <QMenu>
// #include <QAction>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QDebug>
#include <QTableWidget>
#include <QSettings>
#include <QtWidgets/QHeaderView>

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    void setData(QVector<QString> ports);

private:
    QLabel *header;
    QCheckBox *darkModeCheck;
    QCheckBox *notificationsCheck;
    QPushButton *okButton;
    QTableWidget *_portTable;
    QString _configPath;
    QVector<QString> ports2;
private slots:
    void readSettings();
    void saveSettings();
signals:
    void sendData(QVector<QString> ports);
};

#endif // SETTINGSDIALOG_H
