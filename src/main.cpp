/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/29 11:43:30 by tsteur        #+#    #+#                 */
/*   Updated: 2024/02/23 15:30:53 by tsteur        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <exception>
#include <functional>
#include <fstream>

#include "logger.hpp"
#include "http_server.hpp"
#include "ResponseGenerator.hpp"
#include "config.hpp"
#include "ErrorResponse.hpp"

bool	file_extension_check( const char* filename );

#define ALLOW_PATERN "^(GET|POST|DELETE)(?: (?!\\1)(GET|POST|DELETE))?(?: (?!\\1|\\2)(GET|POST|DELETE))?$"

static const config_template_t	config_template = {
	{"max_request_size", F_ONLY_ONCE, "^\\d+$", {}},
	{
	"errors", F_ONLY_ONCE, "^$",
	{
		{"400", F_ONLY_ONCE, FILE_PATERN, {}},
		{"403", F_ONLY_ONCE, FILE_PATERN, {}},
		{"404", F_ONLY_ONCE, FILE_PATERN, {}},
		{"405", F_ONLY_ONCE, FILE_PATERN, {}},
		{"408", F_ONLY_ONCE, FILE_PATERN, {}},
		{"413", F_ONLY_ONCE, FILE_PATERN, {}},
		{"415", F_ONLY_ONCE, FILE_PATERN, {}},
		{"500", F_ONLY_ONCE, FILE_PATERN, {}},
		{"501", F_ONLY_ONCE, FILE_PATERN, {}},
		{"505", F_ONLY_ONCE, FILE_PATERN, {}},
	}
	},
	{
	"server", F_MANDITORY, "^\\d{1,5}$",
	{
		{"root", F_ONLY_ONCE, FILE_PATERN, {}},
		{"name", F_ONLY_ONCE, "^.+$", {}},
		{"listing", F_ONLY_ONCE, "^(true|false)$", {}},
		{
		"location", F_NO_DUPLICATE, FILE_PATERN,
		{
			{"allow", F_ONLY_ONCE, ALLOW_PATERN, {}},
			{"alias", F_ONLY_ONCE, FILE_PATERN, {}},
			{"index", F_ONLY_ONCE, FILE_PATERN, {}},
		},
		},
	},
	},
};

std::vector<uint16_t>	get_ports(config_t config)
{
	std::vector<uint16_t>	ports;
	
	for (size_t idx = 0; idx < config["server"].size(); ++idx)
	{
		int port = std::stoi(config["server"][idx].value);

		if (port > UINT16_MAX)
			throw std::invalid_argument("invalid port");
		if (std::find(ports.begin(), ports.end(), port) == ports.end())
			ports.push_back(port);
	}
	return (ports);
}

void	init_callbacks(config_t config, ResponseGenerator* response_generators, HTTPServer& webserv)
{
	HTTPServer::request_callback_t	request_callback;
	std::string						server_name;
	
	for (size_t idx = 0; idx < config["server"].size(); ++idx)
	{
		request_callback = std::bind(&ResponseGenerator::generateResponse, &response_generators[idx], std::placeholders::_1);

		server_name = "";
		if (config["server"][idx].childs.count("name") == 1)
			server_name = config["server"][idx].childs["name"][0].value;
		webserv.add_request_callback(std::stoi(config["server"][idx].value), server_name, request_callback);
	}
}

int	main(int argc, char* argv[], char* const* envp)
{
	config_t			config;
	
	Logger::set_enabled_level(Logger::DEBUG, false);
	Logger::set_enabled_level(Logger::RESPONSE, false);
	Logger::set_enabled_level(Logger::INFO, false);

	if (argc != 2 || !file_extension_check(argv[1]))
	{
		Logger::log(Logger::ERROR, (std::string)"Error expected 1 config file with .conf extension\n");
		return (EXIT_FAILURE);
	}

	try
	{
		std::ifstream	config_file;
		
		config_file.open(argv[1]);
		config = parse_config(config_file);
		match_config(config, config_template);
		config_file.close();
	}
	catch (const ConfigException& e)
	{
		Logger::log(Logger::ERROR, (std::string)"Error while parsing config: " + e.what() + "\n");
		return (EXIT_FAILURE);
	}
	
	ExtensionGetter		extension_getter;
	ResponseGenerator*	response_generators = init_generators(envp, config, &extension_getter);
	if (config.count("errors") == 1)
		ErrorResponse::setErrorPages(config["errors"][0].childs);

	try
	{
		size_t		max_request_size = 0;
		if (config.count("max_request_size") == 1)
			max_request_size = std::stoul(config["max_request_size"][0].value);
		HTTPServer	webserv(get_ports(config), max_request_size);

		init_callbacks(config, response_generators, webserv);
		webserv.start();
	}
	catch (const std::exception& e)
	{
		Logger::log(Logger::ERROR, (std::string)"Error while running server: " + e.what() + "\n");
		remove_generators(response_generators);
		return (EXIT_FAILURE);
	}

	remove_generators(response_generators);
	return (EXIT_SUCCESS);
}
