CC = g++
TARGET = app
OBJECTS = main.o database.o vector.o cmd.o

all: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)

.PHONY: all clean