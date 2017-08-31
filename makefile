build:
	gcc -c ./libs/cJSON.c -o ./libs/cJSON.o; \
	gcc -c main.c -o main.o; \
	gcc ./libs/cJSON.o main.o -o app; \

run:
	make build; \
	./app; \