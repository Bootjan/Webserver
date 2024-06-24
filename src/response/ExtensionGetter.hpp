/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ExtensionGetter.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/01 14:15:48 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/21 14:01:21 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <unordered_set>
#include <string>
#include <fstream>
#include <map>
#include <sstream>

#include "response_macros.hpp"
#include "ResponseExceptions.hpp"

#define TYPES "./src/resource/mime.types"

typedef struct request_s
{
	std::map<std::string, std::string>	header;
	std::string							url;
	std::string							http_version;
	std::string							method;
	std::string							body;
}	request_t;

const std::map<std::string, int>	typeDict = {
	{ APP, 0 },
	{ AUDIO, 1 },
	{ CHEM, 2 },
	{ FONT, 3 },
	{ IMAGE, 4 },
	{ INODE, 5 },
	{ MESS, 6 },
	{ MODEL, 7 },
	{ MULT, 8 },
	{ TEXT, 9 },
	{ VIDEO, 10 }
};

class ExtensionGetter
{
	public:
		ExtensionGetter();
		ExtensionGetter( const ExtensionGetter& rhs ) = delete;
		ExtensionGetter&	operator=( const ExtensionGetter& rhs ) = delete;
		~ExtensionGetter() {}

		std::string			getContentType( const std::string& extension );
	
	private:
		std::unordered_set<std::string>		_application;
		std::unordered_set<std::string>		_audio;
		std::unordered_set<std::string>		_chemical;
		std::unordered_set<std::string>		_font;
		std::unordered_set<std::string>		_image;
		std::unordered_set<std::string>		_inode;
		std::unordered_set<std::string>		_message;
		std::unordered_set<std::string>		_model;
		std::unordered_set<std::string>		_multipart;
		std::unordered_set<std::string>		_text;
		std::unordered_set<std::string>		_video;

		const std::string	_mimeTypes = TYPES;

		void	insertTypes( std::unordered_set<std::string>* set, const std::string& line );
		void	printTypes( size_t idx ) const;

	private:
		std::unordered_set<std::string>*	typeSetPtrArr[11] = {&_application, &_audio, &_chemical, &_font, &_image, &_inode, &_message, &_model, &_multipart, &_text, &_video};
};
