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

format: client.c server.c
	$(FORMATTER) server.c client.c
