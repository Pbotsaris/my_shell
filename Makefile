SRC=src
OBJ=obj
BIN=bin

CFLAGS +=-W  -Wall -Wextra -g3 -Iinclude -fsanitize=address

CFLAGS_NO_SANINATIZE +=-W -Wall -Wextra -g3 -Iinclude

CC=gcc
TARGET=$(BIN)/my_zsh
RM=rm -rf

SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

all: $(TARGET)

with_leak: $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS_NO_SANINATIZE) 

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS) 

$(OBJ)/%.o: $(SRC)/%.c 
	${CC} ${CFLAGS} -c $< -o $@

clean:
	$(RM) $(TARGET) $(BIN)/*.dSYM $(OBJ)/*.o 
