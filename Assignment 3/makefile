export:main

main: main.o account.o
	gcc -o main.out  main.o account.o -lpthread -lm

main.o: main.c
	gcc -c main.c

account.o: account.c *h
	gcc -c account.c

print: lpr main.c
	lpr account.c

clean:
	rm *.out assignment_3_output_file.txt