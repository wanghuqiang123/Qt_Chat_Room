#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QVector>
#include <QString>

class server : public QObject
{
    Q_OBJECT
public:
    explicit server(QObject *parent = nullptr);
private:
    QTcpServer* chatSerVer;
    QVector<QTcpSocket*>* allClient;
public:
    void startServer();
    void sendMessageToClient(QString message);
public slots:
    void NewClientConnection();
    void socketDisconnected();
    void socketRedyRead();
    void socketStateChanged(QAbstractSocket::SocketState state);
};

#endif // SERVER_H
