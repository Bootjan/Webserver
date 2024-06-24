/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_generators.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: bschaafs <bschaafs@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/20 13:44:22 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/20 13:47:01 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseGenerator.hpp"

ResponseGenerator*	init_generators( char* const* envp, config_t config, ExtensionGetter* extension_getter )
{
	ResponseGenerator*	generators = new ResponseGenerator[config["server"].size()];

	for (size_t idx = 0; idx < config["server"].size(); ++idx)
		generators[idx].setInfo(envp, config["server"][idx].childs, extension_getter);
	return generators;
}

void	remove_generators( ResponseGenerator*& response_generators )
{
	delete[] response_generators;
	response_generators = nullptr;
}
