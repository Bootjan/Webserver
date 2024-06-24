/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ResponseExceptions.hpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: bschaafs <bschaafs@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/19 13:30:59 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/22 12:11:24 by tsteur        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

struct ResponseException : public std::runtime_error
{
	size_t	responseCode;
	ResponseException( const std::string& message, size_t error_code ) : std::runtime_error(message), responseCode(error_code) {}
};

struct InvalidFileExtension : public ResponseException
{
	InvalidFileExtension( const std::string& extension , size_t error_code) : ResponseException("invalid file extension, " + extension, error_code) {}
};

struct IncorrectFormat : public ResponseException
{
	IncorrectFormat( const std::string& filename , size_t error_code) : ResponseException("invalid filename, " + filename, error_code) {}
};

struct UnreadableFile : public ResponseException
{
	UnreadableFile( const std::string& filename , size_t error_code) : ResponseException("file doesn't exist or has no permissions, " + filename, error_code) {}
};

struct InvalidMethod : public ResponseException
{
	InvalidMethod( const std::string& subStr , size_t error_code) : ResponseException("invalid request method, " + subStr, error_code) {}
};

struct BadRequest : public ResponseException
{
	BadRequest( size_t error_code) : ResponseException("bad request", error_code) {}
};

struct UnallowedMethod : public ResponseException
{
	UnallowedMethod( const std::string& method , size_t error_code) : ResponseException("requested method not allowed, " + method, error_code) {}
};


struct SystemError : public ResponseException
{
	SystemError( const std::string& message , size_t error_code) : ResponseException(message, error_code) {}
};

struct InvalidHttpVersion : public ResponseException
{
	InvalidHttpVersion( const std::string& message , size_t error_code) : ResponseException(message, error_code) {}
};

struct TimeoutExeption : public ResponseException
{
	TimeoutExeption( size_t error_code) : ResponseException("server timed out", error_code) {}
};

struct ForbiddenAction : public ResponseException
{
	ForbiddenAction( const std::string& message, size_t error_code ) : ResponseException("action forbidden, " + message, error_code) {}
};
