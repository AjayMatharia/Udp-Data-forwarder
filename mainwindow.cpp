    #include "mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _stationPort = new QVector<QString>();
    _stationName = new QVector<QString>();
    _receiverIp = new QVector<QString>();
    _receiverName = new QVector<QString>();
    _receiverPorts = new QVector<QVector<QString>>();
    _receiverPorts2 = new QVector<QVector<QString>>();

    _configPath = QCoreApplication::applicationDirPath() + "/Settings/config.ini";
    _configPath2 = QCoreApplication::applicationDirPath() + "/Settings/configPort.ini";

    readSettings();
    *_receiverPorts2 = *_receiverPorts;
    QList<QLabel*> _stationLabels;
    QList<ClickableLabel*> _receiverPortsLabelobj;

    QScreen *screen = QGuiApplication::primaryScreen();
    int physicalWidth = 800;
    int physicalHeight = 600;
    if (screen) {
        physicalWidth = screen->size().width();
        physicalHeight = screen->size().height();
    }

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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

    auto _mainWidget = new QWidget();
    _mainWidget->setStyleSheet("background-color: dark blue;");

    QGridLayout* mainLayout = new QGridLayout(_mainWidget);
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->setSpacing(5);

    // Fill the Grid system with interactive labels
    for (int row = 0; row < _stationName->size(); ++row) {
        QLabel *label1 = new QLabel(_stationName->at(row));
        QLabel *label2 = new QLabel("speed");

        ClickableLabel* label3 = new ClickableLabel("Click_to_enable_Receiver_Port");
        label3->setMouseTracking(true);
        label3->setCursor(Qt::PointingHandCursor);

        _receiverPortsLabelobj.append(label3);

        // Apply Stylesheets uniform parameters
        QString basicStyle =
            "QLabel { background-color: #005252; color: white; border: 1px solid #e0e0e0; border-radius: 4px; padding: 6px; }"
            "QLabel:hover { border: 1px solid #00adb5; background-color: #1a1a1a; }";

        label1->setStyleSheet(basicStyle);
        label2->setStyleSheet(basicStyle);
        label3->setStyleSheet(basicStyle);

        // Add to layout grid rows
        mainLayout->addWidget(label1, row, 0);
        mainLayout->addWidget(label2, row, 1);
        mainLayout->addWidget(label3, row, 2);

        QObject::connect(label3, &ClickableLabel::clicked, [this, label3, row]() {
            HiddenIconsPopup* popup = new HiddenIconsPopup(this);

            QStringList tempStationNames;
            QVector<QString> currentStates;

            if (_receiverPorts && !_receiverPorts->isEmpty() && _receiverName) {
                for (int j = 0; j < _receiverPorts->size(); ++j) {
                    if (j < _receiverName->size() && row < _receiverPorts->at(j).size()) {
                        QString displayStr = QString("%1: %2")
                        .arg(_receiverIp->at(j))
                            .arg(_receiverPorts->at(j).at(row));

                        tempStationNames.append(displayStr);
                        currentStates.append(_receiverPorts->at(j).at(row));
                    }
                }
            }

            popup->addPorts(tempStationNames, currentStates);
            popup->adjustSize();

            // 1. LISTEN TO LIVE POPUP TOGGLES INTERACTIVELY
            connect(popup, &HiddenIconsPopup::portToggledInPopup, [this, row](int receiverIdx, bool isChecked) {
                // Bounds guard checks
                if (!_receiverPorts2 || receiverIdx >= _receiverPorts2->size() || row >= _receiverPorts2->at(receiverIdx).size()) return;

                // Step A: Update internal memory matrix instantly
                _receiverPorts2->operator[](receiverIdx)[row] = isChecked ? "1" : "0";

                // Step B: Calculate fresh routing targets
                QList<QPair<QString, quint16>> updatedTargets;
                int receiverCount = _receiverName->size();

                for (int j = 0; j < receiverCount; ++j) {
                    if (j < _receiverPorts2->size() && row < _receiverPorts2->at(j).size()) {
                        if (_receiverPorts2->at(j).at(row) == "1") {
                            QString ip = _receiverIp->at(j);
                            quint16 destPort = _receiverPorts->at(j).at(row).toUShort();
                            updatedTargets.append(qMakePair(ip, destPort));
                        }
                    }
                }

                // Step C: Secure Thread Pointer Resolution via Object Name Mapping
                UdpStationWorker* activeWorker = nullptr;

                if (row < _stationName->size()) {
                    QString targetStationName = _stationName->at(row);

                    // FIX: Replaced deprecated qAsConst with modern std::as_const
                    for (UdpStationWorker* worker : std::as_const(m_udpWorkers)) {
                        if (worker && (worker->objectName() == targetStationName)) {
                            activeWorker = worker;
                            break;
                        }
                    }


                }

                // Step D: Execute Cross-Thread invocation safely
                if (activeWorker) {
                    using ForwardTargetsType = QList<QPair<QString, quint16>>;
                    QMetaObject::invokeMethod(activeWorker, "setForwardingTargets",
                                              Qt::QueuedConnection,
                                              Q_ARG(ForwardTargetsType, updatedTargets));
                    qDebug() << "Live network rerouted securely for station:" << _stationName->at(row);
                }
                qDebug()<<updatedTargets;
            });

            QPoint globalPos = label3->mapToGlobal(QPoint(0, 0));
            int targetX = globalPos.x() - popup->width() - 4;
            int targetY = globalPos.y() + (label3->height() / 2) - (popup->height() / 2);

            popup->move(targetX, targetY);
            popup->startFadeIn(350);
        });
    }
    scrollArea->setWidget(_mainWidget);
    setCentralWidget(scrollArea);
    resize(physicalWidth / 2, physicalHeight / 2);





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
    // resize(400, 300);



    startAllUdpThreads();


}

