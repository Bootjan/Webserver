/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HandlingFiles.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: bschaafs <bschaafs@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/19 16:35:39 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/23 15:30:28 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseGenerator.hpp"

#define	POLL_TIMEOUT 50

void	ResponseGenerator::fetchContentType( void )
{
	_contentType = _extensionGetter->getContentType(_extension);
}

void	ResponseGenerator::checkPermissionForFile( void )
{
	std::string	permission_check = _directory + _filename;
	if (access(permission_check.c_str(), R_OK) == -1)
		throw UnreadableFile(permission_check, 404);
}

void	ResponseGenerator::pollFd( int fd, nfds_t nfds, int timeout )
{
	struct pollfd	fd_info;
	int				status;

	fd_info.fd = fd;
	fd_info.events = nfds;
	status = poll(&fd_info, 1, timeout);
	if (status == -1)
		throw SystemError("Error: poll failed", 500);
	if (status == 0)
		throw TimeoutExeption(408);
}

int	ResponseGenerator::openFile( void )
{
	fetchContentType();
	int	fd = open((_directory + _filename).c_str(), O_RDONLY);
	if (fd == -1)
		throw SystemError("Error: open failed" + _workingDir + _filename, 505);
	return fd;
}

void	ResponseGenerator::readFile( int fd )
{
	int			r_bytes = 1;
	char		buffer[MAX_R_BYTES];

	pollFd(fd, POLLIN, POLL_TIMEOUT);
	while (r_bytes > 0)
	{
		r_bytes = read(fd, &buffer, MAX_R_BYTES);
		if (r_bytes == -1)
			throw SystemError("Error: read failed", 500);
		if (r_bytes > 0)
			_response.append(buffer, r_bytes);
	}
}

int	ResponseGenerator::writeToFile()
{
	_contentType = "text/html";
	int	fd = open((_directory + _filename).c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd == -1)
		throw SystemError("Error: open failed", 500);
	pollFd(fd, POLLOUT, POLL_TIMEOUT);
	if (write(fd, _requestBody.c_str(), _requestBody.size()) == -1)
		throw SystemError("Error: write failed", 500);
	return fd;
}

void	ResponseGenerator::listDirectory( void )
{
	DIR*	dirp = opendir((_directory + _filename).c_str());
	if (!dirp)
		throw SystemError("Error: opendir failed", 500);
	struct dirent*	dp;
	while ((dp = readdir(dirp)))
		if (dp->d_name[0] != '.')
			_response += (std::string)dp->d_name + "\n";
	if (closedir(dirp) == -1)
		throw SystemError("Error: closedir failed", 500);
	_contentType = "text/plain";
}
