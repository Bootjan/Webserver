/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   array_header.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: bschaafs <bschaafs@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/06 14:32:29 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/06 14:55:31 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstring>

#define FREE_1D 1
#define FREE_2D 2

// ||--- HANDLING 2D ARRAYS ---||
void	free_2d_array( char*** arr, uint8_t mode );
char**	copy_2d_array( char* const* arr );
char**	add_to_2d_array( char** arr, const std::string& el );
char**	remove_from_2d_array( char** arr, const std::string& el );
