/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ParseRequest.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/05 17:38:49 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/21 17:41:01 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseGenerator.hpp"

void	ResponseGenerator::setMethod( const std::string& subStr )
{
	std::string	methods[] = { "GET", "POST", "DELETE" };

	size_t	idx = 0;
	for (idx = 0; idx < sizeof(methods) / sizeof(methods[0]); idx++)
		if (subStr.substr(0, methods[idx].size()) == methods[idx])
			break;
	switch (idx)
	{
		case 0:	_method = GET; break;
		case 1:	_method = POST; break;
		case 2:	_method = DELETE; break;
		default: throw BadRequest(404); break;
	}
}

void	ResponseGenerator::setHTTPVersion( const std::string& version )
{
	_HTTPVersion = version;
	if (_HTTPVersion != "HTTP/1.1")
		throw InvalidHttpVersion("Error: HTTP version is not supported, " + _HTTPVersion, 505);
}

void	ResponseGenerator::setFilename( const std::string& url )
{
	_filename = _workingDir + url;
}

void	ResponseGenerator::setArguments( void )
{
	size_t	begin = _filename.find_first_of('?');
	if (begin == std::string::npos)
		return ;
	_arguments = _filename.substr(begin + 1, _filename.size());
	_filename = _filename.substr(0, begin);
}

void	ResponseGenerator::setDirectory( void )
{
	size_t	begin = _filename.find_last_of('/');
	if (begin == std::string::npos)
		throw IncorrectFormat(_filename, 400);
	_directory = _filename.substr(0, begin + 1);
	if (_directory == "/")
		_directory = "";
	if (contains_parent_dir(_directory) == true)
		throw IncorrectFormat(_directory, 400);
	_filename = _filename.substr(begin + 1, _filename.size());
	if (_filename.size() > 0)
		return;
	for (location_t location : _locations)
	{
		if (_directory == _workingDir + location.value + "/")
		{
			_filename = location.defaultFile;
			return;
		}
	}
	_filename = "index.html";
}

void	ResponseGenerator::setAdditionalPath( void )
{
	size_t	dot_idx = _filename.find_last_of('.');
	if (dot_idx == std::string::npos)
		return;
	std::string	temp = _filename.substr(dot_idx, _filename.size());
	size_t	slash_idx = temp.find_first_of('/');
	if (slash_idx == std::string::npos)
		return;
	if (!temp[slash_idx + 1])
		_filename = _filename.substr(0, _filename.size() - 1);
	else
	{
		_additionalPath = temp.substr(slash_idx + 1, _filename.size());
		_filename = _filename.substr(0, slash_idx + temp.size());
	}
}

void	ResponseGenerator::setExtension( void )
{
	size_t	begin = _filename.find_last_of('/');
	if (begin == std::string::npos)
		begin = 0;
	std::string	extension = _filename.substr(begin, _filename.size());
	begin = extension.find_first_of('.');
	if (begin != std::string::npos)
		_extension = extension.substr(begin + 1, extension.size());
}

void	ResponseGenerator::checkCgi( void )
{
	_isCgi = false;
	if (_directory.find(_workingDir + _cgiFolder) == 0)
		_isCgi = true;
}

void	ResponseGenerator::setHostname( std::map<std::string, std::string> dict )
{
	try
	{
		_hostname = dict["Host"];
	}
	catch(const std::exception& e) {}
}

bool	ResponseGenerator::validPermissions( __uint8_t permissions )
{
	if (_method == GET && (permissions & GET_PERM) == 0)
		return false;
	if (_method == POST && (permissions & POST_PERM) == 0)
		return false;
	if (_method == DELETE && (permissions & DELETE_PERM) == 0)
		return false;
	return true;
}

void	ResponseGenerator::checkPermissionForDir( void )
{
	for (location_t location : _locations)
		if (_directory.find(_workingDir + location.value + "/") == 0 && validPermissions(location.permissions) == false)
			throw UnallowedMethod(_workingDir + location.value, 405);
}

void	ResponseGenerator::checkAlias( void )
{
	std::string	tail;
	for (location_t location : _locations)
	{
		if (location.alias.size() != 0 && location.alias != "/" && _directory.find(_workingDir + location.alias + "/") == 0)
		{
			tail = _directory.substr(_workingDir.size() + location.alias.size() + 1, _directory.size());
			_directory = _workingDir + location.value + tail + "/";
			return ;
		}
	}
}

void	ResponseGenerator::parseRequest( request_t request )
{
	setMethod(request.method);
	setHTTPVersion(request.http_version);
	setFilename(request.url);
	setArguments();
	setAdditionalPath();
	setDirectory();
	setExtension();
	checkPermissionForDir();
	checkAlias();
	checkCgi();
	setHostname(request.header);
	_requestBody = request.body;
}
