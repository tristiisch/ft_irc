CXX					=	c++
CXXFLAGS			=	-Wall -Wextra -std=c++98
#CXXFLAGS			=	-Wall -Wextra -Werror -std=c++98
NAME				=	ircserv
LIB_FLAGS			=	-lc
INCLUDE_DIR			=	includes
SRCS_DIR			=	srcs
OBJS_DIR			=	.objs

SRCS				=	$(shell find $(SRCS_DIR) -type f -name '*.cpp')
INCLUDES			=	$(shell find $(INCLUDE_DIR) -type f -name '*.hpp')
OBJS				=	$(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)

all: $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp $(INCLUDES)
	mkdir -p $(dir $@) && $(CXX) $(CXXFLAGS) -c -o $@ $< -I $(INCLUDE_DIR)

$(NAME): $(OBJS) $(INCLUDES)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIB_FLAGS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) -r $(NAME)

re: fclean all

.PHONY: fclean all re
