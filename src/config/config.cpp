/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tsteur <tsteur@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/07 11:28:49 by tsteur        #+#    #+#                 */
/*   Updated: 2024/02/20 15:33:49 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <string>
#include <regex>

#include "config.hpp"

InvalidLineException::InvalidLineException(std::string line): _message("invalid syntax: \"" + line.substr(0, line.size() - 1) + "\"")
{
	
}

const char* InvalidLineException::what() const throw()
{
	return (_message.c_str());
}

MissingFieldException::MissingFieldException(const FieldTemplate field_template): _message((std::string)"missing field: \"" + field_template.tag + "\"")
{
	
}

const char* MissingFieldException::what() const throw()
{
	return (_message.c_str());
}

MoreThanOnceException::MoreThanOnceException(const FieldTemplate field_template): _message((std::string)"field appeared more than once: \"" + field_template.tag + "\"")
{
	
}

const char* MoreThanOnceException::what() const throw()
{
	return (_message.c_str());
}


InvalidValueException::InvalidValueException(std::string tag, const Field field): _message("invalid value: \"" + tag + ": " + field.value + "\"")
{
	
}

const char* InvalidValueException::what() const throw()
{
	return (_message.c_str());
}

DuplicateValueException::DuplicateValueException(std::string tag, const Field field): _message("duplicate value: \"" + tag + ": " + field.value + "\"")
{
	
}

const char* DuplicateValueException::what() const throw()
{
	return (_message.c_str());
}

InvalidFieldException::InvalidFieldException(std::string tag): _message("invalid field: \"" + tag + "\"")
{
	
}

const char* InvalidFieldException::what() const throw()
{
	return (_message.c_str());
}

std::vector<Field>	Field::operator [] (std::string key)
{
	return (childs[key]);
}

static std::string	get_key(std::string line)
{
	size_t	first_character;
	size_t	last_character;

	first_character = line.find_first_not_of('\t');
	last_character = line.find_first_of(':');
	if (last_character == std::string::npos)
		throw InvalidLineException(line);
	return (line.substr(first_character, last_character - first_character));
}
static std::string	get_value(std::string line)
{
	size_t	first_character;
	size_t	last_character;

	first_character = line.find_first_not_of("\t ", line.find_first_of(':') + 1);
	last_character = line.find_first_of("\n", first_character);
	return (line.substr(first_character, last_character - first_character));
}

static field_tree_t	parse_indentation(std::ifstream& file, int indentation, std::string& current_line)
{
	field_tree_t	fields;
	std::string		key;
	Field			field;
	int				current_indentation;
	
	std::getline(file, current_line);
	current_line += '\n';
	while (file)
	{
		if (current_line[0] == '#' || \
			current_line[current_line.find_first_not_of('\t')] == '\n')
		{
			std::getline(file, current_line);
			current_line += '\n';
			continue ;
		}
		current_indentation = current_line.find_first_not_of('\t');
		if (current_indentation > indentation)
			throw InvalidLineException(current_line);
		if (current_indentation < indentation)
			break ;
		key = get_key(current_line);
		if (key.size() == 0)
			throw InvalidLineException(current_line);
		field.value = get_value(current_line);
		field.childs = parse_indentation(file, indentation + 1, current_line);
		fields[key].push_back(field);
	}
	return (fields);
}

config_t	parse_config(std::ifstream& file)
{
	config_t	config;
	std::string	current_line;
	
	config = parse_indentation(file, 0, current_line);
	return (config);
}

void	match_config(config_t& config, config_template_t &config_template)
{
	for (unsigned int i = 0; i < config_template.size(); i++)
	{
		if (config.count(config_template[i].tag) == 1)
		{
			if (config_template[i].flags & F_ONLY_ONCE)
			{
				if (config[config_template[i].tag].size() > 1)
					throw MoreThanOnceException(config_template[i]);
			}
			for (unsigned int j = 0; j < config[config_template[i].tag].size(); j++)
			{
				if (!std::regex_match(config[config_template[i].tag][j].value, std::regex(config_template[i].value_patern)))
					throw InvalidValueException(config_template[i].tag, config[config_template[i].tag][j]);
				match_config(config[config_template[i].tag][j].childs, config_template[i].allowed_fields);
				if (config_template[i].flags & F_NO_DUPLICATE)
				{
					for (unsigned int k = 0; k < config[config_template[i].tag].size(); k++)
					{
						if (j == k)
							continue ;
						if (config[config_template[i].tag][j].value == config[config_template[i].tag][k].value)
							throw DuplicateValueException(config_template[i].tag, config[config_template[i].tag][j]);
					}
				}
			}
		}
		else
		{
			if (config_template[i].flags & F_MANDITORY)
				throw MissingFieldException(config_template[i]);
		}
	}
	for (const auto& field : config)
	{
		bool	found;

		found = false;
		for (unsigned int i = 0; i < config_template.size(); i++)
		{
			if (field.first == config_template[i].tag)
			{
				found = true;
			}
		}
		if (found == false)
			throw InvalidFieldException(field.first);
	}
}
