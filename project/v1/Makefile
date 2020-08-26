CC			=	clang++

VERSION		= 	# -std=c++98

CPPFLAGS	=	-Wall -Werror -Wextra -g

RM			=	rm -rf

SRCS		=	main.cpp
OBJS		= 	$(SRCS:.cpp=.o)
EXEC		=	webserv

all:
	${CC} ${VERSION} ${CPPFLAGS} -o $(EXEC) $(SRCS)

clean:
	$(RM) $(OBJS)
	$(RM) ./a.out.dSYM

fclean: clean
	$(RM) $(EXEC)

re:	fclean all

.PHONY:	all clean fclean re