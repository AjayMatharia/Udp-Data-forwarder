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
#include<QThread>
#include <utility>  // Required for std::as_const

#include "clickablelabel.h"
#include "HiddenIconsPopup.h"
#include "UdpStationWorker.h"




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
    QVector<QVector<QString>> *_receiverPorts2;
    QList<QThread*> m_udpThreads;
    QList<UdpStationWorker*> m_udpWorkers;


//     QMenuBar *_menuBar;
//     QMenu *_fileMenu;
//     QAction *_newAction ;
//     QAction *_openAction;
//     QAction *_exitAction;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void startAllUdpThreads();
    void onUdpDataReceived(const QString &stationName, const QString &textData, const QString &senderIp);
    void stopAllUdpThreads();
    // void startAllUdpThreads();

public slots:
    void readSettings();
};
#endif // MAINWINDOW_H
