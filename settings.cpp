#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);


    _configPath = QCoreApplication::applicationDirPath() + "/Settings/config.ini";
    _configPath2 = QCoreApplication::applicationDirPath() + "/Settings/configPort.ini";

    _stationPort = new QVector<QString>();
    _stationName = new QVector<QString>();
    _receiverIp = new QVector<QString>();
    _receiverName = new QVector<QString>();
    _receiverPorts = new QVector<QVector<QString>>();
    _receiverPortsbutton = new QVector<QPushButton *>();


    readSettings();



    // QList temp = {"a","b","c","d"};
    // qDebug()<<temp;
    // temp.removeAt(1);
    // qDebug()<<temp;

    QObject :: connect(ui->_des_pushButton_station_info_addStation ,&QPushButton::clicked, this, &Settings :: addIp);
    QObject :: connect(ui->_des_pushButton_station_info_removeStation ,&QPushButton::clicked, this, &Settings :: removeIp);
    QObject :: connect(ui->_des_pushButton_receiver_info_addReceiver ,&QPushButton::clicked, this, &Settings :: addIp);
    QObject :: connect(ui->_des_pushButton_receiver_info_removeReceiver ,&QPushButton::clicked, this, &Settings :: removeIp);
    QObject :: connect(ui->_des_pushButton_save ,&QPushButton::clicked, this, &Settings :: saveSettings);



}

Settings::~Settings()
{
    delete ui;
}

