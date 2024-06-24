/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   contains_parent_dir.cpp                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: bschaafs <bschaafs@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/20 13:47:10 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/20 15:53:38 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <regex>
#include <string>

bool	contains_parent_dir( const std::string& str )
{
	if (std::regex_search(str, std::regex("(^\\.\\./|/\\.\\./|/\\.\\.$)")))
		return true;
	return false;
}
