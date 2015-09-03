task: task.c
	mkdir -p bin
	clang  -Wall -pedantic -o bin/task task.c
	./bin/task
