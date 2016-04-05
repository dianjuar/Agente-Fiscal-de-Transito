#include "MultiPropuseSockets.h"

using namespace network;


Base::DataSend::DataSend(QTcpSocket *socket)
{
    this->socket = socket;
}

void Base::DataSend::write(QString s)
{
    //socket->write( IntToArray(s.toUtf8().size()) ); //write size of data
    socket->write(s.toUtf8() ); //write the data itself
    socket->waitForBytesWritten();

    /*qDebug()<<s;
    socket->write( s.toUtf8() );
    socket->flush();*/
}

QByteArray Base::DataSend::IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}
/////////////////////////////////////////////////////////
Base::DataReceiver::DataReceiver(QTcpSocket *socket)
{
    this->socket = socket;
}

void Base::DataReceiver::readyRead()
{
    #if vervose
        qDebug()<<"";
        qDebug()<<("*********Reading**********");
    #endif

    QString s( socket->readAll() );

    #if vervose
        qDebug()<<s;
        qDebug()<<("********Analizing***********");
    #endif

    dataAnalizer(s);
}
/////////////////////////////////////////////////////////
Client::Client(QString host, int port):
    Base::DataSend(&socket),
    Base::DataReceiver(&socket)
{
    connected_B = false;
    this->host = host;
    this->port = port;
}

void Client::connectToHost()
{
    connect(&socket, SIGNAL(connected()), this, SLOT(connected())); //necesary to show alerts if connected.

    socket.connectToHost(host, port);

    connected_B = socket.waitForConnected(3000);

    if(connected_B)//connected
    {
        connect(&socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        connect(&socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
        connect(&socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    }
    else
    {
        disconnect(&socket,SIGNAL(connected()),this,SLOT(connected())); //if not connected disconnect the signal
        qDebug()<<"ERROR connecting to-> "<<host<<":"<<port;
    }
}

void Client::connected()
{
    qDebug()<<"CONECTED to -> "<<host<<":"<<port;
}

void Client::disconnected()
{
     qDebug()<<"DISCONECTED to -> "<<host<<":"<<port;
}

void Client::bytesWritten(qint64 bytes)
{
    #if vervose
        qDebug()<<"Bytes written"<<bytes;
    #endif
}
/////////////////////////////////////////////////////////////////////////
ServerSimple::ServerSimple(QHostAddress adds, int port)
{
    server = new QTcpServer(this);
    connect(server,SIGNAL(newConnection()), this, SLOT(newConnection()) );
    bool connected_B = false;

    if( !server->listen(adds,port) )
    {
        qDebug()<<"Server couldn't start";
    }
    else
    {
       qDebug()<<"Server started. Listening petitions at"<<adds<<":"<<port;
    }
}

void ServerSimple::newConnection()
{
    socket = server->nextPendingConnection();

    connected_B = true;

    set_socket_DR(socket);
    set_socket_DS(socket);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    emit newIncomingConnection();
}

void ServerSimple::connected()
{
    qDebug()<<"CONECTED to -> "<<socket->localAddress()<<":"<<socket->peerPort();
}

void ServerSimple::disconnected()
{
     qDebug()<<"DISCONECTED to -> "<<socket->localAddress()<<":"<<socket->peerPort();
}

void ServerSimple::bytesWritten(qint64 bytes)
{
    #if vervose
        qDebug()<<"Bytes written"<<bytes<<"in"<<socket->localAddress()<<":"<<socket->peerPort();
    #endif
}
