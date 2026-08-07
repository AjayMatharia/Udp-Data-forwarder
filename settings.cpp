#include "settings.h"
#include "ui_settings.h"

#include <QCoreApplication>


Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);

    _configPath = QCoreApplication::applicationDirPath() + "/Settings/config.ini";


    _stationPort.clear();
    _stationName.clear();
    _receiverIp.clear();
    _receiverName.clear();
    _receiverPorts.clear();
    _receiverPortsbutton.clear();

    readSettings();
    qDebug()<<"_stationName"<<_stationName;
    qDebug()<<"_receiverName"<<_receiverName;
    qDebug()<<"_receiverPorts"<<_receiverPorts;


    QObject::connect(ui->_des_pushButton_station_info_addStation, &QPushButton::clicked, this, &Settings::addInfo);
    QObject::connect(ui->_des_pushButton_station_info_removeStation, &QPushButton::clicked, this, &Settings::removeInfo);
    QObject::connect(ui->_des_pushButton_receiver_info_addReceiver, &QPushButton::clicked, this, &Settings::addInfo);
    QObject::connect(ui->_des_pushButton_receiver_info_removeReceiver, &QPushButton::clicked, this, &Settings::removeInfo);
    QObject::connect(ui->_des_pushButton_save, &QPushButton::clicked, this, &Settings::saveSettings);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::addInfo()
{
    QObject* signalSender = sender();

    if (signalSender == ui->_des_pushButton_station_info_addStation) {
        int currentRow = ui->_des_tableWidget_station_info->currentRow();
        int targetRow = (currentRow != -1) ? (currentRow + 1) : ui->_des_tableWidget_station_info->rowCount();

        ui->_des_tableWidget_station_info->insertRow(targetRow);

        QTableWidgetItem *_tableWidgetItemstationname = new QTableWidgetItem("Station Name");
        QTableWidgetItem *_tableWidgetItemstationport = new QTableWidgetItem("0");
        _tableWidgetItemstationname->setTextAlignment(Qt::AlignCenter);
        _tableWidgetItemstationport->setTextAlignment(Qt::AlignCenter);
        ui->_des_tableWidget_station_info->setItem(targetRow, 0, _tableWidgetItemstationname);
        ui->_des_tableWidget_station_info->setItem(targetRow, 1, _tableWidgetItemstationport);

        // Fixed: Loop with auto reference (&) to directly modify the 2D matrix rows
        for (int i = 0; i < _receiverPorts.size(); ++i) {
            _receiverPorts[i].insert(targetRow, "0000");
        }

    } else if (signalSender == ui->_des_pushButton_receiver_info_addReceiver) {
        int currentRow = ui->_des_tableWidget_receiver_info->currentRow();
        int targetRow = (currentRow != -1) ? (currentRow + 1) : ui->_des_tableWidget_receiver_info->rowCount();

        ui->_des_tableWidget_receiver_info->insertRow(targetRow);

        QTableWidgetItem *_tableWidgetItemreceivername = new QTableWidgetItem("receiver Name");
        QTableWidgetItem *_tableWidgetItemreceiverip = new QTableWidgetItem("0.0.0.0");
        QPushButton *temp = new QPushButton("Ports");
        QObject::connect(temp, &QPushButton::clicked, this, &Settings::portSettings);

        _receiverPortsbutton.insert(targetRow, temp);
        _tableWidgetItemreceivername->setTextAlignment(Qt::AlignCenter);
        _tableWidgetItemreceiverip->setTextAlignment(Qt::AlignCenter);
        ui->_des_tableWidget_receiver_info->setItem(targetRow, 0, _tableWidgetItemreceivername);
        ui->_des_tableWidget_receiver_info->setItem(targetRow, 1, _tableWidgetItemreceiverip);
        ui->_des_tableWidget_receiver_info->setCellWidget(targetRow, 2, temp);

        QList<QString> temp2;
        for (int i = 0; i < ui->_des_tableWidget_station_info->rowCount(); ++i) {
            temp2.append("0000");
        }
        _receiverPorts.insert(targetRow, temp2);
    }
}

void Settings::removeInfo()
{
    QObject* signalSender = sender();

    if (signalSender == ui->_des_pushButton_station_info_removeStation) {
        int currentRow = ui->_des_tableWidget_station_info->currentRow();
        if (currentRow >= 0) {
            ui->_des_tableWidget_station_info->removeRow(currentRow);

            // Fixed: Loop with reference (&) to modify live matrix values
            for (int i = 0; i < _receiverPorts.size(); ++i) {
                if (currentRow < _receiverPorts[i].size()) {
                    _receiverPorts[i].removeAt(currentRow);
                }
            }
        }

    } else if (signalSender == ui->_des_pushButton_receiver_info_removeReceiver) {
        int currentRow = ui->_des_tableWidget_receiver_info->currentRow();
        if (currentRow >= 0) {
            ui->_des_tableWidget_receiver_info->removeRow(currentRow);

            if (currentRow < _receiverPorts.size()) {
                _receiverPorts.removeAt(currentRow);
            }
            if (currentRow < _receiverPortsbutton.size()) {
                // Safely delete widget reference from memory
                QPushButton* btn = _receiverPortsbutton.takeAt(currentRow);
                delete btn;
            }
        }
    }
}

