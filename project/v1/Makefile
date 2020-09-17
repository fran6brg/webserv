.PHONY: all clean fclean re norme

CC = clang++

FLAGS = -Wall -Wextra -Werror -g

VERSION     =   -std=c++11 # -std=c++98

NAME = webserv

SRC_PATH = ./srcs
LIB_PATH = ./srcs/methods
GNL_PATH = ./srcs/get_next_line
OBJ_PATH = ./obj
OBJLIB_PATH = ./obj
OBJGNL_PATH = ./obj

SRC_NAME =  Conf.cpp \
			main.cpp \
            Server.cpp \
            Client.cpp \
            Logger.cpp \
            Request.cpp \
            Response.cpp \
            CGI.cpp \
            Utils.cpp \
            utils_tmp.cpp \
            Config_parser.cpp

LIB_NAME = connect.cpp \
           delete.cpp \
           get.cpp \
           head.cpp \
           option.cpp \
           put.cpp \
           trace.cpp \
		   post.cpp

GNL_NAME =  get_next_line.cpp \
			get_next_line_utils.cpp

OBJGNL_NAME = $(GNL_NAME:.cpp=.o)
OBJ_NAME = $(SRC_NAME:.cpp=.o)
OBJLIB_NAME = $(LIB_NAME:.cpp=.o)

SRC = $(addprefix $(SRC_PATH)/, $(SRC_NAME))
LIB = $(addprefix $(LIB_PATH)/, $(LIB_NAME))
GNL = $(addprefix $(GNL_PATH)/, $(GNL_NAME))
OBJ = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))
OBJLIB = $(addprefix $(OBJLIB_PATH)/,$(OBJLIB_NAME))
OBJGNL = $(addprefix $(OBJGNL_PATH)/,$(OBJGNL_NAME))

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJ) $(OBJLIB) $(OBJGNL)
	${CC} ${VERSION} ${CPPFLAGS} -o ${NAME} ${OBJGNL} ${OBJLIB} ${OBJ}

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir $(OBJ_PATH) 2> /dev/null || true
	${CC} ${VERSION} -o $@ -c $<

$(OBJLIB_PATH)/%.o: $(LIB_PATH)/%.cpp
	@mkdir $(OBJLIB_PATH) 2> /dev/null || true
	$(CC) ${VERSION} -o $@ -c $<

$(OBJGNL_PATH)/%.o: $(GNL_PATH)/%.cpp
	@mkdir $(OBJGNL_PATH) 2> /dev/null || true
	$(CC) ${VERSION} -o $@ -c $<

clean:
	$(RM) $(OBJ) $(OBJLIB) $(OBJGNL)

fclean: clean
	$(RM) $(NAME)
	$(RM) webserv.dSYM

tester: 
	../../intra/tester http://localhost:8080

re:	fclean all

re: fclean all
