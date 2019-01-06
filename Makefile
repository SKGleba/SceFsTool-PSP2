CC=gcc
CFLAGS=
LDFLAGS=
SOURCES=main.c
EXECUTABLE=psp2scefstool
all:
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(EXECUTABLE)
clean:
	rm -rf $(EXECUTABLE)
