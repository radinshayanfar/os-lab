all: server client

run_server: server
	@./server.out

run_client: client
	@./client.out

server:
	@rm -f server.out
	@gcc server.c -lpthread -o server.out

client:
	@rm -f client.out
	@gcc client.c -lpthread -o client.out

clean:
	@rm *.out
