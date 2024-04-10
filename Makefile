all:
	gcc -g main.c src/server.c src/clients.c src/http.c src/router.c src/controllers.c -o main -lpthread -Wall

clean:
	rm -vf main
