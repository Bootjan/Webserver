/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ExecuteResponse.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/02 14:48:22 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/21 18:05:22 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseGenerator.hpp"

void	ResponseGenerator::executeGet( void )
{
	if (is_directory(_directory + _filename))
	{
		if (_dirlisting == false)
			throw ForbiddenAction("directory listing", 403);
		listDirectory();
		return;
	}
	checkPermissionForFile();
	int	fd;
	if (_isCgi)
		fd = runCgi();
	else
	{
		fetchContentType();
		fd = openFile();
	}
	readFile(fd);
	close(fd);
}

void	ResponseGenerator::executePost( void )
{
	int	fd;
	if (_isCgi)
	{
		checkPermissionForFile();
		fd = runCgi();
		readFile(fd);
	}
	else
		fd = writeToFile();
	close(fd);
}

void	ResponseGenerator::executeDelete( void )
{
	if (access((_directory + _filename).c_str(), F_OK) == -1)
		return;
	if (std::remove((_directory + _filename).c_str()) == -1)
		throw SystemError("Error: remove failed", 500);
	_contentType = "text/html";
}
