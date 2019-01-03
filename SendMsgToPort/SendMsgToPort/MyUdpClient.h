#ifndef MYUDPCLIENT_H
#define MYUDPCLIENT_H
#include "qudpsocket.h"

class MyUdpClient : public QUdpSocket
{
    Q_OBJECT
public:
    explicit MyUdpClient(QObject *parent = nullptr);
    ~MyUdpClient(void){}

public slots:
    void onDataReceived();

signals:
    void updateClients(QString&, QByteArray&);

private:
    QString m_sReceivedData;

};

#endif // MYUDPCLIENT_H
