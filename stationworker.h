#ifndef STATIONWORKER_H
#define STATIONWORKER_H

#include <QObject>
#include <QUdpSocket>

class StationWorker : public QObject {
    Q_OBJECT
private:
    QString m_stationName;
    quint16 m_port;
    QUdpSocket *m_socket;

public:
    // Pass config info straight into the worker context
    StationWorker(const QString &name, quint16 port, QObject *parent = nullptr);

public slots:
    // This executes strictly inside the background thread loop
    void startListening();
    void stopListening();

private slots:
    void handleReadyRead();

signals:
    // Thread-safe signal to push raw string updates back to your Main View
    void dataReceived(const QString &stationName, const QByteArray &data);
    void finished();
};

#endif // STATIONWORKER_H
