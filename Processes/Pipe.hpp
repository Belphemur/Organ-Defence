/*
 *  Pipe.hpp
 *  
 *
 *  Created by apple on 20/03/10.
 *  Copyright 2010 __Balor__. All rights reserved.
 *
 */

#ifndef _PIPE_BALOR_HPP
#define	_PIPE_BALOR_HPP
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdexcept>
#include <errno.h>
#include <cstdio>
#include "../Network/Packet.hpp"
enum RW
{
	READ=0,
	WRITE=1
};
class Pipe
{
public:
	Pipe();
	int init();
	template <typename T>
	ssize_t read(T& info)
	{
		ssize_t amountRead;
		amountRead=::read(pipefd[READ],&info,sizeof(T));
		if(amountRead == -1)
		{
			if(block)
			{
				perror("Pipe read");
				throw std::runtime_error("PIPE: read error");
			}
			else if(errno!=EAGAIN)
			{
				perror("Pipe read");
				throw std::runtime_error("PIPE: read error");
			}

		}
		return amountRead;
	}
	template <typename T>
	ssize_t write(T info)
	{
		ssize_t amountWrite;
		amountWrite=::write(pipefd[WRITE], &info, sizeof(T));
		if(amountWrite == -1)
		{
			perror("Pipe write");
			throw std::runtime_error("PIPE : write error");
		}
		return amountWrite;
	}
	ssize_t readPacket(sf::Packet &PacketToReceive);
	ssize_t writePacket(sf::Packet PacketToSend);
	
	void close(RW dir);
	void deblock(RW dir);
	void clear();

private:
	bool block;
	int pipefd[2];
};
#endif
