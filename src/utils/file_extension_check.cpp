/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   file_extension_check.cpp                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bschaafs <bschaafs@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/21 18:25:35 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/21 18:28:00 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>

bool	file_extension_check( const char* filename )
{
	std::string	file(filename);
	if (file.size() < 5)
		return false;
	std::string	extension = file.substr(file.size() - 5, file.size());
	return extension == ".conf";
}
