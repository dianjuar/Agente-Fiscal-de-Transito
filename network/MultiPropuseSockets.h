#ifndef NETWORKING_H
#define NETWORKING_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QAbstractSocket>

#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QDataStream>

#define vervose 0

namespace network
{
    namespace Base
    {
        class DataReceiver: public QObject
        {
            Q_OBJECT

        private:
            QTcpSocket *socket;

            virtual void dataAnalizer(QString msj){}

        public slots:
            void readyRead();

        public:
            DataReceiver(QTcpSocket *socket);
            DataReceiver(){ }

            //setters
            void set_socket_DR( QTcpSocket *s ) { this->socket = s; }

            void run();
        };
        ////////////////////////////////////////////
        class DataSend
        {
        private:
            QTcpSocket *socket;

            QByteArray IntToArray(qint32 source);

        public:
            DataSend(QTcpSocket *socket);
            DataSend(){}

            //setters
            void set_socket_DS( QTcpSocket *s ) { this->socket = s; }

            void write(QString s);
        };
    }

    ////////////////////////////////////////////
    class Client: public network::Base::DataReceiver, public network::Base::DataSend
    {
        Q_OBJECT
    private:
       QTcpSocket socket;

    protected:
       QString host;
       int port;
       bool connected_B;

    public:
       Client(QString host, int port);
       void connectToHost();

       //getters
       bool isConnected()   { return connected_B; }
       QString get_host()   { return host; }
       int get_port()       { return port; }

       //setters
       void set_host( QString host ){ this->host = host; }

    signals:

    public slots:
       void	connected();
       void	disconnected();
       void	bytesWritten(qint64 bytes);
    };
    ///////////////////////////////////////////
    /// \brief The ServerSimple class
    /// Only works for one petition... for now.
    class ServerSimple: public network::Base::DataReceiver, public network::Base::DataSend
    {
        Q_OBJECT
    private:
       QTcpSocket *socket;
       QTcpServer *server;

    protected:
       QHostAddress adds;
       int port;
       bool connected_B;

    public:
       ServerSimple(QHostAddress adds, int port);

       //getters
       bool isConnected()        { return connected_B; }
       QHostAddress get_host()   { return adds; }
       int get_port()            { return port; }

    signals:
        void newIncomingConnection();

    public slots:
       void newConnection();
       void	connected();
       void	disconnected();
       void	bytesWritten(qint64 bytes);

    };

}

#endif // NETWORKING_H
