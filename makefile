CC = gcc

CLIENTSRC = client.c glad.c graphics/graphics.c graphics/graphics.h
SERVERSRC = server.c

BUILD_DIR = build

CFLAGS   = 	-I"C:/msys64/mingw64/include"
LDFLAGS  = 	-L"C:/msys64/mingw64/lib" -lglfw3 -lgdi32 -lopengl32

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

client: $(BUILD_DIR) $(CLIENTSRC)
	$(CC) -o $(BUILD_DIR)/client $(CLIENTSRC) $(CFLAGS) $(LDFLAGS)

server: $(BUILD_DIR) $(SERVERSRC) 
	$(CC) -o $(BUILD_DIR)/server $(SERVERSRC) $(CFLAGS) $(LDFLAGS)
