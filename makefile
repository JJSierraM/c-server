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

connect-to-jorge:
	./out/client $$(cat ips.txt | grep jorge-ip | cut -d' ' -f2) $$(cat ips.txt | grep jorge-port | cut -d' ' -f2)

# to-do: use tmux instead, e.g.
#    tmux split-window -h
#    tmux split-window -v
#
#    # Select the first pane and run the first command
#    tmux select-pane -t 0
#    tmux send-keys "$COMMAND1" Enter
#
#    # Select the second pane and run the second command
#    tmux select-pane -t 1
#    tmux send-keys "$COMMAND2" Enter
#
#    tmux select-pane -t 2
#    tmux send-keys "$COMMAND3" Enter
