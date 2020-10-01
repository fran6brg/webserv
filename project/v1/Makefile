.PHONY: all clean fclean re

CC			= 	clang++
CPPFLAGS	= 	-Wall -Wextra -Werror -g
VERSION 	= 	-std=c++11
NAME 		= 	webserv
RM 			= 	rm -rf

SRC_PATH	= 	./srcs
LIB_PATH	= 	./srcs/methods
GNL_PATH	= 	./srcs/get_next_line
REQ_PATH	= 	./srcs/request

OBJ_PATH	= 	./objs
OBJLIB_PATH	= 	./objs
OBJGNL_PATH	= 	./objs
OBJREQ_PATH	= 	./objs

SRC_NAME	=  	Conf.cpp \
				main.cpp \
		   		Server.cpp \
				Client.cpp \
				Logger.cpp \
				Response.cpp \
				CGI.cpp \
				Utils.cpp \
				utils_tmp.cpp \
				Config_parser.cpp

LIB_NAME 	= 	connect.cpp \
		   		delete.cpp \
		   		get.cpp \
		   		option.cpp \
		   		put.cpp \
		   		trace.cpp \
		   		post.cpp

GNL_NAME 	=  	get_next_line.cpp \
				get_next_line_utils.cpp


REQ_NAME 	=  	Request.cpp \
				Parsing.cpp \
				Debug.cpp

OBJ_NAME	=	$(SRC_NAME:.cpp=.o)
OBJLIB_NAME	=	$(LIB_NAME:.cpp=.o)
OBJGNL_NAME	=	$(GNL_NAME:.cpp=.o)
OBJREQ_NAME	= 	$(REQ_NAME:.cpp=.o)

SRC			= 	$(addprefix $(SRC_PATH)/, $(SRC_NAME))
LIB			= 	$(addprefix $(LIB_PATH)/, $(LIB_NAME))
GNL			= 	$(addprefix $(GNL_PATH)/, $(GNL_NAME))
REQ			= 	$(addprefix $(REQ_PATH)/, $(REQ_NAME))

OBJ			= 	$(addprefix $(OBJ_PATH)/,$(OBJ_NAME))
OBJLIB		= 	$(addprefix $(OBJLIB_PATH)/,$(OBJLIB_NAME))
OBJGNL		= 	$(addprefix $(OBJGNL_PATH)/,$(OBJGNL_NAME))
OBJREQ		= 	$(addprefix $(OBJREQ_PATH)/,$(OBJREQ_NAME))


all: $(NAME)
	@touch ./www/perso/delete/to_delete
	@$(RM) ./www/tester/file_should_exist_after
	@$(RM) ./www/tester/multiple_same
	@$(RM) ./www/tester/temp_file
$(NAME): $(OBJ) $(OBJLIB) $(OBJGNL) $(OBJREQ)
	${CC} ${VERSION} ${CPPFLAGS} -o ${NAME} ${OBJ} ${OBJLIB} ${OBJGNL} $(OBJREQ)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	${CC} ${VERSION} -o $@ -c $<

$(OBJLIB_PATH)/%.o: $(LIB_PATH)/%.cpp
	@mkdir $(OBJLIB_PATH) 2> /dev/null || true
	$(CC) ${VERSION} -o $@ -c $<

$(OBJGNL_PATH)/%.o: $(GNL_PATH)/%.cpp
	@mkdir $(OBJGNL_PATH) 2> /dev/null || true
	$(CC) ${VERSION} -o $@ -c $<

$(OBJREQ_PATH)/%.o: $(REQ_PATH)/%.cpp
	@mkdir $(OBJREQ_PATH) 2> /dev/null || true
	$(CC) ${VERSION} -o $@ -c $<

clean:
	$(RM) $(OBJ) $(OBJLIB) $(OBJGNL) $(OBJREQ)

fclean: clean
	$(RM) $(NAME)
	$(RM) webserv.dSYM
	$(RM) log/*.txt
	$(RM) www/tester/file_should_exist_after
	$(RM) www/tester/multiple_same
	$(RM) www/tester/temp_file
	
launch: all
	./webserv conf/tester.conf

tester:
	../../intra/tester http://localhost:8080

ubuntu_launch: all
	./webserv conf/ubuntu_tester.conf

ubuntu_tester: 
	../../intra/ubuntu_tester http://localhost:8080

valgrind: all 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./webserv conf/ubuntu_tester.conf

valgrind_light: all 
	valgrind ./webserv conf/ubuntu_tester.conf

re:	fclean all

re: fclean all
