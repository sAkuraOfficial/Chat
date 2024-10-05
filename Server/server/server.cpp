#include "server.h"

Server::Server(QObject *parent)
    : QObject(parent)
{
    m_pDatabase = new Database(this);
    m_pProtocol = new Protocol(this);
}
Server::~Server()
{
}

bool Server::runServer(int port)
{
    bool isProtocolListening = m_pProtocol->listen(port);
    if (!isProtocolListening)
    {
        
        return false;
    }
    bool isDatabaseOpen = m_pDatabase->openDatabase();
}

Protocol *Server::getProtocol()
{
    return m_pProtocol;
}

Database *Server::getDatabase()
{
    return m_pDatabase;
}