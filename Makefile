NAME	= ircserv
CC		= c++
CFLAGS	= -Wall -Wextra -Werror -std=c++98 -g
SRC		= main.cpp \
			Server.cpp \
			Client.cpp \
			libX.cpp \
			Utils.cpp \
			Channel.cpp \
			privMsg.cpp \
			Join.cpp\
			Mode.cpp \
			Authentification.cpp \
			Topic.cpp \
			Invite.cpp \
			Kick.cpp \
			Mod.cpp \
			Names.cpp \
			
OBJ		= $(SRC:.cpp=.o)

all: $(NAME)

%.o: %.cpp
		$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
		 $(CC) $(OBJ) $(CFLAGS) -o $(NAME)

clean:
		rm -rf $(OBJ)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: re all fclean clean