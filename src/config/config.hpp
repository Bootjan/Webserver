/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tsteur <tsteur@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/06 12:44:04 by tsteur        #+#    #+#                 */
/*   Updated: 2024/02/19 14:56:48 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <map>
# include <vector>
# include <fstream>
# include <string>
# include <regex>

struct FieldTemplate;
struct Field;

class ConfigException : public std::exception {};

class InvalidLineException : public ConfigException
{
	private:
		const std::string _message;
	public:
					InvalidLineException(std::string line);
		const char*	what() const throw();
};

class MissingFieldException : public ConfigException
{
	private:
		const std::string _message;
	public:
					MissingFieldException(const FieldTemplate field_template);
		const char*	what() const throw();
};

class MoreThanOnceException : public ConfigException
{
	private:
		const std::string _message;
	public:
					MoreThanOnceException(const FieldTemplate field_template);
		const char*	what() const throw();
};

class InvalidValueException : public ConfigException
{
	private:
		const std::string _message;
	public:
					InvalidValueException(std::string tag, Field field);
		const char*	what() const throw();
};

class DuplicateValueException : public ConfigException
{
	private:
		const std::string _message;
	public:
					DuplicateValueException(std::string tag, Field field);
		const char*	what() const throw();
};

class InvalidFieldException : public ConfigException
{
	private:
		const std::string _message;
	public:
					InvalidFieldException(std::string tag);
		const char*	what() const throw();
};


# define FILE_PATERN "^(/|)([^/\\s]+/)*([^/\\s]+/?)?$"

enum
{
	F_MANDITORY = 1,
	F_ONLY_ONCE = 2,
	F_NO_DUPLICATE = 4,
};

struct FieldTemplate
{	
	const char*							tag;
	const int							flags;
	const char*							value_patern;
	const std::vector<FieldTemplate>	allowed_fields;
};

typedef const std::vector<FieldTemplate>	config_template_t;


typedef std::map<std::string, std::vector<Field>>	field_tree_t;

struct Field
{
	std::string		value;
	field_tree_t	childs;

	std::vector<Field>	operator [] (std::string key);
};

typedef field_tree_t	config_t;

config_t	parse_config(std::ifstream& file);
void		match_config(config_t &config, config_template_t &config_template);

#endif