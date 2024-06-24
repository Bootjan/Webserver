/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/29 15:34:29 by tsteur        #+#    #+#                 */
/*   Updated: 2024/02/21 16:16:49 by tsteur        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <system_error>

#include "server.hpp"

static void	poll_fd(int fd, short events, int timeout)
{
	struct pollfd	fd_poll;
	int				status;

	fd_poll.fd = fd;
	fd_poll.events = events;
	Logger::log(Logger::DEBUG, "Server::Client -> waiting for socket to be available for I/O...\n");
	status = poll(&fd_poll, 1, timeout);
	if (status == -1)
		throw std::system_error(errno, std::generic_category());
	if (status == 0)
	{
		Logger::log(Logger::DEBUG, "Server::Client -> timeout while waiting for socket to be available\n");
		throw TimeoutExeption(408);
	}
}

const char*	Server::Client::TimeoutExeption::what() const noexcept
{
	return ("timeout");
}

Server::Client::Client(int socket)
{
	_socket = socket;
}

Server::Client::~Client(void)
{
	(void)close(_socket);
}

int	Server::Client::get_socket(void)
{
	return (_socket);
}

void	Server::Client::poll_socket(short events, int timeout)
{
	poll_fd(_socket, events, timeout);
}

size_t	Server::Client::send_to_socket(const void* buff, size_t size, int flags)
{
	ssize_t bytes_send;
	
	bytes_send = send(_socket, buff, size, flags);
	if (bytes_send == -1)
		throw std::system_error(errno, std::generic_category());
	return (bytes_send);
}

size_t	Server::Client::recv_from_socket(void* buff, size_t size, int flags)
{
	ssize_t bytes_received;
	
	bytes_received = recv(_socket, buff, size, flags);
	if (bytes_received == -1)
		throw std::system_error(errno, std::generic_category());
	return (bytes_received);
}


Server::Server(const std::vector<uint16_t>& ports)
{
	_sockets = std::vector<int>(ports.size());
	std::fill(_sockets.begin(), _sockets.end(), -1);
	for (unsigned int i = 0; i < ports.size(); i++)
	{
		_sockets[i] = _create_socket(ports[i]);
		_port_table.insert({_sockets[i], ports[i]});
	}
	
	_accept_callback = NULL;
}

Server::~Server(void)
{
	for (unsigned int i = 0; i < _sockets.size(); i++)
	{
		if (_sockets[i] != -1)
			(void)close(_sockets[i]);
	}
}

void	Server::set_accept_callback(accept_callback_t* accept_callback)
{
	_accept_callback = accept_callback;
}

int	Server::_create_socket(uint16_t port)
{
	int					server_fd;
	int					old_errno;
	int					opt;
	struct sockaddr_in	addr;
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		throw std::system_error(errno, std::generic_category());

	opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		old_errno = errno;
		(void)close(server_fd);
		throw std::system_error(old_errno, std::generic_category());
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		old_errno = errno;
		(void)close(server_fd);
		throw std::system_error(old_errno, std::generic_category());
	}
	
	return (server_fd);
}

int	Server::_select_socket(void)
{
	fd_set	set;
	int		max;
	
	max = -1;
	FD_ZERO(&set);
	for (unsigned int i = 0; i < _sockets.size(); i++)
	{
		FD_SET(_sockets[i], &set);
		if (_sockets[i] > max)
			max = _sockets[i];
	}

	Logger::log(Logger::DEBUG, "Server -> waiting for incomming connection...\n");

	 if (select(max + 1, &set, NULL, NULL, NULL) == -1)
		throw std::system_error(errno, std::generic_category());
	
	for (unsigned int i = 0; i < _sockets.size(); i++)
	{
		if (FD_ISSET(_sockets[i], &set))
			return (_sockets[i]);
	}
	return (-1);
}

void	Server::start(int listen_backlog)
{
	int	selected_socket;
	int	client_socket;
	struct sockaddr_in	addr;
	socklen_t			addr_len;
	Client* 	client;
	
	_running = true;
	
	for (unsigned int i = 0; i < _sockets.size(); i++)
	{
		poll_fd(_sockets[i], POLLOUT, 10);
		if (listen(_sockets[i], listen_backlog) == -1)
			throw std::system_error(errno, std::generic_category());
	}

	Logger::log(Logger::DEBUG, "Server -> listening...\n");

	while (_running)
	{
		selected_socket = _select_socket();
		client_socket = accept(selected_socket, (struct sockaddr *)&addr, &addr_len);
		if (client_socket == -1)
			throw std::system_error(errno, std::generic_category());
		Logger::log(Logger::DEBUG, "Server -> connection accepted\n");
		client = new Client(client_socket);
		if (_accept_callback == NULL)
			delete client;
		else
			(*_accept_callback)(client, _port_table[selected_socket]);
	}
}

void	Server::stop(void)
{
	_running = false;
}