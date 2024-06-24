/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bootjan <bootjan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:41:43 by tsteur            #+#    #+#             */
/*   Updated: 2024/02/20 22:30:19 by bootjan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
# define LOGGER_HPP

# include <ostream>
# include <string>
# include <iostream>
# include <exception>

class Logger
{
	public:
		enum	level_e
		{
			ERROR = 0,
			WARNING,
			INFO,
			DEBUG,
			RESPONSE,
			LEVEL_COUNT,
		};
	
	private:
		static bool				_enabled[LEVEL_COUNT];
		static std::ostream*	_streams[LEVEL_COUNT];
		static const char*		_prefixes[LEVEL_COUNT];
		static const char*		_suffixes[LEVEL_COUNT];

				Logger(void) = delete;
				Logger(const Logger&) = delete;
				~Logger(void) = delete;

		Logger&	operator = (const Logger&) = delete;
	
	public:
		static void	set_enabled_level(level_e level, bool enabled);
		static void	set_stream_level(level_e level, std::ostream* stream);
		static void	log(level_e level, std::string message) noexcept;
};

#endif