void Settings::saveSettings()
{
    _stationName.clear();
    _stationPort.clear();
    _receiverName.clear();
    _receiverIp.clear();

    int rows = ui->_des_tableWidget_station_info->rowCount();
    int rows2 = ui->_des_tableWidget_receiver_info->rowCount();

    for (int r = 0; r < rows; ++r) {
        QTableWidgetItem* item0 = ui->_des_tableWidget_station_info->item(r, 0);
        QTableWidgetItem* item1 = ui->_des_tableWidget_station_info->item(r, 1);
        _stationName.append(item0 ? item0->text() : "SN");
        _stationPort.append(item1 ? item1->text() : "0");
    }

    for (int r = 0; r < rows2; ++r) {
        QTableWidgetItem* item0 = ui->_des_tableWidget_receiver_info->item(r, 0);
        QTableWidgetItem* item1 = ui->_des_tableWidget_receiver_info->item(r, 1);
        _receiverName.append(item0 ? item0->text() : "RN");
        _receiverIp.append(item1 ? item1->text() : "0.0.0.0");
    }

    // Ensure our internal storage matrix matches our updated table row boundaries
    // BEFORE clearing or executing size validation loops
    if (_receiverPorts.size() < rows2) {
        while (_receiverPorts.size() < rows2) {
            QList<QString> dummyPorts;
            for (int s = 0; s < rows; ++s) {
                dummyPorts.append("0000");
            }
            _receiverPorts.append(dummyPorts);
        }
    } else if (_receiverPorts.size() > rows2) {
        _receiverPorts.resize(rows2);
    }

    QSettings settings(_configPath, QSettings::IniFormat);
    settings.clear(); // Wipes clean to prevent junk trailing indexes

    // 1. Save Station Info Array
    settings.beginWriteArray("Stations_info");
    for (int r = 0; r < rows; ++r) {
        settings.setArrayIndex(r);
        settings.setValue("Station_Name", _stationName.at(r));
        settings.setValue("Port", _stationPort.at(r));
    }
    settings.endArray();

    // 2. Save Receiver Info Array
    settings.beginWriteArray("receiver_info");
    for (int r = 0; r < rows2; ++r) {
        settings.setArrayIndex(r);
        settings.setValue("receiver_Name", _receiverName.at(r));
        settings.setValue("IP_Address", _receiverIp.at(r));
    }
    settings.endArray();

    // 3. Multi-dimensional Matrix Mapping Serialization Sequence
    // FIX: Using uniform group name structure matching read settings
    for (int i = 0; i < rows2; ++i) {
        settings.beginWriteArray("receiver_Ports_" + QString::number(i));
        const QList<QString> &temp = _receiverPorts.at(i);

        for (int r = 0; r < temp.size(); ++r) {
            settings.setArrayIndex(r);
            settings.setValue("Port_Value", temp.at(r));
        }
        settings.endArray();
    }

    settings.sync(); // Force write immediate file changes to your disk layout
}

