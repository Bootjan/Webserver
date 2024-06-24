/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ErrorResponse.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: bschaafs <bschaafs@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/21 14:17:14 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/22 14:04:48 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "response_macros.hpp"
#include "config.hpp"
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <map>

class ErrorResponse
{
	public:
		ErrorResponse() {}
		ErrorResponse( const ErrorResponse& rhs ) = delete;
		ErrorResponse&	operator=( const ErrorResponse& rhs ) = delete;
		~ErrorResponse() {}

		static std::string	generateResponse( size_t error_code );
		static void			setErrorPages( config_t config );
	private:
		static std::map<size_t, std::string>	_errorPages;

		static std::string						errorMessage( size_t error_code );
		static std::string						errorBody( size_t error_code );
};
