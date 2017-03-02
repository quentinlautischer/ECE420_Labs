all: 
	gcc -Wall -o main main.c Lab3IO.c -lpthread -std=c99
	gcc datagen.c Lab3IO.c -o datagen
	./datagen -s 3 -b 100 -p -o ./data_input

clean:
	rm -f main
	rm -f datagen
	rm -f data_input
	rm -f data_output

run:
	./main 4

data:
	./datagen -s 3 -b 100 -p -o ./data_input