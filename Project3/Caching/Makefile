BIN=arc

all: $(OBJ)
	$(CXX) -o $(BIN) $(wildcard *.cpp) -std=c++11 -pthread -lpthread -ggdb

clean:
	rm $(BIN)
