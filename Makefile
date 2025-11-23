NAME = projet_complex

SRC = src/main.c \
      src/init.c \
      src/free.c \
      src/voisins.c \
      src/utils.c \
      src/couverture.c \
      src/fichier.c \
      src/suppression.c \
      src/branch.c \
      src/bound.c \
      src/branch_ameliore.c \
      src/branch_common.c

OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I./includes -g
LDFLAGS = -lm

TARGET = projet_complex

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re