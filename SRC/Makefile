
NAME = libhoa.a

SRC = Manager.cpp ManagerOdbc.cpp
OBJECTS = Manager.o ManagerOdbc.o

CFLGAGS = -Wall -Wextra -Werror
AR = ar rcs

RM = rm -f

CP = cp
LIB_PATH = ../LIB 


INCLUDE = #INSERT SQL INCLUDE PATH 
LIBOPTION = #INSERT SQL INCLUDE PATH

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) -I $(INCLUDE)  -c $(SRC) -std=c++20
	$(AR) $(NAME) $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)
	$(RM) ../LIB/*.a

re: fclean all

install:
	$(CP) $(NAME) $(LIB_PATH)
