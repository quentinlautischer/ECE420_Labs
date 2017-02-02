build: 
	gcc -Wall -o main_client main_client.c -lpthread -std=c99
	gcc -Wall -o main_server main_server.c -lpthread -std=c99
	gcc -Wall -o arrayRW arrayRW.c -lpthread -std=c99

run_server:
	./main_server 3001 100

run_client:
	./main_client 3001 100

run_arrayRW:
	./arrayRW 100