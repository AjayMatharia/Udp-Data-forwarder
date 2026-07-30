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





    // // Create a menu bar
    // _menuBar = new QMenuBar(this);

    // // Create a "File" menu
    // _fileMenu = new QMenu("File", this);

    // // Create actions
    // _newAction = new QAction("New", this);
    // _openAction = new QAction("Open", this);
    // _exitAction = new QAction("Exit", this);


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
    // delete _button;
    // delete _mainTable;
    // delete _menuBar;
    // delete _fileMenu;
    // delete _newAction ;
    // delete _openAction;
    // delete _exitAction;
}
