CC = gcc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I.
LDFLAGS = -L../../hcloud
LIBS = -lhcloud -lreadline

SERVER_OBJS = server.o
CLIENT_OBJS = client.o

all: server client

server: $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o server $(SERVER_OBJS) $(LIBS)

client: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o client $(CLIENT_OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f *.o server client

