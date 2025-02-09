#include "../hcloud.h"

int	main(void)
{
	t_client	client;
	configure_client(&client, "192.168.18.30");//ip do servidor

	const char *request = "GET index.html HTTP/1.1\r\nHost: 192.168.18.30\r\n\r\n";
	http_send(&client, request);

	return (0);
}
