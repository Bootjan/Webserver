/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bootjan <bootjan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 16:50:48 by tsteur            #+#    #+#             */
/*   Updated: 2024/02/20 22:35:59 by bootjan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logger.hpp"

bool	Logger::_enabled[Logger::LEVEL_COUNT] = {
	true,	// Error
	true,	// Warning
	true,	// Info
	false,	// Debug
	false,	// Response
};

std::ostream*	Logger::_streams[Logger::LEVEL_COUNT] = {
	&std::cerr,	// Error
	&std::cout,	// Warning
	&std::cout,	// Info
	&std::cout,	// Debug
	&std::cout,	// Response
};

const char*	Logger::_prefixes[Logger::LEVEL_COUNT] = {
	"\x1b[1;31m",	// Error
	"\x1b[33m",		// Warning
	"\x1b[37m",		// Info
	"\x1b[90m",		// Debug
	"\x1b[36m", 	// Response
};

const char*	Logger::_suffixes[Logger::LEVEL_COUNT] = {
	"\x1b[0m",	// Error
	"\x1b[0m",	// Warning
	"\x1b[0m",	// Info
	"\x1b[0m",	// Debug
	"\x1b[0m",	// Response
};
	
void	Logger::set_enabled_level(level_e level, bool enabled)
{
	_enabled[level] = enabled;
}

void	Logger::set_stream_level(level_e level, std::ostream* stream)
{
	_streams[level] = stream;
}

void	Logger::log(level_e level, std::string message) noexcept
{
	if (_enabled[level])
	{
		try
		{
			(*_streams[level]) << _prefixes[level] << message << _suffixes[level] << std::flush;
		}
		catch (const std::exception& e) {}
	}
}
