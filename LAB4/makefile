all: 
	gcc -Wall main_serial.c -o main_serial -lm -std=c99
	mpicc -g -Wall -o main main.c -lm -std=c99
	gcc datatrim.c -o datatrim
	./datatrim -i ./web-Stanford.txt -o ./data_input
	gcc serialtester.c Lab4_IO.c -o serialtester -lm 

run_serial:
	./main_serial

run:
	mpirun -np 4 ./main

clean:
	rm -f datatrim
	rm -f data_input
	rm -f main
	rm -f data_output
	rm -f main_serial
	rm -f serialtester

