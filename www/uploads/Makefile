# VARIABLES
NAME			:=	webserv
CFLAGS			:=	-Isrc -Wall -Wextra -Werror -std=c++98 -pedantic
SANIT			:=	-g -fsanitize=address
CC				:=	@c++

# STATIC
RM				:=	@rm
MKDIR_P			:=	@mkdir -p
SRC_DIR			:=	./src
OBJ_DIR			:=	./obj
SRC				:=	$(shell find $(SRC_DIR) -name '*.cpp')
OBJ				:=	$(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
$(NAME)			: 	$(OBJ)
					$(CC) $(OBJ) -o $@ $(SANIT)
$(OBJ_DIR)/%.o	:	$(SRC_DIR)/%.cpp $(SRC_DIR)/%.hpp
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

# TO DO:
# install depencies:
# - brew install cmake #only for tester
# - brew install php  ?

# $(NAME)			: 	$(OBJ)
# 					which php-cgi || brew install php
# 					$(CC) $(OBJ) -o $@ $(SANIT)


