/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ErrorResponse.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: bschaafs <bschaafs@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/21 14:31:49 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/22 15:09:44 by tsteur        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ErrorResponse.hpp"

std::map<size_t, std::string>	ErrorResponse::_errorPages;

std::string	ErrorResponse::errorMessage( size_t error_code )
{
	switch(error_code)
	{
		case 400: return "BAD_REQUEST";
		case 403: return "FORBIDDEN";
		case 404: return "PAGE_NOT_FOUND";
		case 405: return "INVALID_METHOD";
		case 408: return "REQ_TIMEOUT";
		case 413: return "LARGE_PAYLOAD";
		case 415: return "INVALID_MEDIA_TYPE";
		case 500: return "INTERNAL_SERVER_ERR";
		case 501: return "NOT_IMPLEMENTED";
		case 505: return "HTTP_NOT_SUPP";
		default: return "WHAT_IS_GOING_ON_TYCHO";
	}
}

std::string	ErrorResponse::errorBody( size_t error_code )
{
	if (_errorPages.find(error_code) != _errorPages.end())
		return _errorPages[error_code];
	
	switch(error_code)
	{
		case 400: return ERROR_400;
		case 403: return ERROR_403;
		case 404: return ERROR_404;
		case 405: return ERROR_405;
		case 408: return ERROR_408;
		case 413: return ERROR_413;
		case 415: return ERROR_415;
		case 500: return ERROR_500;
		case 501: return ERROR_501;
		case 505: return ERROR_505;
		default: return ERROR_42;
	}
}

std::string	read_file( const std::string& filename )
{
	std::ifstream	ss(filename);
	std::string		file_content, sub_str;

	if (!ss.is_open())
		throw std::runtime_error("Error: ifstream failed");
	while (std::getline(ss, sub_str))
		file_content += sub_str + "\n";
	ss.close();
	return file_content;
}

void	ErrorResponse::setErrorPages( config_t config )
{
	std::string	file_content;
	for (auto el : config)
	{
		if (access(el.second[0].value.c_str(), R_OK) != 0)
			continue;
		file_content = read_file(el.second[0].value.c_str());
		_errorPages[std::stoi(el.first)] = file_content;
	}
}

std::string	ErrorResponse::generateResponse( size_t error_code )
{
	std::string	response;

	response += "HTTP/1.1 ";
	response += std::to_string(error_code) + " ";
	response += errorMessage(error_code) + "\n";
	response += "Content-Type: text/html\n\n";
	response += errorBody(error_code);
	return response;
}
