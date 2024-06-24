CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -Wpedantic -Isrc -std=c++11
LDFLAGS = 

ifeq ($(shell uname -s),Linux)
	CXXFLAGS +=-fsanitize=leak
endif

ifdef FSAN
	CXXFLAGS +=-fsanitize=address
endif

ifdef BUG
	CXXFLAGS +=-g
endif

NAME = webserv

SRC_DIR = src
RESPONSE_DIR = response
LOGGER_DIR = log
SERVER_DIR = server
CONFIG_DIR = config
UTILS_DIR = utils
ERROR_DIR = error

SRC :=	$(wildcard $(SRC_DIR)/*.cpp) \
		$(wildcard $(SRC_DIR)/$(RESPONSE_DIR)/*.cpp) \
		$(wildcard $(SRC_DIR)/$(LOGGER_DIR)/*.cpp) \
		$(wildcard $(SRC_DIR)/$(UTILS_DIR)/*.cpp) \
		$(wildcard $(SRC_DIR)/$(SERVER_DIR)/*.cpp) \
		$(wildcard $(SRC_DIR)/$(ERROR_DIR)/*.cpp) \
		$(wildcard $(SRC_DIR)/$(CONFIG_DIR)/*.cpp)

OBJS_DIR = objs
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJS_DIR)/%.o)

I_DIRS := $(shell find . -type f -name "*.hpp" -exec dirname {} + | uniq)
INCLUDE = $(I_DIRS:%=-I%)

.PHONY: all clean fclean re

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(OBJS_DIR)

re: fclean all

$(NAME): $(OBJS_DIR) $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(LDFLAGS)

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

fsan:
	$(MAKE) FSAN=1 BUG=1
.PHONY: fsan

resan:	fclean fsan
.PHONY: resan

debug:
	$(MAKE) BUG=1
.PHONY: debug

rebug:	fclean debug
.PHONY: rebug