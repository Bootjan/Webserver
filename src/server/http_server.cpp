/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   http_server.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/01 15:10:12 by tsteur        #+#    #+#                 */
/*   Updated: 2024/02/23 15:33:20 by tsteur        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "http_server.hpp"
#include "ErrorResponse.hpp"

#define	POLL_TIMEOUT 50

HTTPServer::HTTPServer(const std::vector<uint16_t>& ports, size_t max_request_size): Server(ports)
{
	_request_callbacks.clear();
	_default_request_callbacks.clear();
	_accept_callback = std::bind(&HTTPServer::_accept_client, this, std::placeholders::_1, std::placeholders::_2);
	set_accept_callback(&_accept_callback);
	_max_request_size = max_request_size;
}

HTTPServer::~HTTPServer(void)
{
}

void	HTTPServer::add_request_callback(uint16_t port, std::string server_name, request_callback_t request_callback)
{
	if (_default_request_callbacks.count(port) == 0)
		_default_request_callbacks.insert({port, request_callback});
	if (_request_callbacks.count(port) == 0)
		_request_callbacks.insert({port, std::map<std::string, request_callback_t>()});
	_request_callbacks[port].insert({server_name, request_callback});
}

static std::string	recv_full_request(Server::Client* client)
{
	std::string	request = "";
	char		buffer[1024];
	size_t		bytes_recved;

	while (true)
	{
		bytes_recved = client->recv_from_socket(buffer, sizeof(buffer) - 1);
		buffer[bytes_recved] = '\0';
		request.append(buffer, bytes_recved);
		if (bytes_recved < sizeof(buffer) - 1)
			break;
	}
	
	return (request);
}

void	HTTPServer::_accept_client(Server::Client* client, uint16_t port)
{
	std::string			response;
	std::string			request_string;
	request_callback_t*	callback;
	request_t			request;
	size_t				content_length;
	
	Logger::log(Logger::DEBUG, "HTTPServer -> request recieved\n");

	try
	{
		try
		{
			client->poll_socket(POLLIN, POLL_TIMEOUT);
			request_string = recv_full_request(client);
			request = parseRequest(request_string);
			if (request.header.count("Content-Length") == 1)
			{
				content_length = std::stoul(request.header["Content-Length"]);
				while (content_length > request_string.size())
				{
					client->poll_socket(POLLIN, POLL_TIMEOUT);
					request_string += recv_full_request(client);
				}
				request = parseRequest(request_string);
			}
			if (_max_request_size > 0 && request.body.size() > _max_request_size)
			{
				response = ErrorResponse::generateResponse(413);
				client->poll_socket(POLLOUT, POLL_TIMEOUT);
				client->send_to_socket(response.c_str(), response.size());
				delete client;
				return ;
			}
			if (request.header.count("Host") == 1)
				callback = _choose_request_callback(port, request.header["Host"]);
			else
				callback = _choose_request_callback(port, "");
			if (callback == NULL)
			{
				Logger::log(Logger::WARNING, (std::string)"HTTPServer -> missing request callback \n");
				response = ErrorResponse::generateResponse(501);
			}
			else
				response = (*callback)(request);
		}
		catch (const ResponseException& e)
		{
			Logger::log(Logger::WARNING, (std::string)"Warning: " + e.what() + (std::string)"\n");
			response = ErrorResponse::generateResponse(e.responseCode);
		}
		
		client->poll_socket(POLLOUT, POLL_TIMEOUT);
		client->send_to_socket(response.c_str(), response.size());
	}
	catch (const std::exception& e)
	{
		Logger::log(Logger::ERROR, (std::string)"HTTPServer -> error while handling request: " + e.what() + "\n");
		try
		{
			response = ErrorResponse::generateResponse(500);
			client->poll_socket(POLLOUT, POLL_TIMEOUT);
			client->send_to_socket(response.c_str(), response.size());
		}
		catch(const std::exception& e)
		{
			(void) e;
		}
	}
	
	delete client;
}

HTTPServer::request_callback_t*	HTTPServer::_choose_request_callback(uint16_t port, std::string server_name)
{
	if (_request_callbacks.count(port) == 1 && _request_callbacks[port].count(server_name) == 1)
		return (&(_request_callbacks[port][server_name]));
	if (_default_request_callbacks.count(port) == 1)
		return (&(_default_request_callbacks[port]));
	return (NULL);
}