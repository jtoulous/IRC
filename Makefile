NAME	= IRC
CC		= c++
CFLAGS	= -Wall -Wextra -Werror -std=c++98 -g
SRC		= main.cpp \
			Server.cpp \
			Client.cpp \
			libX.cpp \
			Utils.cpp \
			Channel.cpp \
			privMsg.cpp \
			Join.cpp \
			Authentification.cpp \
			Topic.cpp \
			Invite.cpp \
			Kick.cpp \
			Mode.cpp \
			iMode.cpp 

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