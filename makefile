CC = g++
CFLAGS = -Wall
LDFLAGS = -lGL -lm -ldl -lpthread -pthread -lX11 -lXcursor -lXi
INCLUDES = -I/src/header 
SRC = src/main.cpp
OUT = build/sokol_app

all:
	$(CC) $(INCLUDES) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)
	$(info $$DATA = $(DATA))

clean:
	rm -f $(OUT)
