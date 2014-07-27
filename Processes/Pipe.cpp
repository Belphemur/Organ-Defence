/*
 *  Pipe.cpp
 *  
 *
 *  Created by apple on 20/03/10.
 *  Copyright 2010 __Balor__. All rights reserved.
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdexcept>
#include <string.h>
#include "../Network/Packet.hpp"
#include "Pipe.hpp"

Pipe::Pipe()
{
	pipefd[0]=0;
	pipefd[1]=0;	
	block=true;
}
int Pipe::init()
{
	return pipe(pipefd);
}
ssize_t Pipe::readPacket(sf::Packet &PacketToReceive)
{
	
	ssize_t amountRead=-1;
	sf::Uint32 PacketSize=0;
	std::vector<char> myPendingPacket;
	while(amountRead == -1)
	{
		amountRead=::read(pipefd[READ],&PacketSize,sizeof(PacketSize));
		usleep(1);
	}
	char Buffer[1024];
	while (myPendingPacket.size() < PacketSize)
	{
		// Receive a chunk of data
		std::size_t SizeToGet = std::min(static_cast<std::size_t>(PacketSize - myPendingPacket.size()), sizeof(Buffer));
		usleep(100);
		amountRead=::read(pipefd[READ],&Buffer,SizeToGet);
		// Append it into the packet
		if (amountRead > 0)
		{
			myPendingPacket.resize(myPendingPacket.size() + amountRead);
			char* Begin = &myPendingPacket[0] + myPendingPacket.size() - amountRead;
			memcpy(Begin, Buffer, amountRead);
		}
	}
	PacketToReceive.Clear();
	if (!myPendingPacket.empty())
		PacketToReceive.OnReceive(&myPendingPacket[0], myPendingPacket.size());
	
	return amountRead;
}

ssize_t Pipe::writePacket(sf::Packet PacketToSend)
{
	// Get the data to send from the packet
	size_t DataSize = 0;
	ssize_t amountWriten;
	const char* Data = PacketToSend.OnSend(DataSize);
	
	// Send the packet size
	sf::Uint32 PacketSize = static_cast<unsigned long>(DataSize);
	amountWriten=::write(pipefd[WRITE], &PacketSize, sizeof(PacketSize));
	if(amountWriten == -1)
	{
		perror("Pipe");
		throw std::runtime_error("PM : PACKET(phase 1) : write error");
	}
	if(amountWriten< (ssize_t)sizeof(PacketSize))
		throw std::runtime_error("PM : PACKET(phase 1) : Not everything written ! (PIPE)");
	// Send the packet data
	if (PacketSize > 0)
	{
		amountWriten=::write(pipefd[WRITE], Data, DataSize);
		if(amountWriten == -1)
			throw std::runtime_error("PM : PACKET(phase 2) : write error");
		if(amountWriten<(ssize_t)DataSize)
			throw std::runtime_error("PM : PACKET(phase 2) : Not everything written ! (PIPE)");
	}
	return amountWriten;
}

void Pipe::close(RW dir)
{
	::close(pipefd[dir]);
	pipefd[dir]=-1;
}
void Pipe::deblock(RW dir)
{
	::fcntl(pipefd[dir], F_SETFL, O_NONBLOCK);
	block=false;
}
void Pipe::clear()
{
	if(pipefd[READ]==-1)
		close(WRITE);
	else
		close(READ);
}
