CC=g++
OBJ = main.o user.o HashedFile.o page.o

%.o: %.cpp
	$(CC) -g -c -o $@ $< 

linearhash: $(OBJ)
	g++ -g -o $@ $^

clean:
	rm *.o linearhash