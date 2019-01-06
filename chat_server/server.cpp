#include "server.h"

server::server(QObject *parent) : QObject(parent)
{


}
void server::startServer()
{
    allClient = new QVector<QTcpSocket*>;

    chatSerVer = new QTcpServer();  //create a class on the heap;
    if(chatSerVer != NULL && allClient != NULL)
    {
        chatSerVer->setMaxPendingConnections(10); //set max number of connect
        //if have new connect,emit newConnection(),
        connect(chatSerVer,SIGNAL(newConnection()),this,SLOT(NewClientConnection()));
        if(chatSerVer->listen(QHostAddress::Any,8001) )
        {
            qDebug()<<"Server has started.Listening to port 8001.";
        }
        else
        {
            qDebug()<<"Server failed to start,Error:" + chatSerVer->errorString();
        }
    }
}
void server::sendMessageToClient(QString message)
{
    if (allClient->size() > 0)
    {
        for (int i = 0; i < allClient->size(); i++)
        {
            if (allClient->at(i)->isOpen() && allClient->at(i)->isWritable())
            {
                allClient->at(i)->write(message.toUtf8());
            }
        }
    }
}
void server::NewClientConnection()
{
    QTcpSocket*  client = chatSerVer->nextPendingConnection();
    QString ipAddress = client->peerAddress().toString();
    int port = client->peerPort();

    connect(client,SIGNAL(disconnected()),this,SLOT(socketDisconnected()));
    connect(client,SIGNAL(readyRead()),this,SLOT(socketRedyRead()));
    connect(client,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(socketStateChanged(QAbstractSocket::SocketState)));

    allClient->push_back(client);
    qDebug()<<"Socket connected from " + ipAddress + ":"+QString::number(port);
}
void server::socketDisconnected()  //you can do something such as storage client data and information
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());  //return the OBject of sent signal;
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();
    qDebug()<<"Socket disconnected from " + socketIpAddress + ":" + QString::number(port);
}
void server::socketRedyRead()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();

    QString data = QString(client->readAll());

    qDebug()<<"Message:"+data+"("+socketIpAddress+":"+QString::number(port)+")";
    sendMessageToClient(data);
}
void server::socketStateChanged(QAbstractSocket::SocketState state)
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();
    QString desc;

    switch (state) {
    case QAbstractSocket::UnconnectedState:
            desc = "the socket is not connected.";
        break;
    case QAbstractSocket::HostLookupState:
            desc = "the socket is performing a host name lookup.";
        break;
    case QAbstractSocket::ConnectingState:
             desc = "the socket is has started establishing a connection.";
        break;
    case QAbstractSocket::ConnectedState:
            desc = "a connection is established.";
        break;
    case QAbstractSocket::BoundState:
            desc = "The socket is bound to an address and port.";
        break;
    case QAbstractSocket::ClosingState:
            desc = "The socket is about to close (data may still be waiting to be written).";
        break;
    case QAbstractSocket::ListeningState:
            desc = "For internal use only.";
        break;
    }
    qDebug()<<"Socket state changed (" + socketIpAddress + ":" + QString::number(port) + ")" + desc;
}
