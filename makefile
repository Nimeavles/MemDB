COMPILER = g++
CFlags = -fdiagnostics-color=always -g

memdb.out: server.o main.o client.o
	$(COMPILER) $(CFlags) server.o main.o client.o -o memdb.out	

server.o: src/server.cpp
	$(COMPILER) $(CFlags) -c src/server.cpp

main.o: src/main.cpp
	$(COMPILER) $(CFlags) -c src/main.cpp

client.o: src/client.cpp
	$(COMPILER) $(CFlags) -c src/client.cpp

clean:
	@rm -v server.o main.o client.o	