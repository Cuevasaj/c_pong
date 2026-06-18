CC      = clang
CFLAGS  = -Wall -Wextra -I/usr/local/opt/raylib/include
LDFLAGS = -L/usr/local/opt/raylib/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

TARGET  = pong
SRC     = main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean
