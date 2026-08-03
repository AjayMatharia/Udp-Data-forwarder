#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QApplication>
#include<QGuiApplication>
#include<QScrollArea>
#include<QScreen>
#include "clickablelabel.h"


#include "settingsdialog.h"  // IWYU pragma: keep
#include "settings.h"  // IWYU pragma: keep



class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QPushButton *_button;
    QTabWidget *_mainTable;
    QString _configPath;
    QString _configPath2;
    QVector<QString> *_stationPort;
    QVector<QString> *_stationName;
    QVector<QString> *_receiverIp;
    QVector<QString> *_receiverName;
    QVector<QVector<QString>> *_receiverPorts;

//     QMenuBar *_menuBar;
//     QMenu *_fileMenu;
//     QAction *_newAction ;
//     QAction *_openAction;
//     QAction *_exitAction;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void readSettings();
};
#endif // MAINWINDOW_H
