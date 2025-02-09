#include "../hcloud.h"

void	start_server(void)
{
	t_server	server;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int	client_socket;

	create_server_or(&server);
	printf("Servidor iniciado em localhost: na porta %d\n", PORTP);

	while (1)
	{
		client_socket = accept(server.server_socket, (struct sockaddr *)&client_addr, &client_len);
		if (client_socket == -1)
		{
			perror("Erro ao aceitar conexão");
			continue ;
		}

		char request[BUFFER_SIZE];
		int received = recv(client_socket, request, BUFFER_SIZE, 0);
		if (received > 0)
		{
			request[received] = '\0';
			http_request(client_socket, request, "./www");
		}
		else
			close(client_socket);
	}
}

int	main(void)
{
	start_server();
	return 0;
}