MainWindow::~MainWindow() {

}



void MainWindow::readSettings(){



    // QSettings settings(_configPath, QSettings::IniFormat);

    // int size = settings.beginReadArray("Stations_info");

    // // qDebug()<<"ok"<<size;


    // for (int i = 0; i < size; ++i) {
    //     // Set the current index context
    //     settings.setArrayIndex(i);

    //     _stationName->append(settings.value("Station_Name: ", "Default Name").toString());
    //     _stationPort->append(settings.value("Port: ", "0000").toString());


    // }

    // settings.endArray();

    // int size2 = settings.beginReadArray("receiver_info");

    // for (int i = 0; i < size; ++i) {
    //     // Set the current index context
    //     settings.setArrayIndex(i);

    //     _receiverName->append(settings.value("receiver_Name: ", "Default Name").toString());
    //     _receiverIp->append(settings.value("IP_Address: ", "0.0.0.0").toString());



    // }

    // settings.endArray();


    // // qDebug() << "size:"<<size2;



    // for (int i = 0; i < size2; ++i) {
    //     // 1. Opens the array group (e.g., "receiver_Ports0")
    //     int size = settings.beginReadArray("receiver_Ports" + QString::number(i));
    //     QVector<QString> temp;

    //     for (int r = 0; r < size; ++r) {
    //         // FIX 1: Use 'r' (inner loop index) instead of 'i'
    //         settings.setArrayIndex(r);

    //         // FIX 2: Fixed typo in "Receiver" if your INI file matches your key name
    //         temp.append(settings.value("receiver_Port: ", "0000").toString());
    //     }


    //     _receiverPorts->append(temp);

    //     // 2. Closes the array group before moving to the next 'i'
    //     settings.endArray();
    // }


    QSettings settings(_configPath, QSettings::IniFormat);

    // 1. Read Stations
    int size = settings.beginReadArray("Stations_info");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        // FIX: Removed trailing colons/spaces to perfectly match saveSettings keys
        _stationName->append(settings.value("Station_Name", "Default Name").toString());
        _stationPort->append(settings.value("Port", "0000").toString());
    }
    settings.endArray();

    // 2. Read Receivers
    int size2 = settings.beginReadArray("receiver_info");
    // FIX: Loop boundary changed from 'size' to 'size2' to avoid tracking wrong row indexes
    for (int i = 0; i < size2; ++i) {
        settings.setArrayIndex(i);
        // FIX: Removed trailing colons/spaces to match saveSettings keys
        _receiverName->append(settings.value("receiver_Name", "Default Name").toString());
        _receiverIp->append(settings.value("IP_Address", "0.0.0.0").toString());
    }
    settings.endArray();

    // 3. Read Receiver Ports Multi-dimensional Matrix Mapping
    for (int i = 0; i < size2; ++i) {
        // FIX: Appended the correct underscore "_" to match your file structure layout
        int portArraySize = settings.beginReadArray("receiver_Ports_" + QString::number(i));
        QVector<QString> temp;

        for (int r = 0; r < portArraySize; ++r) {
            settings.setArrayIndex(r);
            // FIX: Match the actual inner value key written ("Port_Value")
            temp.append(settings.value("Port_Value", "0000").toString());
        }
        settings.endArray();

        // Sizing Fallback Strategy:
        // If the file was completely empty or newly created, fill it out to match the current station count
        if (temp.isEmpty()) {
            for (int s = 0; s < size; ++s) {
                temp.append("0000");
            }
        } else if (temp.size() < size) {
            // Pad out values if new stations were added globally since the last session
            while (temp.size() < size) {
                temp.append("0000");
            }
        } else if (temp.size() > size) {
            // Trim off trailing items if stations were deleted out of sequence
            temp.resize(size);
        }

        _receiverPorts->append(temp);
    }


    // qDebug() << *_receiverPorts;
}

