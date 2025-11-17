CC = g++
CFLAGS = -Wall
LDFLAGS = -lGL -lm -ldl -lpthread -pthread -lX11 -lXcursor -lXi
SRC = src/main.cpp
OUT = build/sokol_app

all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)
	$(info $$DATA = $(DATA))

clean:
	rm -f $(OUT)
