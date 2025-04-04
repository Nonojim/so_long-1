/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lduflot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 11:13:25 by lduflot           #+#    #+#             */
/*   Updated: 2025/03/29 19:41:02 by lduflot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// Cherche \n ou fin du fichier
char	*ft_find_newline(char *str)
{
	if (str == NULL)
		return (NULL);
	while (*str != '\0')
	{
		if (*str == '\n')
			return (str);
		str++;
	}
	return (NULL);
}

int	ft_read_and_add_buffer(char **stash, int fd)
{
	char		*buff;
	int			byte_read;

	buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buff == NULL)
		return (-1);
	byte_read = read(fd, buff, BUFFER_SIZE);
	if (byte_read < 0)
	{
		free(buff);
		if (*stash != NULL)
		{
			free(*stash);
			*stash = NULL;
		}
		return (-1);
	}
	buff[byte_read] = '\0';
	*stash = ft_strjoin(*stash, buff);
	free(buff);
	return (byte_read);
}
//lit les données et les stocke dans stash.
//byte_read = nbr d octet lu a chaque appel de read
// buff = stockage temporaore des donnes lu
// strjoin = join les nouvelles donne lu au donne deja lu 

char	*ft_readline(char *stash, int fd)
{
	char	*newline;
	int		byte_read;

	byte_read = 1;
	newline = ft_find_newline(stash);
	while (newline == NULL && byte_read > 0)
	{
		byte_read = ft_read_and_add_buffer(&stash, fd);
		if (byte_read < 0 || stash == NULL)
		{
			free(stash);
			return (NULL);
		}
		newline = ft_find_newline(stash);
	}
	return (stash);
}

char	*ft_extract_line(char **stash)
{
	char	*line;
	int		len_line;
	char	*tmp_stash;

	if (*stash == NULL || **stash == '\0')
		return (NULL);
	line = extract_line_from_stash(*stash);
	if (line == NULL)
		return (NULL);
	len_line = ft_strlen(line);
	if (len_line > 0 && line[len_line - 1] == '\n')
		line[len_line - 1] = '\0';
	tmp_stash = ft_strdup(*stash + len_line);
	free(*stash);
	*stash = tmp_stash;
	return (line);
}
//stash = stock, reserve, cachette
//line = buffer pour lire les donnes

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = ft_readline(stash, fd);
	if (stash == NULL)
		return (NULL);
	line = ft_extract_line(&stash);
	if (line == NULL && stash != NULL)
	{
		free(stash);
		stash = NULL;
	}
	return (line);
}
//line = zone memoire qui stock les donnes

/*int	main(void)
{
	int		fd;
	char	*line;

	fd = open("test.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("Lecture: %s\n", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}*/
