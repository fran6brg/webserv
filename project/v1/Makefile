CC			=	clang++

VERSION		= 	-std=c++11 # -std=c++98

CPPFLAGS	=	-Wall -Werror -Wextra -g

RM			=	rm -rf

SRCS		=	srcs/main.cpp \
				srcs/Conf.cpp \
				srcs/Server.cpp \
				srcs/Client.cpp \
				srcs/Debug.cpp \
				srcs/Request.cpp \
				srcs/Response.cpp \
				srcs/CGI.cpp \
				srcs/Utils.cpp

OBJS		= 	$(SRCS:.cpp=.o)
EXEC		=	webserv

all:
	${CC} ${VERSION} ${CPPFLAGS} -o $(EXEC) $(SRCS)

clean:
	$(RM) $(OBJS)
	$(RM) ./a.out.dSYM

fclean: clean
	$(RM) $(EXEC)
	$(RM) webserv.dSYM

re:	fclean all

.PHONY:	all clean fclean re
