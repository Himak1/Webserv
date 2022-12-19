# VARIABLES
NAME			:=	webserv
CFLAGS			:=	-Iinc #-Wall -Wextra -Werror -std=c++98
SANIT			:=	-g #-fsanitize=address
CC				:=	c++

# STATIC
RM				:=	rm
MKDIR_P			:=	mkdir -p
INC_DIR			:=	./inc
SRC_DIR			:=	./src
OBJ_DIR			:=	./obj
SRC				:=	main \
					TcpServer
					
OBJ				:=	$(SRC:%=$(OBJ_DIR)/%.o)
$(NAME)			: 	$(OBJ)
					$(CC) $(OBJ) -o $@ $(SANIT)
$(OBJ_DIR)/%.o	:	$(SRC_DIR)/%.cpp $(INC_DIR)/%.hpp
					$(MKDIR_P) $(dir $@)
					$(CC) $(CFLAGS) -c $< -o $@

.PHONY			:	all bonus clean fclean re
all				:	$(NAME)
bonus			:	$(NAME)
clean			:
					$(RM) -rf $(OBJ_DIR) 
fclean			:	clean
					$(RM) -f $(NAME)
re				: 	fclean all
