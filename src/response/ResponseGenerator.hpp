/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ResponseGenerator.hpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/31 19:51:01 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/21 17:35:31 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <poll.h>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <dirent.h>

#include "ExtensionGetter.hpp"
#include "logger.hpp"
#include "array_header.hpp"
#include "config.hpp"

#define MAX_R_BYTES 512

typedef struct location_s
{
	std::string		value;
	__uint8_t		permissions : 3;
	std::string		defaultFile;
	std::string		alias;
}	location_t;

class ResponseGenerator
{
	public:
		ResponseGenerator() : _envp(nullptr), _extensionGetter(nullptr) {};
		ResponseGenerator( const ResponseGenerator& rhs ) = delete;
		ResponseGenerator&	operator=( const ResponseGenerator& rhs ) = delete;
		~ResponseGenerator();
	
		std::string			generateResponse( request_t request );
		void				setInfo( char* const* envp, config_t config, ExtensionGetter* extension_getter );

	private:
		enum methodCodes { GET = 0, POST, DELETE, ERROR };
		enum permissions { GET_PERM = 1, POST_PERM = 2, DELETE_PERM = 4, ALL_PERM = 7 };
	
		size_t			_responseCode;
		methodCodes		_method;
		bool			_isCgi;
		std::string		_HTTPVersion;
		std::string		_filename;
		std::string		_directory;
		std::string		_extension;
		std::string		_arguments;
		std::string		_additionalPath;
		std::string		_contentType;
		std::string		_hostname;
		std::string		_response;
		std::string		_header;
		std::string		_requestBody;


		char**					_envp;
		std::string				_workingDir;
		ExtensionGetter*		_extensionGetter;
		const std::string		_cgiFolder = "/cgi-bin";
		bool					_dirlisting;
		std::vector<location_t>	_locations;
	
		// ||--- MAIN FUNCTIONS ---||
		void		resetVariables( void );
		void		parseRequest( request_t request );
		void		handleMethod( void );
		void		generateFirstLine( void );
		void		generateHeader( void );

		// ||--- PARSE REQUEST ---||
		void		setMethod( const std::string& subStr );
		void		setHTTPVersion( const std::string& version );
		void		setFilename( const std::string& url );
		void		setArguments( void );
		void		setAdditionalPath( void );
		void		setDirectory( void );
		void		setExtension( void );
		void		checkPermissionForDir( void );
		bool		validPermissions( __uint8_t permissions );
		void		checkCgi( void );
		void		setHostname( std::map<std::string, std::string> dict );
	
		void		checkAlias( void );

		// ||--- EXECUTE METHODS ---||
		void		executeGet( void );
		void		executePost( void );
		void		executeDelete( void );

		void		listDirectory( void );

		// ||--- HANDLING FILES ---||
		void		fetchContentType( void );
		void		checkPermissionForFile( void );
		void		pollFd( int fd, nfds_t nfds, int timeout );
		int			openFile( void );
		void		readFile( int fd );
		int			writeToFile();

		// ||--- EXECUTE CGI ---||
		std::string	extractPaths( void ) const;
		std::string	findPath( const std::string& command ) const;
		char**		computeCmd( const std::string& command );
		std::string	getCommand( void );
		void		moveToDir( void );
		void		setOptionsPost( void );
		void		addToEnvp( const std::string& new_el );
		void		setOptions( void );
		void		executeCgi( int fd );
		int			runCgi( void );

		// ||--- GET DATE ---||
		std::string	getDate( void ) const;

		// ||--- INITIALISATION ---||
		location_t	setLocation( config_t el, const std::string& value );
		__uint8_t	setPermissions( config_t config );
		std::string	setDefaultFile( config_t config );
		std::string	setAlias( config_t config );
};

// ||--- INITIALIZING GENERATORS ---||
ResponseGenerator*	init_generators( char* const* envp, config_t config, ExtensionGetter* extension_getter );
void				remove_generators( ResponseGenerator*& response_generators );

// ||--- CONTAINS PARENT DIRECTORY ---||
bool				contains_parent_dir( const std::string& str );

// ||--- CHECK DIRECTORY ---||
bool				is_directory( const std::string& path );
