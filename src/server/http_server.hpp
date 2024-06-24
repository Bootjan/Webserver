/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   http_server.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/01 14:51:56 by tsteur        #+#    #+#                 */
/*   Updated: 2024/02/21 13:50:44 by tsteur        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_SERVER_HPP
# define HTTP_SERVER_HPP

# include <stdint.h>

# include <string>
# include <map>
# include <functional>

# include "logger.hpp"
# include "server.hpp"
# include "ResponseGenerator.hpp"

class HTTPServer : public Server
{
	public:
		typedef std::function<std::string( request_t )>	request_callback_t;
	
	private:
		using Server::set_accept_callback;
		using Server::accept_callback_t;

		accept_callback_t												_accept_callback;
		std::map<uint16_t, request_callback_t>							_default_request_callbacks;
		std::map<uint16_t, std::map<std::string, request_callback_t>>	_request_callbacks;
		size_t															_max_request_size;

		void				_accept_client(Server::Client* client, uint16_t port);
		request_callback_t*	_choose_request_callback(uint16_t port, std::string server_name);
	
	public:
						HTTPServer() = delete;
						HTTPServer(const std::vector<uint16_t>& ports, size_t max_request_size = 0);
						HTTPServer(const HTTPServer&) = delete;
						~HTTPServer(void);
					
		HTTPServer&		operator = (const HTTPServer&) = delete;

		void			add_request_callback(uint16_t port, std::string server_name, request_callback_t request_callback);
};

request_t	parseRequest( const std::string& request );

#endif