void Settings::readSettings() {
    _stationName.clear();
    _stationPort.clear();
    _receiverName.clear();
    _receiverIp.clear();
    _receiverPorts.clear();
    _receiverPortsbutton.clear();

    // --- Read Stations ---
    QSettings settings(_configPath, QSettings::IniFormat);
    int stationSize = settings.beginReadArray("Stations_info");
    ui->_des_tableWidget_station_info->setRowCount(stationSize);

    for (int i = 0; i < stationSize; ++i) {
        settings.setArrayIndex(i);
        // FIX: Removed trailing colons/spaces from keys to match saveSettings
        QString name = settings.value("Station_Name", "Default Name").toString();
        QString port = settings.value("Port", "0").toString();

        _stationName.append(name);
        _stationPort.append(port);

        QTableWidgetItem *itemText = new QTableWidgetItem(name);
        QTableWidgetItem *itemPort = new QTableWidgetItem(port);
        itemText->setTextAlignment(Qt::AlignCenter);
        itemPort->setTextAlignment(Qt::AlignCenter);

        ui->_des_tableWidget_station_info->setItem(i, 0, itemText);
        ui->_des_tableWidget_station_info->setItem(i, 1, itemPort);
    }
    settings.endArray();

    // --- Read Receivers ---
    int receiverSize = settings.beginReadArray("receiver_info");
    ui->_des_tableWidget_receiver_info->setRowCount(receiverSize);

    for (int i = 0; i < receiverSize; ++i) {
        settings.setArrayIndex(i);
        // FIX: Removed trailing colons/spaces from keys to match saveSettings
        QString name = settings.value("receiver_Name", "Receiver").toString();
        QString ip = settings.value("IP_Address", "0.0.0.0").toString();

        _receiverName.append(name);
        _receiverIp.append(ip);

        QTableWidgetItem *itemName = new QTableWidgetItem(name);
        QTableWidgetItem *itemIp = new QTableWidgetItem(ip);
        itemName->setTextAlignment(Qt::AlignCenter);
        itemIp->setTextAlignment(Qt::AlignCenter);

        ui->_des_tableWidget_receiver_info->setItem(i, 0, itemName);
        ui->_des_tableWidget_receiver_info->setItem(i, 1, itemIp);

        // Recreate the button inside the layout
        QPushButton *btnPorts = new QPushButton("Ports", this);
        QObject::connect(btnPorts, &QPushButton::clicked, this, &Settings::portSettings);
        ui->_des_tableWidget_receiver_info->setCellWidget(i, 2, btnPorts);
        _receiverPortsbutton.append(btnPorts);
    }
    settings.endArray();

    // --- Read Ports Matrix ---
    for (int i = 0; i < receiverSize; ++i) {
        QList<QString> savedPorts; // Match type to QList<QString> if header specifies QList

        // FIX: Added matching underscore "_" to group key name
        int savedPortsCount = settings.beginReadArray("receiver_Ports_" + QString::number(i));
        for (int r = 0; r < savedPortsCount; ++r) {
            settings.setArrayIndex(r);
            // FIX: Match the actual inner value key written ("Port_Value")
            savedPorts.append(settings.value("Port_Value", "0000").toString());
        }
        settings.endArray();

        // Fallback or sizing normalization step
        if (savedPorts.isEmpty()) {
            for (int s = 0; s < stationSize; ++s) {
                savedPorts.append("0000");
            }
        } else if (savedPorts.size() < stationSize) {
            // Pad if stations were added out of sequence globally
            while (savedPorts.size() < stationSize) {
                savedPorts.append("0000");
            }
        } else if (savedPorts.size() > stationSize) {
            // Trim down if rows were cleared out since the last cycle
            savedPorts.resize(stationSize);
        }
        _receiverPorts.append(savedPorts);
    }
}



void Settings::portSettings(){
    QObject* signalSender = sender();
    if (!signalSender) return;

    // Use a clean casting check to locate the button index
    QPushButton* clickedButton = qobject_cast<QPushButton*>(signalSender);
    indexvalue = _receiverPortsbutton.indexOf(clickedButton);
    if (indexvalue == -1) return;

    QDialog dialog(this);
    dialog.setWindowTitle("Inline Quick Configuration");
    dialog.setMinimumWidth(300);

    // 1. Initialize UI child widgets inline
    QTableWidget *portTable = new QTableWidget(&dialog);

    // Ensure table rows strictly match our internal memory configuration array size
    int currentPortCount = _receiverPorts.at(indexvalue).size();
    portTable->setRowCount(currentPortCount);
    portTable->setColumnCount(1);

    portTable->horizontalHeader()->setDefaultSectionSize(150);
    portTable->setHorizontalHeaderItem(0, new QTableWidgetItem("PORTS"));

    // 2. Populate the table items from your memory array
    for (int i = 0; i < currentPortCount; ++i) {
        QTableWidgetItem *itemPort = new QTableWidgetItem(_receiverPorts.at(indexvalue).at(i));
        itemPort->setTextAlignment(Qt::AlignCenter);
        portTable->setItem(i, 0, itemPort);
    }

    QPushButton *btnSave = new QPushButton("Save", &dialog);
    QPushButton *btnCancel = new QPushButton("Cancel", &dialog);

    // 3. Assemble layouts
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(btnCancel);
    buttonLayout->addWidget(btnSave);

    mainLayout->addWidget(portTable);
    mainLayout->addLayout(buttonLayout);

    // 4. Link signals to built-in slots
    connect(btnSave, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(btnCancel, &QPushButton::clicked, &dialog, &QDialog::reject);

    // 5. Open synchronously
    if (dialog.exec() == QDialog::Accepted) {

        // FIX: Extract a mutable reference using [] to write data back into the master matrix safely
        QList<QString> &activePortsRow = _receiverPorts[indexvalue];

        // Clear the older states ONCE before repopulating
        activePortsRow.clear();

        // 6. Loop over the table to extract edited text strings from the user
        for (int i = 0; i < portTable->rowCount(); ++i) {
            QTableWidgetItem *item = portTable->item(i, 0);

            // Defend against null items if a cell was left completely empty/blank by the user
            QString portText = (item) ? item->text().trimmed() : "0000";
            if (portText.isEmpty()) {
                portText = "0000";
            }

            // Push the updated text value straight into the matrix array
            activePortsRow.append(portText);
        }

    } else {
        qDebug() << "Inline layout generation cancelled.";
    }
}


