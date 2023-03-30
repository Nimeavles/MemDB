COMPILER = g++
CFlags = -fdiagnostics-color=always -g

memdb.o: server.o main.o
	$(COMPILER) $(CFlags) server.o main.o -o memdb.o	

server.o: src/server.cpp
	$(COMPILER) $(CFlags) -c src/server.cpp

main.o: src/main.cpp
	$(COMPILER) $(CFlags) -c src/main.cpp

clean:
	@rm -v server.o main.o	