void Settings::addIp(){

    QObject* signalSender = sender();
    if (signalSender == ui->_des_pushButton_station_info_addStation) {
        // 1. Get the currently selected row index
        int currentRow = ui->_des_tableWidget_station_info->currentRow();

        // 2. Determine the insertion target index
        // If no row is selected (currentRow is -1), append to the end of the table
        int targetRow = (currentRow != -1) ? (currentRow + 1) : ui->_des_tableWidget_station_info->rowCount();

        // 3. Insert the blank row
        ui->_des_tableWidget_station_info->insertRow(targetRow);


        QTableWidgetItem *_tableWidgetItemstationname = new QTableWidgetItem("Station Name");
        QTableWidgetItem *_tableWidgetItemstationport = new QTableWidgetItem("0");
        _tableWidgetItemstationname->setTextAlignment(Qt::AlignCenter);
        _tableWidgetItemstationport->setTextAlignment(Qt::AlignCenter);
        ui->_des_tableWidget_station_info->setItem(targetRow, 0, _tableWidgetItemstationname);
        ui->_des_tableWidget_station_info->setItem(targetRow, 1, _tableWidgetItemstationport);

    } else if (signalSender == ui->_des_pushButton_receiver_info_addReceiver) {
        // 1. Get the currently selected row index
        int currentRow = ui->_des_tableWidget_receiver_info->currentRow();

        // 2. Determine the insertion target index
        // If no row is selected (currentRow is -1), append to the end of the table
        int targetRow = (currentRow != -1) ? (currentRow + 1) : ui->_des_tableWidget_receiver_info->rowCount();

        // 3. Insert the blank row
        ui->_des_tableWidget_receiver_info->insertRow(targetRow);

        // ui->_des_tableWidget_station_info->insertRow(ui->_des_tableWidget_station_info->rowCount());
        // ui->_des_tableWidget_station_info->setRowCount(ui->_des_tableWidget_station_info->rowCount()+1);
        QTableWidgetItem *_tableWidgetItemreceivername = new QTableWidgetItem("receiver Name");
        QTableWidgetItem *_tableWidgetItemreceiverip = new QTableWidgetItem("0.0.0.0");
        QPushButton *temp = new QPushButton("Ports");
        QObject :: connect(temp ,&QPushButton::clicked, this, &Settings :: portSettings);
        _receiverPortsbutton->insert(targetRow,temp);
        _tableWidgetItemreceivername->setTextAlignment(Qt::AlignCenter);
        _tableWidgetItemreceiverip->setTextAlignment(Qt::AlignCenter);
        ui->_des_tableWidget_receiver_info->setItem(targetRow, 0, _tableWidgetItemreceivername);
        ui->_des_tableWidget_receiver_info->setItem(targetRow, 1, _tableWidgetItemreceiverip);
        ui->_des_tableWidget_receiver_info->setCellWidget(targetRow, 2,temp);

        QVector<QString> temp2;

        for (int i = 0; i < ui->_des_tableWidget_station_info->rowCount(); ++i) {

            temp2.append("0000");
        }
        _receiverPorts->insert(targetRow,temp2);
    }


}
void Settings::removeIp(){
    QObject* signalSender = sender();
    if (signalSender == ui->_des_pushButton_station_info_removeStation) {
        int currentRow = ui->_des_tableWidget_station_info->currentRow();
        if (currentRow >= 0) {
            ui->_des_tableWidget_station_info->removeRow(currentRow);
        }
    } else if (signalSender == ui->_des_pushButton_receiver_info_removeReceiver) {
        int currentRow = ui->_des_tableWidget_receiver_info->currentRow();
        if (currentRow >= 0) {
            ui->_des_tableWidget_receiver_info->removeRow(currentRow);

            // delete _receiverPorts->at(currentRow);
            QPushButton *temp =  _receiverPortsbutton->at(currentRow);
            _receiverPortsbutton->removeAt(currentRow);
            delete temp;
        }
    }

}
void Settings::saveSettings(){

    _stationName->clear();
    _stationPort->clear();
    _receiverName->clear();
    _receiverIp->clear();
    // _receiverPorts->clear();
    // Get row and column counts

    int rows = ui->_des_tableWidget_station_info->rowCount();
    int rows2 = ui->_des_tableWidget_receiver_info->rowCount();

    // int cols = ui->_des_tableWidget_station_info->columnCount();

    // Loop through every row

    for (int r = 0; r < rows; ++r) {
        _stationName->append(ui->_des_tableWidget_station_info->item(r, 0)->text());
        _stationPort->append(ui->_des_tableWidget_station_info->item(r, 1)->text());

    }
    for (int r = 0; r < rows2; ++r) {
        _receiverName->append(ui->_des_tableWidget_receiver_info->item(r, 0)->text());
        _receiverIp->append(ui->_des_tableWidget_receiver_info->item(r, 1)->text());

    }


    // 1. Specify the custom INI path and set the format


    QSettings settings(_configPath, QSettings::IniFormat);
    settings.clear();


    //Saving Station Info
    settings.beginWriteArray("Stations_info");


    for (int r = 0; r < rows; ++r) {
        settings.setArrayIndex(r);
        settings.setValue("Station_Name: ", _stationName->at(r));
        settings.setValue("Port: ", _stationPort->at(r));

    }

    // 3. Save values inside the group

    // 4. Close the group
    settings.endArray();


    //Saving Reviever Info

    settings.beginWriteArray("receiver_info");


    for (int r = 0; r < rows2; ++r) {
        settings.setArrayIndex(r);
        settings.setValue("receiver_Name: ", _receiverName->at(r));
        settings.setValue("IP_Address: ", _receiverIp->at(r));

    }

    // 3. Save values inside the group

    // 4. Close the group
    settings.endArray();


    // QSettings settings2(_configPath2, QSettings::IniFormat);
    // settings2.clear();
    // for (int var = 0; var < total; ++var) {

    // }
    // _receiverPorts->clear();

    // for (int i = 0; i < rows2; ++i) {
    //     QVector<QString> temp;
    //     for (int i = 0; i < rows; ++i) {
    //         temp.append("0000");
    //     }
    //     _receiverPorts->append(temp);
    // }
    _receiverPorts->resize(rows2);


    QSettings settings2(_configPath2 , QSettings::IniFormat);
    settings2.clear();

    for (int i = 0; i < rows2; ++i) {
        settings2.beginWriteArray("receiver_Ports"+ QString::number(i));
        auto temp = _receiverPorts->at(i);


        for (int r = 0; r < temp.size(); ++r) {
            settings2.setArrayIndex(r);
            settings2.setValue("receiver_Port: ", temp.at(r));

        }

        settings2.endArray();


    }

}
void Settings::readSettings(){
    _receiverPorts->clear();
    QSettings settings(_configPath, QSettings::IniFormat);

    int size = settings.beginReadArray("Stations_info");

    ui->_des_tableWidget_station_info->setRowCount(size);

    for (int i = 0; i < size; ++i) {
        // Set the current index context
        settings.setArrayIndex(i);

        _stationName->append(settings.value("Station_Name: ", "Default Name").toString());
        _stationPort->append(settings.value("Port: ", "0000").toString());

        QTableWidgetItem *_tableWidgetItemstationname = new QTableWidgetItem(_stationName->at(i));
        QTableWidgetItem *_tableWidgetItemstationport = new QTableWidgetItem(_stationPort->at(i));
        _tableWidgetItemstationname->setTextAlignment(Qt::AlignCenter);
        _tableWidgetItemstationport->setTextAlignment(Qt::AlignCenter);

        ui->_des_tableWidget_station_info->setItem(i, 0, _tableWidgetItemstationname);
        ui->_des_tableWidget_station_info->setItem(i, 1, _tableWidgetItemstationport);
    }

    settings.endArray();

    int size2 = settings.beginReadArray("receiver_info");
    ui->_des_tableWidget_receiver_info->setRowCount(size2);

    for (int i = 0; i < size; ++i) {
        // Set the current index context
        settings.setArrayIndex(i);

        _receiverName->append(settings.value("receiver_Name: ", "Default Name").toString());
        _receiverIp->append(settings.value("IP_Address: ", "0.0.0.0").toString());


        QTableWidgetItem *_tableWidgetItemreceivername = new QTableWidgetItem(_receiverName->at(i));
        QTableWidgetItem *_tableWidgetItemreceiverip = new QTableWidgetItem(_receiverIp->at(i));
        QPushButton *temp = new QPushButton("Ports");
        QObject :: connect(temp ,&QPushButton::clicked, this, &Settings :: portSettings);
        _receiverPortsbutton->insert(i,temp);
        _tableWidgetItemreceivername->setTextAlignment(Qt::AlignCenter);
        _tableWidgetItemreceiverip->setTextAlignment(Qt::AlignCenter);
        ui->_des_tableWidget_receiver_info->setItem(i, 0, _tableWidgetItemreceivername);
        ui->_des_tableWidget_receiver_info->setItem(i, 1, _tableWidgetItemreceiverip);
        ui->_des_tableWidget_receiver_info->setCellWidget(i, 2,temp);


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


    // QSettings settings2(_configPath2, QSettings::IniFormat);

    // for (int i = 0; i < size2; ++i) {
    //     int size = settings2.beginReadArray("receiver_Ports"+ QString::number(i));
    //     QVector<QString> temp;
    //     temp.clear();
    //     for (int r = 0; r < size; ++r) {
    //         settings2.setArrayIndex(i);
    //         temp.append(settings2.value("receiver_Port","0000").toString());
    //     }

    //     qDebug()<<temp;

    //     _receiverPorts->append(temp);
    //     settings2.endArray();

    // }







}

void Settings::portSettings(){
    QObject* signalSender = sender();
    // qDebug()<<this;

    indexvalue=_receiverPortsbutton->indexOf(signalSender);
    // qDebug()<<_receiverPorts->at(_receiverPortsbutton->indexOf(signalSender));
    SettingsDialog dlg(this);
    QObject::connect(&dlg, &SettingsDialog::sendData, this, &Settings::receiveData);
    dlg.setData(_receiverPorts->at(_receiverPortsbutton->indexOf(signalSender)));
    dlg.exec();
    QObject::disconnect(&dlg, &SettingsDialog::sendData, this, &Settings::receiveData);

}
void Settings::receiveData(QVector<QString> temp){

    qDebug()<<*_receiverPorts;

    _receiverPorts->insert(indexvalue,temp);
    qDebug()<<*_receiverPorts;

}
