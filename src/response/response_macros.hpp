/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_macros.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/01 21:32:29 by bootjan       #+#    #+#                 */
/*   Updated: 2024/02/21 17:32:53 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define APP		"application"
#define AUDIO	"audio"
#define CHEM	"chemical"
#define FONT	"font"
#define IMAGE	"image"
#define INODE	"inode"
#define MESS	"message"
#define MODEL	"model"
#define MULT	"multipart"
#define TEXT	"text"
#define VIDEO	"video"

#define ERROR_400	"<h1 font-size=\"40\">Error 400: Bad Request</h1><a>The request send by the user was incorrectly formatted.</a>"
#define ERROR_403	"<h1 font-size=\"40\">Error 403: Forbidden</h1><a>The requested action is not allowed by the server</a>"
#define ERROR_404	"<h1 font-size=\"40\">Error 404: Page Not Found</h1><a>The page you requested could not be found.</a>"
#define ERROR_405	"<h1 font-size=\"40\">Error 405: Method Not Allowed</h1><a>The method requested is not supported.</a>"
#define ERROR_408	"<h1 font-size=\"40\">Error 408: Request Timeout</h1><a>The server timed out waiting for a request</a>"
#define ERROR_413	"<h1 font-size=\"40\">Error 413: Payload Too Large</h1><a>The request is to large.</a>"
#define ERROR_415	"<h1 font-size=\"40\">Error 415: Unsupported Media Type</h1><a>The requested media type is not supported</a>"
#define ERROR_500	"<h1 font-size=\"40\">Error 500: Internal Server Error</h1><a>The server had an unexpected error. Try reloading the page</a>"
#define ERROR_501	"<h1 font-size=\"40\">Error 501: Not Implemented</h1><a>The request callback is not implemented</a>"
#define ERROR_505	"<h1 font-size=\"40\">Error 505: HTTP Version Not Supported</h1><a>The requested HTTP version is not supported by the server</a>"
#define ERROR_42	"<h1 font-size=\"40\">Error 42: What</h1><a>What is going on? Need coffee?</a>"
