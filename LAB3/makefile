all: 
	gcc -Wall -o main main.c Lab3IO.c -fopenmp -std=c99
	gcc serialtester.c Lab3IO.c -o serialtester -lm
	gcc datagen.c Lab3IO.c -o datagen
	gcc test_program.c -o test_program -fopenmp -std=c99

gen3:
	./datagen -s 3 -b 100 -p -o ./data_input

gen100:
	./datagen -s 100 -b 100  -o ./data_input

gen1000:
	./datagen -s 1000 -b 100  -o ./data_input

gen3000:
	./datagen -s 3000 -b 100  -o ./data_input

test_program:
	./test_program

clean:
	rm -f main
	rm -f datagen
	rm -f data_input
	rm -f data_output
	rm -f test_program

run:
	./main 4

data:
	./datagen -s 3 -b 100 -p -o ./data_input