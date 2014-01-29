CC=g++
OBJ = main.o user.o HashedFile.o

%.o: %.cpp
	$(CC) -g -c -o $@ $< 

linearhash: $(OBJ)
	g++ -g -o $@ $^

clean:
	rm *.o linearhash