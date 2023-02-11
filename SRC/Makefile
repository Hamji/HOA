
NAME = libhoa.a

SRC = Manager.cpp ManagerOdbc.cpp
OBJECTS = Manager.o ManagerOdbc.o

CFLGAGS = -Wall -Wextra -Werror
AR = ar rcs

RM = rm -f

CP = cp
LIB_PATH = ../LIB 


INCLUDE = /opt/homebrew/Cellar/unixodbc/2.3.11/include 

all: $(NAME)

$(NAME): $(SRC)
	g++ $(CFLAGS) -I $(INCLUDE) -lm  -c $(SRC) -std=c++20
	$(AR) $(NAME) $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)
	$(RM) ../LIB/*.a

re: fclean all

install:
	$(CP) $(NAME) $(LIB_PATH)
