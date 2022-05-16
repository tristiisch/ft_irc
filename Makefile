CXX					=	c++
#CXXFLAGS			=	-Wall -Wextra -std=c++98 -g3
CXXFLAGS			=	-Wall -Wextra -std=c++98 -g3 -fsanitize=address
#CXXFLAGS			=	-Wall -Wextra -Werror -std=c++98
NAME				=	ircserv
NAME_CLIENT			=	client

LIB_FLAGS			=
INCLUDE_DIR			=	includes
SRCS_DIR			=	srcs
OBJS_DIR			=	.objs
OBJS_DIR_DEBUG		=	.objs_debug
OBJS_DIR_CLIENT		=	.objs_client

ifeq ($(OS),Windows_NT)

# SRCS				=	$(shell dir * $(SRCS_DIR) /S /B | findstr \.cpp)
SRCS				=	$(shell FORFILES /S /M *.cpp)
# INCLUDES			=	$(shell dir * $(INCLUDE_DIR) /S /B  | findstr \.hpp)
INCLUDES			=	$(shell FORFILES /S /M *.hpp)

else

SRCS				=	$(shell find $(SRCS_DIR) -type f -name '*.cpp')
INCLUDES			=	$(shell find $(INCLUDE_DIR) -type f -name '*.hpp')
SRCS_CLIENT			=	client.cpp

endif
OBJS				=	$(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
OBJS_DEBUG			=	$(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR_DEBUG)/%.o)
OBJS_CLIENT			=	$(SRCS_CLIENT:%.cpp=$(OBJS_DIR_CLIENT)/%.o)

all: $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp $(INCLUDES)
	@printf '\r[⚪] Compile %s ... ' $(basename $@)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $< -I $(INCLUDE_DIR)
	@printf '\r[🟢] Compile %s\n' $(basename $@)

$(OBJS_DIR_DEBUG)/%.o: $(SRCS_DIR)/%.cpp $(INCLUDES)
	@printf '\r[⬜] Compile %s DEBUG ... ' $(basename $@)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $< -I $(INCLUDE_DIR) -D DEBUG_MODE=1
	@printf '\r[🟪] Compile %s DEBUG\n' $(basename $@)

$(OBJS_DIR_CLIENT)/%.o: %.cpp
	@printf '\r[⬜] Compile %s Client ... ' $(basename $@)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@printf '\r[🟪] Compile %s Client\n' $(basename $@)

$(NAME): $(OBJS) $(INCLUDES)
	@printf '\r[⚪] Link %s ... ' $(NAME)
	@$(CXX) $(CXXFLAGS) $(OBJS) $(LIB_FLAGS) -o $(NAME)
	@printf '\r[🟢] Link %s\n' $(NAME)

debug: $(OBJS_DEBUG) $(INCLUDES)
	@printf '\r[⬜] Link %s DEBUG ...' $(NAME)
	@$(CXX) $(CXXFLAGS) $(OBJS_DEBUG) $(LIB_FLAGS) -o $(NAME)
	@printf '\r[🟪] Link %s DEBUG\n' $(NAME)

client: $(OBJS_CLIENT)
	@printf '\r[⬜] Link %s ...' $(NAME_CLIENT)
	@$(CXX) $(CXXFLAGS) $(OBJS_CLIENT) -o $(NAME_CLIENT)
	@printf '\r[🟪] Link %s\n' $(NAME_CLIENT)

kill:
	@printf '[⚪] Kill PID ...\n'
	@kill "$(pidof ircserv)"
	@netstat -anp|grep 6667
	@sudo fuser -k 6667/tcp
	@printf '[🟢] Kill PID \n'

clean:
	@printf '\r[⚪] Delete *.o ... '
	@$(RM) $(OBJS) $(OBJS_DEBUG) $(OBJS_CLIENT)
	@printf '\r[🟢] Delete *.o\n'

fclean: clean
	@printf '\r[⚪] Delete %s ... ' $(NAME)
	@$(RM) -r $(NAME) $(OBJS_DIR) $(OBJS_DIR_DEBUG) $(OBJS_DIR_CLIENT) log.txt commands.txt
	@printf '\r[🟢] Delete %s\n' $(NAME)

re: fclean all

.PHONY: fclean all re debug kill
