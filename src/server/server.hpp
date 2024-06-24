/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/29 15:38:57 by tsteur        #+#    #+#                 */
/*   Updated: 2024/02/21 12:32:04 by tsteur        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <stdint.h>
# include <stddef.h>
# include <functional>

# include <sys/socket.h>
# include <sys/types.h>
# include <sys/select.h>
# include <unistd.h>
# include <netinet/in.h>
# include <errno.h>
# include <poll.h>

# include <stdexcept>
# include <vector>
# include <map>

# include "ResponseGenerator.hpp"
# include "logger.hpp"

class Server
{
	public:
		class Client
		{
			private:
				int	_socket;
			
			public:
				class TimeoutExeption : public std::exception
				{
					public:
						const char*	what() const noexcept;
				};
				
						Client() = delete;
						Client(int socket);
						Client(const Client&) = delete;
						~Client(void);

				Client&	operator = (const Client&) = delete;

				int		get_socket(void);

				void	poll_socket(short events, int timeout = -1);
				size_t	send_to_socket(const void* buff, size_t size, int flags = 0);
				size_t	recv_from_socket(void* buff, size_t size, int flags = 0);
		};
		
		typedef	std::function<void(Client*, uint16_t port)>	accept_callback_t;
	
	private:
		std::vector<int>		_sockets;
		std::map<int, uint16_t>	_port_table;
		bool					_running;
		accept_callback_t*		_accept_callback;

		int				_create_socket(uint16_t port);
		int				_select_socket(void);
	
	public:
						Server() = delete;
						Server(const std::vector<uint16_t>& ports);
						Server(const Server&) = delete;
		virtual			~Server(void);

		Server&			operator = (const Server&) = delete;

		void			set_accept_callback(accept_callback_t* accept_callback);

		void			start(int listen_backlog = 8);
		void			stop(void);
		
};

#endif