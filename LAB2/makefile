all: 
	gcc -Wall -o client main_client.c -lpthread -std=c99
	gcc -Wall -o server main_server.c -lpthread -std=c99

clean:
	rm -f main_server
	rm -f main_client
	rm -f server
	rm -f client
	rm -f arrayRW

run_server:
	./server 3003 100

run_client:
	./client 3003 100
