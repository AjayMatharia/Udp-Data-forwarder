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
#include <QDateTime>
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
    QVector<QString> *_stationPort;
    QVector<QString> *_stationName;
    QVector<QString> *_receiverIp;
    QVector<QString> *_receiverName;
    QVector<QString> *_receiverPort ;
    QVector<QVector<QString>> *_receiverPorts;
    QVector<QVector<QString>> *_receiverPorts2;
    QList<QThread*> m_udpThreads;
    QList<UdpStationWorker*> m_udpWorkers;
    QWidget* _mainWidget;
    QList<QLabel*> _stationLabels;
    QList<ClickableLabel*> _receiverPortsLabelobj;
    QGridLayout* mainLayout;
     QList<QLabel*> _speedLabelsObj;
     QMap<QString, QList<qint64>> m_stationPacketTimes;



//     QMenuBar *_menuBar;
//     QMenu *_fileMenu;
//     QAction *_newAction ;
//     QAction *_openAction;
//     QAction *_exitAction;
    void clearMainWidget();
    void buildMainGrid();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void startAllUdpThreads();
    void onUdpDataReceived(const QString &stationName, const QString &textData, const QString &senderIp,int packetSize);
    void stopAllUdpThreads();
    // void startAllUdpThreads();

public slots:
    void readSettings();
    // void MainWindow::onUdpDataReceived(const QString &stationName, const QString &textData, const QString &senderIp, int packetSize);
};
#endif // MAINWINDOW_H
