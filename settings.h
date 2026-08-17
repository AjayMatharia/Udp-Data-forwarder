#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QVector2D>
#include <QVector>
#include <QMap>
#include <QSettings>
#include <QLineEdit>



namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private:
    Ui::Settings *ui;
    //2D vector
    QVector<QString> _stationPort;
    QVector<QString> _stationName;
    QVector<QString> _receiverIp;
    QVector<QString> _receiverName;
    QVector<QPushButton *> _receiverPortsbutton;
    QVector<QVector<QString>> _receiverPorts;

    QString _configPath;
    // QString _configPath2;
    int indexvalue;


    QMap<QString, QString> _stationIpMap;

public slots:
    void readSettings();
    void portSettings();
    void saveSettings();
    void addInfo();
    void removeInfo();


};

#endif // SETTINGS_H
