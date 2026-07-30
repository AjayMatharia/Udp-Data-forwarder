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

#include "settingsdialog.h"  // IWYU pragma: keep
#include "settings.h"  // IWYU pragma: keep



class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QPushButton *_button;
    QTabWidget *_mainTable;
//     QMenuBar *_menuBar;
//     QMenu *_fileMenu;
//     QAction *_newAction ;
//     QAction *_openAction;
//     QAction *_exitAction;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
