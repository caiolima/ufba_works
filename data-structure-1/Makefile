CC=g++
OBJ = main.o HashedFile.o node.o user.o

%.o: %.cpp
	$(CC) -g -c -o $@ $< 

btreehash: $(OBJ)
	g++ -g -o $@ $^

clean:
	rm *.o btreehash