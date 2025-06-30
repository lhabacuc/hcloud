/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ser_conf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhabacuc <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 15:36:09 by lhabacuc          #+#    #+#             */
/*   Updated: 2024/08/30 15:36:11 by lhabacuc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hcloud.h"

void	create_server_or(t_server *server)
{
	struct sockaddr_in server_addr;

	server->server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server->server_socket == -1)
	{
		perror("Erro ao criar socket");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORTP);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server->server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("Erro ao vincular socket");
		close(server->server_socket);
		exit(EXIT_FAILURE);
	}

	if (listen(server->server_socket, 5) == -1)
	{
		perror("Erro ao ouvir");
		close(server->server_socket);
		exit(EXIT_FAILURE);
	}
}

void	create_server(t_server *server, const char *ip)
{
	struct sockaddr_in server_addr;

	server->server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server->server_socket == -1)
	{
		perror("Erro ao criar socket");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORTP);
	if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
	{
		perror("Endereço IP inválido");
		close(server->server_socket);
		exit (EXIT_FAILURE);
	}

	if (bind(server->server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("Erro ao vincular socket");
		close(server->server_socket);
		exit (EXIT_FAILURE);
	}


	if (listen(server->server_socket, 5) == -1)
	{
		perror ("Erro ao ouvir");
		close (server->server_socket);
		exit (EXIT_FAILURE);
	}
}

int	acess_client(t_server *server)
{
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_socket;

	client_socket = accept(server->server_socket, (struct sockaddr *)&client_addr, &client_len);
	if (client_socket == -1)
	{
		perror("Erro ao aceitar conexão");
		close(server->server_socket);
		exit(EXIT_FAILURE);
	}

	printf("Cliente conectado: %s\n", inet_ntoa(client_addr.sin_addr));
	return (client_socket);
}

void	close_server(t_server *server)
{
	if (close(server->server_socket) == -1)
	{
		perror("Erro ao fechar socket");
		exit(EXIT_FAILURE);
	}
	printf("Socket do servidor fechado com sucesso\n");
}

/* func exmple : */
void	process_message(int client_socket, const char *username, const char *message)
{
	FILE *fp = fopen("messages.txt", "a");
	if (fp == NULL)
	{
		perror("Erro ao abrir arquivo de mensagens");
		send_data(client_socket, "500 Internal Server Error\n");
		return ;
	}

	fprintf(fp, "%s: %s\n", username, message);
	fclose(fp);
	send_data(client_socket, "200 OK: Mensagem recebida\n");
}

void	request(int client_socket, const char *request)
{
	char method[10];
	char path[BUFFER_SIZE];
	char data[BUFFER_SIZE];
	int ret;

	ret = sscanf(request, "%9[^:]:%1023[^:]:%1023[^\n]", method, path, data);
	if (ret < 2)
	{
		printf("Formato de requisição inválido\n");
		send_data(client_socket, "400 Bad Request\n");
		return;
	}

	if (strcmp(method, "GET") == 0)
		send_data(client_socket, "200 OK: GET request received\n");
	else if (strcmp(method, "POST") == 0)
		send_data(client_socket, "200 OK: POST request received\n");
	else if (strcmp(method, "DWN") == 0)
		send_data(client_socket, "200 OK: Download request received\n");
	else if (strcmp(method, "UPT") == 0)
	{
		FILE *fp = fopen(path, "wb");
		if (fp != NULL)
		{
		fwrite(data, 1, strlen(data), fp);
		fclose(fp);
		send_data(client_socket, "200 OK: File uploaded\n");
		}
		else
			send_data(client_socket, "500 Internal Server Error\n");
	}
	else
		send_data(client_socket, "405 Method Not Allowed\n");
}

void	process_request(int client_socket, const char *request)
{
	char buffer[BUFFER_SIZE];
	char method[10], path[BUFFER_SIZE], prev[10], username[BUFFER_SIZE];
	int read_size;

	memset(method, 0, sizeof(method));
	memset(path, 0, sizeof(path));
	memset(prev, 0, sizeof(prev));
	memset(username, 0, sizeof(username));
	(void)request;
	read_size = read(client_socket, buffer, sizeof(buffer) - 1);
	if (read_size <= 0)
	{
		perror("Erro ao ler a requisição");
		close(client_socket);
		return ;
	}

	buffer[read_size] = '\0';

	sscanf(buffer, "%9[^:]:%1023[^:]:%9[^:]:%1023[^\n]", method, path, prev, username);

	if (strcmp(method, "GET") == 0)
	{
		printf("Recebido GET: %s\n", path);
		send_file(client_socket, path);
	}
	else if (strcmp(method, "POST") == 0)
	{
		printf("Recebido POST: %s\n", path);
		receive_file(client_socket, path);
	}
	else if (strcmp(method, "CMD") == 0)
		printf("Recebido CMD: %s\n", path);
	else if (strcmp(method, "DWN") == 0)
	{
		printf("Recebido DWN: %s\n", path);
		send_file(client_socket, path);
	}
	else if (strcmp(method, "msg") == 0)
	{
		printf("Recebido msg: %s\n", path);
		process_message(client_socket, username, path);
	}
	else
	{
		printf("Método desconhecido: %s\n", method);
		const char *error_msg = "Método não reconhecido";
		write(client_socket, error_msg, strlen(error_msg));
	}
	close(client_socket);
}

char	*if_dir(const char *d)
{
	if (!d)
		return ("./www");
	return ((char*)d);
}

void	http_request(int client_socket, const char *request, const char *dir)
{
	char *method;
	char *path;
	char buffer[BUFFER_SIZE];

	method = strtok((char *)request, " ");
	path = strtok(NULL, " ");
	if (method && path)
	{
		if (strcmp(method, "GET") == 0)
			serve_directory(client_socket, if_dir(dir), path);
		else
		{
			snprintf(buffer, sizeof(buffer), "HTTP/1.1 405 Method Not Allowed\r\n\r\n");
			write(client_socket, buffer, strlen(buffer));
		}
	}
	else
	{
		snprintf(buffer, sizeof(buffer), "HTTP/1.1 400 Bad Request\r\n\r\n");
		write(client_socket, buffer, strlen(buffer));
	}
	close(client_socket);
}
