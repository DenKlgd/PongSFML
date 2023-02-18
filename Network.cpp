#include "Network.h"

using namespace TCPnetwork;

bool TCP_Base::sendMessage(std::string& msg)
{
	packet.clear();
	packet << msg;
	if (socket.send(packet) != sf::Socket::Status::Done)
		return false;
	return true;
}

bool TCP_Base::receiveMessage(std::string& msg)
{
	packet.clear();
	if (socket.receive(packet) != sf::Socket::Status::Done)
		return false;

	packet >> msg;
	return true;
}

void TCP_Base::setBlocking(bool blocking)
{
	socket.setBlocking(blocking);
}

void TCP_Base::closeConnection()
{
	socket.disconnect();
	remoteIP = sf::IpAddress::None;
}

std::string TCP_Base::getLocalIP()
{
	return localIP.toString();
}

std::string TCP_Base::getRemoteIP()
{
	return remoteIP.toString();
}

///

void Server::setBlocking(bool blocking)
{
	listener.setBlocking(blocking);
	socket.setBlocking(blocking);
}

bool Server::listenForConnections()
{
	if (listener.listen(53000) != sf::Socket::Status::Done)
		return false;

	if (listener.accept(socket) != sf::Socket::Status::Done)
		return false;

	return true;
}

void Server::closeConnection()
{
	socket.disconnect();
	listener.close();
	remoteIP = sf::IpAddress::None;
}

///

bool Client::connect(std::string ipAddres)
{
	sf::IpAddress remoteIP = ipAddres;
	if (remoteIP == sf::IpAddress::None)
		return false;

	if (socket.connect(remoteIP, 53000) != sf::Socket::Done)
		return false;

	this->remoteIP = remoteIP;

	return true;
}
