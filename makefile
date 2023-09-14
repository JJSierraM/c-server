## Formatter
STYLE_BLUEPRINT=webkit
FORMATTER=clang-format -i -style=$(STYLE_BLUEPRINT)

all:
	make server
	make client

server: server.c
	gcc server.c -o server

client: client.c
	gcc client.c -o client

format: main.c
	$(FORMATTER) main.c
