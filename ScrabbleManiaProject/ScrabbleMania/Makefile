# Luis Barajas
# Eric Parton

# Simple example of a Makefile

### Variables for this project ###
# These should be the only ones that need to be modified
# The files that must be compiled, with a .o extension
OBJECTS = fatal_error.o sockets.o Player.o ClientScrabble.o Scrabble.o trim.o
# The header files
DEPENDS = scrabble_structs.h sockets.h fatal_error.h Player.h ClientScrabble.h Scrabble.h trim.h
# The executable programs to be created
CLIENT = Client
SERVER = Server
# SCRABBLE = Scrabble
# PLAYER = Player

# Name of the project / zipfile
MAIN = scrabble

### Variables for the compilation rules ###
# These should work for most projects, but can be modified when necessary
# The compiler program to use
CC = g++ -pthread
# Options to use when compiling object files
# NOTE the use of gnu99, because otherwise the socket structures are not included
#  http://stackoverflow.com/questions/12024703/why-cant-getaddrinfo-be-found-when-compiling-with-gcc-and-std-c99
CFLAGS = -Wall -g -pedantic # -O2
# Options to use for the final linking process
# This one links the math library
# LDLIBS = -lpthread

### The rules ###
# These should work for most projects without change
# Special variable meanings:
#   $@  = The name of the rule
#   $^  = All the requirements for the rule
#   $<  = The first required file of the rule

# Default rule
# all: $(CLIENT) $(SERVER)
# all:  $(CLIENT) $(PLAYER) $(SCRABBLE) $(SERVER)
all:  $(CLIENT) $(SERVER)

# Rule to make the client program
$(CLIENT): $(CLIENT).o $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# Rule to make the server program
$(SERVER): $(SERVER).o $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# Rule to make the scrabble program
# $(SCRABBLE): $(SCRABBLE).o $(OBJECTS)
# 	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

	# Rule to make the player program
# $(PLAYER): $(PLAYER).o $(OBJECTS)
# 	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# Rule to make the object files
%.o: %.cpp $(DEPENDS)
	$(CC) $< -c -o $@ $(CFLAGS)

# Clear the compiled files
clean:
	rm -rf *.o $(CLIENT) $(SERVER)

# Create a zip with the source code of the project
# Useful for submitting assignments
# Will clean the compilation first
zip: clean
	zip -r $(MAIN).zip *

# Indicate the rules that do not refer to a file
.PHONY: clean all zip
