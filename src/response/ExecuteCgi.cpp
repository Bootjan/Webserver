/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ExecuteCgi.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/05 16:23:26 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/21 15:51:37 by tsteur        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseGenerator.hpp"

std::string ResponseGenerator::extractPaths( void ) const
{
	size_t		idx;
	std::string	paths;
	for (idx = 0; _envp[idx]; idx++)
		if (strncmp(_envp[idx], "PATH=", 5) == 0)
			break ;
	if (!_envp[idx])
		return nullptr;
	paths = _envp[idx];
	paths = paths.substr(5, paths.size());
	return paths;
}

std::string	ResponseGenerator::findPath( const std::string& command ) const
{
	if (access(command.c_str(), X_OK) == 0)
		return command;
	std::string	paths = extractPaths();
	if (paths.size() == 0)
		return command;
	std::string			sub_str, poss_path;
	std::stringstream	ss(paths);
	while (std::getline(ss, sub_str, ':'))
	{
		poss_path = sub_str + "/" + command;
		if (access(poss_path.c_str(), X_OK) == 0)
			return poss_path;
	}
	return command;
}

char**	ResponseGenerator::computeCmd( const std::string& command )
{
	char**	cmds = (char **)calloc(3, sizeof(char *));
	if (!cmds)
		throw SystemError("Error: malloc failed", 500);
	cmds[0] = strdup(command.c_str());
	if (!cmds[0])
		{ free(cmds); throw SystemError("Error: malloc failed", 500); }
	cmds[1] = strdup(_filename.c_str());
	if (!cmds[1])
		{ free(cmds[0]); free(cmds); throw SystemError("Error: malloc failed", 500); }
	return cmds;
}

std::string	ResponseGenerator::getCommand( void )
{
	std::string	cgi_extensions[] = { "pl", "py", "sh", "js", "rb" };
	size_t		idx;

	for (idx = 0; idx < sizeof(cgi_extensions) / sizeof(cgi_extensions[0]) && _extension != cgi_extensions[idx]; idx++)
		;
	switch (idx)
	{
		case 0: return "perl";
		case 1: return "python3";
		case 2: return "sh";
		case 3: return "node";
		case 4: return "ruby";
		default: throw InvalidFileExtension(_extension, 415);
	}
}

void	ResponseGenerator::moveToDir( void )
{
	if (chdir(_directory.c_str()) == -1)
		throw SystemError("Error: chdir failed", 500);
}

void	ResponseGenerator::setOptionsPost( void )
{
	int	fd[2];

	if (pipe(fd) == -1)
		throw SystemError("Error: pipe failed", 500);
	if (write(fd[1], _requestBody.c_str(), _requestBody.size()) == -1)
		throw SystemError("Error: write failed", 500);
	if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
		throw SystemError("Error: dup2 failed", 500);
	close(fd[1]);
	close(fd[0]);
}

void	ResponseGenerator::addToEnvp( const std::string& new_el )
{
	_envp = add_to_2d_array(_envp, new_el);
	if (!_envp)
		throw SystemError("Error: malloc failed", 500);
}

void	ResponseGenerator::setOptions( void )
{
	addToEnvp("PATH_INFO=" + _additionalPath);
	if (_method == GET)
		addToEnvp("QUERY_STRING=" + _arguments);
	else if (_method == POST)
		setOptionsPost();
}

void	ResponseGenerator::executeCgi( int fd )
{
	setOptions();
	moveToDir();
	std::string	command = getCommand();
	std::string	path = findPath(command);

	if (dup2(fd, STDOUT_FILENO) != STDOUT_FILENO)
		throw SystemError("Error: dup2 failed", 500);
	char**		cmds = computeCmd(command);
	if (!cmds)
		throw SystemError("Error: failed to make cmds", 500);

	execve(path.c_str(), cmds, _envp);
	free_2d_array(&cmds, FREE_2D);
	exit(EXIT_FAILURE);
}

int	ResponseGenerator::runCgi( void )
{
	_contentType = "text/html";
	int	fd[2], pid, status = 0;

	if (pipe(fd) == -1)
		throw SystemError("Error: pipe failed", 500);
	pid = fork();
	if (pid == -1)
		throw SystemError("Error: fork failed", 500);
	if (pid == 0)
		executeCgi(fd[1]);
	else
		waitpid(pid, &status, 0);
	close(fd[1]);
	if (status != 0)
		throw SystemError("Error: child process failed", 500);
	return fd[0];
}
