/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   array_utils.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: bootjan <bootjan@student.42.fr>              +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/06 14:02:26 by bschaafs      #+#    #+#                 */
/*   Updated: 2024/02/19 12:16:30 by bschaafs      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "array_header.hpp"

static size_t	len_2d_array( char* const* arr )
{
	size_t	len = 0;
	for (; arr[len]; len++)
		;
	return len;
}

void	free_2d_array( char*** arr, uint8_t mode )
{
	if (!arr || !*arr)
		return ;
	for (size_t i = 0; (*arr)[i] && mode == FREE_2D; i++)
		free((*arr)[i]);
	free(*arr);
	*arr = nullptr;
}

char**	copy_2d_array( char* const* arr )
{
	size_t	len = len_2d_array(arr);
	char	**new_arr = (char **)calloc(len + 1, sizeof(char *));
	if (!new_arr)
		return nullptr;
	for (size_t i = 0; i < len; i++)
	{
		new_arr[i] = strdup(arr[i]);
		if (!new_arr[i])
			return (free_2d_array(&new_arr, FREE_2D), nullptr);
	}
	return new_arr;
}

char**	add_to_2d_array( char** arr, const std::string& el )
{
	if (!arr)
		return nullptr;
	size_t	len = len_2d_array(arr);
	char**	new_arr = (char **)calloc(len + 2, sizeof(char *));
	if (!new_arr)
		return (free_2d_array(&arr, FREE_2D), nullptr);
	for (size_t i = 0; i < len; i++)
		new_arr[i] = arr[i];
	new_arr[len] = strdup(el.c_str());
	if (!new_arr[len])
		return (free_2d_array(&new_arr, FREE_2D), free_2d_array(&arr, FREE_2D), nullptr);
	free_2d_array(&arr, FREE_1D);
	return new_arr;
}

char**	remove_from_2d_array( char** arr, const std::string& el )
{
	if (!arr)
		return nullptr;
	size_t	idx = 0, len = len_2d_array(arr);
	for(; idx < len; idx++)
		if (strncmp(arr[idx], el.c_str(), el.size()) == 0)
			break;
	if  (idx == len)
		return arr;
	char**	new_arr = (char **)calloc(len, sizeof(char *));
	if (!new_arr)
		return (free_2d_array(&arr, FREE_2D), nullptr);
	free(arr[idx]);
	for (size_t i = 0, j = 0; i < len; i++)
		if (i != idx)
			new_arr[j++] = arr[i];
	free_2d_array(&arr, FREE_1D);
	return new_arr;
}
