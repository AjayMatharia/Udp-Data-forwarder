#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // _button = new QPushButton("PushButton",this);
    // _button->setStyleSheet(
    //     "QPushButton {"
    //     "    background-color: lightblue;"  // Default color
    //     "    border: 2px solid black;"
    //     "    border-radius: 5px;"
    //     "}"
    //     "QPushButton:hover {"
    //     "    background-color: lightgreen;"  // Color when hovered
    //     "}"
    //     );



    _configPath = QCoreApplication::applicationDirPath() + "/Settings/config.ini";
    _configPath2 = QCoreApplication::applicationDirPath() + "/Settings/configPort.ini";







    QMenuBar *_menuBar = new QMenuBar(this);

    // Create a "File" menu
    QMenu *_fileMenu = new QMenu("File", this);
    QMenu *_settingsMenu = new QMenu("Settings", this);


    // Create actions
    QAction *_newAction = new QAction("New", this);
    QAction *_openAction = new QAction("Open", this);
    QAction *_exitAction = new QAction("Exit", this);
    QAction *_settingsAction = new QAction("Preferences...", this);

    // Add actions to the menu
    _fileMenu->addAction(_newAction);
    _fileMenu->addAction(_openAction);
    _fileMenu->addSeparator();
    _fileMenu->addAction(_exitAction);

    _settingsMenu->addAction(_settingsAction);

    // Add menu to the menu bar
    _menuBar->addMenu(_fileMenu);
    _menuBar->addMenu(_settingsMenu);

    // Set the menu bar
    setMenuBar(_menuBar);




    // Connect actions to slots
    connect(_newAction, &QAction::triggered, this, [=]() {
        QMessageBox::information(this, "Action", "New File created!");
    });

    connect(_openAction, &QAction::triggered, this, [=]() {
        QMessageBox::information(this, "Action", "Open File dialog!");
    });

    connect(_exitAction, &QAction::triggered, this, [=]() {
        QApplication::quit();
    });


    connect(_settingsAction, &QAction::triggered, this, [=]() {
        Settings dlg(this);
        dlg.exec();
    });



    setWindowTitle("Data Forwarding");
    resize(400, 300);






}

MainWindow::~MainWindow() {

}



void MainWindow::readSettings(){
    QSettings settings(_configPath, QSettings::IniFormat);

    int size = settings.beginReadArray("Stations_info");



    for (int i = 0; i < size; ++i) {
        // Set the current index context
        settings.setArrayIndex(i);

        _stationName->append(settings.value("Station_Name: ", "Default Name").toString());
        _stationPort->append(settings.value("Port: ", "0000").toString());


    }

    settings.endArray();

    int size2 = settings.beginReadArray("receiver_info");

    for (int i = 0; i < size; ++i) {
        // Set the current index context
        settings.setArrayIndex(i);

        _receiverName->append(settings.value("receiver_Name: ", "Default Name").toString());
        _receiverIp->append(settings.value("IP_Address: ", "0.0.0.0").toString());



    }

    settings.endArray();


    // qDebug() << "size:"<<size2;

    QSettings settings2(_configPath2, QSettings::IniFormat);

    for (int i = 0; i < size2; ++i) {
        // 1. Opens the array group (e.g., "receiver_Ports0")
        int size = settings2.beginReadArray("receiver_Ports" + QString::number(i));
        QVector<QString> temp;

        for (int r = 0; r < size; ++r) {
            // FIX 1: Use 'r' (inner loop index) instead of 'i'
            settings2.setArrayIndex(r);

            // FIX 2: Fixed typo in "Receiver" if your INI file matches your key name
            temp.append(settings2.value("receiver_Port: ", "0000").toString());
        }

        // qDebug() << temp;

        _receiverPorts->append(temp);

        // 2. Closes the array group before moving to the next 'i'
        settings2.endArray();
    }
}