void MainWindow::startAllUdpThreads() {
    // 1. Clean up active thread arrays and clear trailing memory pointer addresses safely
    stopAllUdpThreads();

    int stationCount = _stationName->size();
    int receiverCount = _receiverName->size();

    for (int i = 0; i < stationCount; ++i) {
        QString name = _stationName->at(i);
        quint16 port = _stationPort->at(i).toUShort();

        if (port == 0) continue;

        // 2. Prepare our strict routing list FIRST
        QList<QPair<QString, quint16>> checkedTargetsOnly;

        for (int j = 0; j < receiverCount; ++j) {
            if (j < _receiverPorts->size() && i < _receiverPorts->at(j).size()) {
                if (_receiverPorts->at(j).at(i) == "1") {
                    QString ip = _receiverIp->at(j);
                    quint16 destPort = port; // Match destination target port criteria
                    checkedTargetsOnly.append(qMakePair(ip, destPort));
                }
            }
        }

        // 3. Pass parameters directly into constructor while object is 100% on the main thread
        QThread *thread = new QThread(this);
        UdpStationWorker *worker = new UdpStationWorker(name, port, checkedTargetsOnly);
        worker->setObjectName(name);

        // 4. Move lifecycle tracking after initialization is finalized
        worker->moveToThread(thread);

        // 5. Standard thread lifecycle attachments
        connect(thread, &QThread::started, worker, &UdpStationWorker::startListening);
        connect(worker, &UdpStationWorker::finished, thread, &QThread::quit);

        // FIX: Let the thread's formal finish line act as the master cleanup trigger
        // to prevent mismatched pointer deletion cycles
        connect(thread, &QThread::finished, worker, &QObject::deleteLater);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);

        connect(worker, &UdpStationWorker::datagramReceived, this, &MainWindow::onUdpDataReceived);

        m_udpThreads.append(thread);
        m_udpWorkers.append(worker);

        thread->start();
    }
}



// Thread-safe slot that captures incoming data on the primary UI loop
void MainWindow::onUdpDataReceived(const QString &stationName, const QString &textData, const QString &senderIp) {
    // This runs completely on the Main Thread. You can directly interact with widgets.
    qDebug() << QString("[%1] Incoming from %2: %3").arg(stationName, senderIp, textData);

    // Example: Append directly to your UI display monitor
    // ui->textEdit_logs->append(QString("[%1]: %2").arg(stationName, textData));
}

// Clean destructor termination safety routines
void MainWindow::stopAllUdpThreads() {
    // Tell workers to stop listening
    for (UdpStationWorker* worker : m_udpWorkers) {
        if (worker) {
            QMetaObject::invokeMethod(worker, "stopListening", Qt::QueuedConnection);
        }
    }

    // Safely block and wait for system handles to release
    for (QThread* thread : m_udpThreads) {
        if (thread && thread->isRunning()) {
            thread->quit();
            thread->wait();
        }
    }

    // FIX: Wipe lists immediately so no dead reference pointers linger in RAM
    m_udpWorkers.clear();
    m_udpThreads.clear();
}


