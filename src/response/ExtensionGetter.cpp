/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ExtensionGetter.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/01 14:45:02 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/21 17:42:57 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ExtensionGetter.hpp"

void	ExtensionGetter::insertTypes( std::unordered_set<std::string>* set, const std::string& line )
{
	size_t				begin = line.find_first_of('/');
	std::string			extensions = line.substr(begin + 1, line.size());
	std::string			extension;
	std::stringstream	ss(extensions);

	std::getline(ss, extension, '\t');
	if (extension.size() > 0)
		set->insert(extension);
	if (!std::getline(ss, extension, ' '))
		return ;
	while (std::getline(ss, extension, ' '))
		if (extension.size() > 0)
			set->insert(extension);
}

ExtensionGetter::ExtensionGetter()
{
	std::ifstream	typesFile;
	typesFile.open(_mimeTypes);
	if (!typesFile.is_open())
		throw std::runtime_error("couldn't open " + _mimeTypes);
	std::string	line;
	while (!typesFile.eof())
	{
		std::getline(typesFile, line);
		if (line.size() == 0 || line[0] == '#')
			continue;
		for (auto el : typeDict)
			if (line.find(el.first) != std::string::npos)
				{ insertTypes(typeSetPtrArr[el.second], line); continue; }
	}
	typesFile.close();
}

void	ExtensionGetter::printTypes( size_t idx) const
{
	if (idx > 10)
		return ;
	for (std::string el : (*typeSetPtrArr)[idx])
		std::cout << el << std::endl;
}

std::string	ExtensionGetter::getContentType( const std::string& extension )
{
	if (extension == "ico")
		return "text/plain";
	for (auto el : typeDict)
		if ((*typeSetPtrArr)[el.second].find(extension) != (*typeSetPtrArr)[el.second].end())
			return (el.first + "/" + extension);
	throw InvalidFileExtension(extension, 415);
}
