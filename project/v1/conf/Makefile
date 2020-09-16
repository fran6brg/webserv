CC			=	clang++

VERSION		= 	-std=c++11 # -std=c++98

CPPFLAGS	=	-Wall -Werror -Wextra -g

RM			=	rm -rf

GNL			=	srcs/get_next_line/get_next_line.cpp \
				srcs/get_next_line/get_next_line_utils.cpp

METHODS		=	srcs/methods/connect.cpp \
				srcs/methods/delete.cpp \
				srcs/methods/get.cpp \
				srcs/methods/head.cpp \
				srcs/methods/option.cpp \
				srcs/methods/put.cpp \
				srcs/methods/trace.cpp

SRCS		=	$(GNL) \
				$(METHODS) \
				srcs/main.cpp \
				srcs/Conf.cpp \
				srcs/Server.cpp \
				srcs/Client.cpp \
				srcs/Logger.cpp \
				srcs/Request.cpp \
				srcs/Response.cpp \
				srcs/CGI.cpp \
				srcs/Utils.cpp \
				srcs/utils_tmp.cpp \
				srcs/Config_parser.cpp

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
