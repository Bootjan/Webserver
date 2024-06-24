/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ResponseGenerator.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/01 16:19:06 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/21 18:06:01 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseGenerator.hpp"

ResponseGenerator::~ResponseGenerator( void )
{
	free_2d_array(&_envp, FREE_2D);
}

__uint8_t	ResponseGenerator::setPermissions( config_t config )
{
	std::string	permission_str;
	uint8_t	permissions = 0;
	if (config.count("allow") == 1)
		permission_str = config["allow"][0].value;
	else
		return ALL_PERM;

	if (permission_str.find("GET") != std::string::npos)
		permissions |= GET_PERM;
	if (permission_str.find("POST") != std::string::npos)
		permissions |= POST_PERM;
	if (permission_str.find("DELETE") != std::string::npos)
		permissions |= DELETE_PERM;
	return permissions;
}

std::string	ResponseGenerator::setDefaultFile( config_t config )
{
	if (config.count("index") == 1)
		return config["index"][0].value;
	return "index.html";
}

std::string	ResponseGenerator::setAlias( config_t config )
{
	std::string	alias;
	if (config.count("alias") == 1)
		alias = config["alias"][0].value;
	return alias;
}

location_t	ResponseGenerator::setLocation( config_t el, const std::string& value )
{
	location_t	location;
	
	location.value = value;
	if (value == "/" || value == "./")
		location.value = "";
	location.permissions = setPermissions(el);
	location.defaultFile = setDefaultFile(el);
	location.alias = setAlias(el);
	return location;
}

void	ResponseGenerator::setInfo( char* const* envp, config_t config, ExtensionGetter* extension_getter )
{
	_envp = copy_2d_array(envp);
	if (!_envp)
		throw SystemError("Error: malloc failed", 500);
	_extensionGetter = extension_getter;
	if (config.count("root") == 1)
		_workingDir = config["root"][0].value;
	if (_workingDir == "./" || _workingDir == ".")
		_workingDir = "";
	if (config.count("listing") == 1 && config["listing"][0].value == "false")
		_dirlisting = false;
	else
		_dirlisting = true;
	if (config.count("location") == 0)
		return;
	for (Field el : config["location"])
		_locations.push_back(setLocation(el.childs, el.value));
}

std::string	ResponseGenerator::getDate( void ) const
{
	time_t			t = std::time(nullptr);
    tm* 			timestamp = std::localtime(&t);

	std::ostringstream	oss;
	oss << std::put_time(timestamp, "Date: %a, %d %b %Y %H:%M:%S CET\n");
	return oss.str();
}

void	ResponseGenerator::generateHeader( void )
{
	_header += _HTTPVersion + " ";
	_header += std::to_string(_responseCode) + " OK\n";
	_header += "Server: " + _hostname + "\n";
	_header += getDate();
	_header += "Content-Type: " + _contentType + "\n\n";
}

void	ResponseGenerator::generateFirstLine( void )
{
	_header += _HTTPVersion + " ";
	_header += std::to_string(_responseCode) + " OK\n";
}

void	ResponseGenerator::handleMethod( void )
{
	switch (_method)
	{
		case GET: executeGet(); break;
		case POST: executePost(); break;
		case DELETE: executeDelete(); break; 
		default: return ;
	}
}

void	ResponseGenerator::resetVariables( void )
{
	_HTTPVersion.clear();
	_filename.clear();
	_directory.clear();
	_extension.clear();
	_arguments.clear();
	_additionalPath.clear();
	_hostname.clear();
	_response.clear();
	_header.clear();
	_contentType.clear();
	_requestBody.clear();
	_responseCode = 200;
}

std::string	ResponseGenerator::generateResponse( request_t request )
{
	resetVariables();
	parseRequest(request);
	handleMethod();
	if (_isCgi == true)
		generateFirstLine();
	else
		generateHeader();
	Logger::log(Logger::RESPONSE, "--------------------------RESPONSE--------------------------\n" + _header + "\n");
	return _header + _response;
}
