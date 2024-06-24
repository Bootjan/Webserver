/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   is_directory.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: bschaafs <bschaafs@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/21 17:34:34 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/21 17:52:57 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <string>

bool	is_directory( const std::string& path )
{
	struct stat	path_stat;
	stat(path.c_str(), &path_stat);
	return S_ISDIR(path_stat.st_mode) != 0;
}
