#pragma once

enum class ConnectionStatus
{
	RequestingConnection,
	WaitingForReply,
	WaitingForConnections,
	Replying,
	ConnectionEstablished,
};