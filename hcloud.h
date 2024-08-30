/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hcloud.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhabacuc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:59:14 by lhabacuc          #+#    #+#             */
/*   Updated: 2024/08/30 14:59:16 by lhabacuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HCLOUD_H
# define HCLOUD_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <pwd.h>
# include <errno.h>

# define BUFFER_CLI 100
# define PORTP 8080
# define BUFFER_SIZE 1024

typedef struct s_sever
{
	char	method[10];
	char	path[BUFFER_SIZE];
	char	*username;
	char	previliegio[10];
	char	ip[21];
	int		server_socket;
}		t_server;

typedef struct s_client
{
	char	method[10];
	char	path[BUFFER_SIZE];
	char	*username;
	char	prev[10];
	char	ip[21];
	int		client_socket;
}		t_client;

/* client */
void	configure_client(t_client *client, const char *server_ip);
void	process_command(t_client *client, const char *command);


/* server */
void	ceate_server(t_server *server, const char *ip);
void	process_request(int client_socket, const char *request);
void	process_message(int client_socket, const char *username, const char *message);
void	ceate_server_or(t_server *server);
void	request(int client_socket, const char *request);
int		acess_client(t_server *server);

/* comum */
void	receive_file(int client_socket, const char *file_path);
void	send_file(int client_socket, const char *file_path);
void	receive_data(int client_socket, char *buffer, size_t buffer_size);
void	send_data(int client_socket, const char *data);
void	apply_each_client(void (*func)(int));
void	add_client(int cl);

#endif
