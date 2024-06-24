/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse_request.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/06 13:46:05 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/20 13:10:15 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseGenerator.hpp"

static void	parseFirstLine( request_t& convertedRequest, const std::string& line )
{
	std::string			subStr;
	std::stringstream	ss(line);
	std::getline(ss, subStr, ' ');
	convertedRequest.method = subStr;
	std::getline(ss, subStr, ' ');
	convertedRequest.url = subStr;
	std::getline(ss, subStr, ' ');
	size_t	end = subStr.find('\r');
	if (end == std::string::npos)
		end = subStr.size();
	convertedRequest.http_version = subStr.substr(0, end);
}

static void	insertLine( std::map<std::string, std::string>& header, const std::string& line )
{
	size_t	begin = line.find_first_of(':');
	if (begin == std::string::npos || begin + 2 > line.size())
		return ;
	std::string key;
	std::string value;

	key = line.substr(0, begin);
	value = line.substr(line.find_first_not_of("\t ", begin + 1));
	if (value[value.size() - 1] == '\r')
		value.resize(value.size() - 1);
	header.insert({ key, value});
}

request_t	parseRequest( const std::string& request )
{
	request_t			convertedRequest;
	std::string			line;
	std::stringstream	ss(request);
	
	std::getline(ss, line);
	parseFirstLine(convertedRequest, line);
	while (std::getline(ss, line) && line.size() > 1)
		insertLine(convertedRequest.header, line);
	
	while (std::getline(ss, line))
		convertedRequest.body += line + "\n";
	return convertedRequest;
}
