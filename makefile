## Formatter
STYLE_BLUEPRINT=webkit
FORMATTER=clang-format -i -style=$(STYLE_BLUEPRINT)

CLIENT=./src/client.c
SERVER=./src/server.c

all:
	make server
	make client

server: $(SERVER)
	gcc $(SERVER) -o ./out/server

client: $(CLIENT)
	gcc $(CLIENT) -o ./out/client

format: client.c server.c
	$(FORMATTER) $(SERVER) $(CLIENT)

exec:
	gnome-terminal --tab --title="Client 1" -- ./out/client 127.0.0.1 8080
	gnome-terminal --tab --title="Client 2" -- ./out/client 127.0.0.1 8080
	gnome-terminal --tab --title="Client 3" -- ./out/client 127.0.0.1 8080
	gnome-terminal --tab --title="Client 1" -- ./out/client 127.0.0.1 34197
	gnome-terminal --tab --title="Client 2" -- ./out/client 127.0.0.1 34197
	gnome-terminal --tab --title="Client 3" -- ./out/client 127.0.0.1 34197
