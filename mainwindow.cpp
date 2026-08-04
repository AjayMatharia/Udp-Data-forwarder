    #include "mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _stationPort = new QVector<QString>();
    _stationName = new QVector<QString>();
    _receiverIp = new QVector<QString>();
    _receiverName = new QVector<QString>();
    _receiverPorts = new QVector<QVector<QString>>();



    _configPath = QCoreApplication::applicationDirPath() + "/Settings/config.ini";
    _configPath2 = QCoreApplication::applicationDirPath() + "/Settings/configPort.ini";


    readSettings();

    QList<QLabel*> _stationLabels;

    QScreen *screen = QGuiApplication::primaryScreen();
    int physicalWidth = 800;  // Safe fallback defaults
    int physicalHeight = 600;
    if (screen) {
        physicalWidth = screen->size().width();
        physicalHeight = screen->size().height();
    }

    // 2. Create the primary Viewport Scroll Area container
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Style the scroll area window viewport track and vertical bar lines
    scrollArea->setStyleSheet(
        "QScrollArea { border: 1px solid #dcdcdc; background-color: #f9f9f9; }"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: #008080;" /* Your custom teal track color */
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

    // 3. Create the inner main content canvas widget
    auto _mainWidget = new QWidget();
    _mainWidget->setStyleSheet("background-color: green;");

    // 4. Create and configure the grid layout mapping system
    QGridLayout* mainLayout = new QGridLayout(_mainWidget);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setSpacing(5);

    // 5. Fill the Grid system with interactive custom clickable labels
        for (int row = 0; row < _stationName->size(); ++row) {
            // QString itemText = QString("Column %1 - Label %2").arg(col + 1).arg(row + 1);
            QString itemText = QString(_stationName->at(row));

            QLabel *label1 = new QLabel(_stationName->at(row));
            QLabel *label2 = new QLabel(_receiverName->at(row));

            ClickableLabel* label3 = new ClickableLabel("Click_to_enable_Receiver_Port");
            label3->setMouseTracking(true);
            label3->setCursor(Qt::PointingHandCursor);

            // Set layout stylesheet details cleanly avoiding duplicate overrides
            label1->setStyleSheet(
                "QLabel {"
                "    background-color: blue; "
                "    color: white; "
                "    border: 1px solid #e0e0e0; "
                "    border-radius: 4px; "
                "    padding: 6px;"
                "}"
                "QLabel:hover {"
                "    border: 1px solid #00adb5; " /* Beautiful glowing cyan hover highlight */
                "    background-color: #1a1a1a; "
                "}"
                );
            label2->setStyleSheet(
                "QLabel {"
                "    background-color: lightgreen; "
                "    color: white; "
                "    border: 1px solid #e0e0e0; "
                "    border-radius: 4px; "
                "    padding: 6px;"
                "}"
                "QLabel:hover {"
                "    border: 1px solid #00adb5; " /* Beautiful glowing cyan hover highlight */
                "    background-color: #1a1a1a; "
                "}"
                );
            label3->setStyleSheet(
                "QLabel {"
                "    background-color: black; "
                "    color: white; "
                "    border: 1px solid #e0e0e0; "
                "    border-radius: 4px; "
                "    padding: 6px;"
                "}"
                "QLabel:hover {"
                "    border: 1px solid #00adb5; " /* Beautiful glowing cyan hover highlight */
                "    background-color: #1a1a1a; "
                "}"
                );

            // CONNECT THE CLICK SIGNAL TO OPEN THE CHECKBOX POP-UP WINDOW
            QObject::connect(label3, &ClickableLabel::clicked, [label3, this]() {
                HiddenIconsPopup* popup = new HiddenIconsPopup(this);

                QPoint globalPos = label3->mapToGlobal(QPoint(0, 0));
                int targetX = globalPos.x() - popup->width() - 4;
                int targetY = globalPos.y() + (label3->height() / 2) - (popup->height() / 2);

                popup->move(targetX, targetY);

                // REPLACED popup->show() WITH THE SMOOTH FADE ANIMATION
                popup->startFadeIn(350); // 350 milliseconds makes for an elegant pop-open look
            });





            mainLayout->addWidget(label1, row, 0);
            mainLayout->addWidget(label2, row, 1);
            mainLayout->addWidget(label3, row, 3);
        }


    // 6. Connect your fully built content canvas container to the parent scroll framing area
    scrollArea->setWidget(_mainWidget);

    // 7. Force window layout layer viewport display below standard menu systems
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

