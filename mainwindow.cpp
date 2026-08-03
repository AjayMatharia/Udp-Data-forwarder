    #include "mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    QScreen *screen = QGuiApplication::primaryScreen();
    int physicalWidth=0,physicalHeight=0;

    if (screen) {
        // qreal dpr = screen->devicePixelRatio(); // e.g., 1.5 or 2.0

        // Multiply logical size by the device pixel ratio
        // physicalWidth = screen->size().width() * dpr;
        // physicalHeight = screen->size().height() * dpr;
        physicalWidth = screen->size().width() ;
        physicalHeight = screen->size().height() ;
        // qDebug() << "True Hardware Resolution:" << physicalWidth << dpr << physicalHeight;
    }
    // 1. Create the main wrapper widget (Set this as your Central Widget if using QMainWindow)
    // auto _mainWidget = new QWidget(this);
    // QHBoxLayout* mainLayout = new QHBoxLayout(_mainWidget);
    // mainLayout->setContentsMargins(10, 10, 10, 10);
    // mainLayout->setSpacing(15);

    // Create the 3 main columns using an array/loop
    // for (int i = 0; i < 3; ++i) {
    //     // A. Create the Scroll Area container for the column
    //     QScrollArea* scrollArea = new QScrollArea(_mainWidget);
    //     scrollArea->setWidgetResizable(true); // Allows internal labels to resize smoothly
    //     scrollArea->setStyleSheet("QScrollArea { border: 1px solid #dcdcdc; background-color: #f9f9f9; }");

    //     // B. Create the inner widget that holds the actual labels
    //     QWidget* containerWidget = new QWidget();
    //     QVBoxLayout* columnLayout = new QVBoxLayout(containerWidget);
    //     columnLayout->setAlignment(Qt::AlignTop); // Forces labels to stack tightly at the top
    //     columnLayout->setSpacing(1);

    //     // C. Fill the column with a collection of QLabels (e.g., 20 labels per column)
    //     for (int j = 1; j <= 20; ++j) {
    //         QLabel* label = new QLabel(QString("Column %1 - Label %2").arg(i + 1).arg(j));
    //         label->setStyleSheet("background-color: black; border: 1px solid #e0e0e0; padding: 6px;");
    //         columnLayout->addWidget(label);
    //     }

    //     // D. Connect the inner widget to the scroll window frame
    //     scrollArea->setWidget(containerWidget);

    //     // E. Add the scrollable column to your primary horizontal screen layout
    //     // Gives equal widths to all three columns (stretch factor = 1)
    //     mainLayout->addWidget(scrollArea, 1);
    // }

    // // If your class inherits from QMainWindow, set it here to respect the menu bar:
    // this->setCentralWidget(_mainWidget);

    // 1. Create the Scroll Area container
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Style the scroll area and its scrollbar components
    scrollArea->setStyleSheet(
        "QScrollArea { border: 1px solid #dcdcdc; background-color: #f9f9f9; }"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #008080;"
        "    width: 10px;"
        "    margin: 0px 0px 0px 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #bcbcbc;"
        "    min-height: 20px;"
        "    border-radius: 5px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #a0a0a0;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        );

    // 2. Create the inner main content widget
    auto _mainWidget = new QWidget();
    _mainWidget->setStyleSheet("background-color: green;");

    // 3. Create and configure the grid layout
    QGridLayout* mainLayout = new QGridLayout(_mainWidget);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setSpacing(5);

    // 4. Fill the layout with labels
    // 4. Fill the layout with custom clickable labels
    for (int col = 0; col < 3; ++col) {
        for (int row = 0; row < 300; ++row) {
            QString itemText = QString("Column %1 - Label %2").arg(col + 1).arg(row + 1);

            // Use our new class instead of standard QLabel
            ClickableLabel* label = new ClickableLabel(itemText);
            label->setMouseTracking(true);

            // 1. Set the stylesheet without the invalid cursor property
            label->setStyleSheet(
                "QLabel {"
                "    background-color: black; "
                "    color: white; "
                "    border: 1px solid #e0e0e0; "
                "    border-radius: 4px; "
                "    padding: 6px;"
                "}"
                "QLabel:hover {"
                "    border: 1px solid #00adb5; "
                "    background-color: #1a1a1a; "
                "}" // Removed "cursor: pointer;" from here
                );

            // 2. The Native Qt Way: This automatically changes the cursor to a pointing hand on hover
            label->setCursor(Qt::PointingHandCursor);


            // CONNECT THE CLICK SIGNAL TO A LAMBDA TO OPEN THE POP-UP
            QObject::connect(label, &ClickableLabel::clicked, [this, itemText]() {
                // Create a clean modal pop-up dialog window
                QDialog* popup = new QDialog(this);
                popup->setWindowTitle("Label Details");
                popup->setAttribute(Qt::WA_DeleteOnClose); // Automatically frees memory when closed

                QVBoxLayout* popupLayout = new QVBoxLayout(popup);

                // Add custom info text inside the pop-up
                QLabel* infoText = new QLabel(QString("You clicked on:\n%1").arg(itemText), popup);
                infoText->setAlignment(Qt::AlignCenter);
                popupLayout->addWidget(infoText);

                // Show the pop-up window
                popup->resize(250, 150);
                popup->exec(); // .exec() opens it as a modal (blocks the main window until closed)
            });

            mainLayout->addWidget(label, row, col);
        }
    }

    // for (int col = 0; col < 3; ++col) {
    //     for (int row = 0; row < 300; ++row) { // Changed row index to start from 0
    //         QLabel* label = new QLabel(QString("Column %1 - Label %2").arg(col + 1).arg(row + 1));
    //         // label->setStyleSheet(
    //         //     "background-color: black; "
    //         //     "color: white; "
    //         //     "border: 2px solid red; "  /* Format: width style color */
    //         //     "border-radius: 4px; "    /* Optional: Rounds the sharp corners */
    //         //     "padding: 6px;"
    //         //     );
    //         label->setStyleSheet(
    //             "QLabel {"
    //             "    background-color: black; "
    //             "    color: white; "
    //             "    border: 1px solid #444444; "
    //             "    padding: 6px;"
    //             "}"
    //             "QLabel:hover {"  /* FIXED: Changed '::hover' to ':hover' */
    //             "    border: 1px solid #00adb5; "
    //             "    background-color: #1a1a1a; "
    //             "}"
    //             );


    //         // CRITICAL FIX: Changed text color to white so it's readable on black background
    //         // label->setStyleSheet("background-color: black; color: white; border: 1px solid #e0e0e0; padding: 6px;");

    //         mainLayout->addWidget(label, row, col);
    //     }
    // }

    // // 5. Connect the populated inner widget to the scroll area
    // scrollArea->setWidget(_mainWidget);

    // // 6. Set the scroll area as the central widget of your QMainWindow
    this->setCentralWidget(scrollArea);



    // QWidget* part1 = new QWidget();
    // QWidget* part2 = new QWidget();
    // QWidget* part3 = new QWidget(); // Your arrow button column

    // // Add widgets to layout with stretch values: (widget, stretch_factor)
    // mainLayout->addWidget(part1, 33); // Takes 40% width
    // mainLayout->addWidget(part2, 33); // Takes 40% width
    // mainLayout->addWidget(part3, 33); // Takes 20% width
    // part1->setStyleSheet("background-color: black;");
    // part2->setStyleSheet("background-color: yellow;");
    // part3->setStyleSheet("background-color: black;");

    // auto * part1Layout = new QVBoxLayout(part1);
    // auto * part2Layout = new QVBoxLayout(part2);
    // auto * part3Layout = new QVBoxLayout(part3);


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
    _stationPort = new QVector<QString>();
    _stationName = new QVector<QString>();
    _receiverIp = new QVector<QString>();
    _receiverName = new QVector<QString>();
    _receiverPorts = new QVector<QVector<QString>>();



    _configPath = QCoreApplication::applicationDirPath() + "/Settings/config.ini";
    _configPath2 = QCoreApplication::applicationDirPath() + "/Settings/configPort.ini";


    readSettings();

    QList<QLabel*> _stationLabels;

    // for (int i = 0; i <_stationName->size(); ++i) {
    //     _stationLabels.append(new QLabel);
    //     _stationLabels.at(i)->setText(_stationName->at(i));
    //     part1Layout->addWidget(_stationLabels.at(i));
    // }







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

    // qDebug()<<"ok"<<size;


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


        _receiverPorts->append(temp);

        // 2. Closes the array group before moving to the next 'i'
        settings2.endArray();
    }
    // qDebug() << *_receiverPorts;
}

