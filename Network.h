#pragma once

#include "SFML/Network.hpp"

namespace TCPnetwork
{
	class TCP_Base
	{
	protected:
		sf::IpAddress localIP = sf::IpAddress::LocalHost;
		sf::IpAddress remoteIP = sf::IpAddress::None;
		sf::TcpSocket socket;
		sf::Packet packet;
	public:
		bool sendMessage(std::string& msg);
		bool receiveMessage(std::string& msg);
		virtual void setBlocking(bool);
		virtual void closeConnection();
		std::string getLocalIP();
		std::string getRemoteIP();
		virtual ~TCP_Base() = default;
	};

	class Server : public TCP_Base
	{
	private:
		sf::TcpListener listener;

	public:
		void setBlocking(bool) override;
		bool listenForConnections();
		void closeConnection() override;
	};

	class Client : public TCP_Base
	{
	private:

	public:
		bool connect(std::string ipAddres);
	};
}
