NAME		=	PmergeMe
CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -MMD

SRCS		=	main.cpp \
				PmergeMe.cpp

OBJS		=	$(SRCS:.cpp=.o)
DEPS		=	$(SRCS:.cpp=.d)

HEADERS		=	PmergeMe.hpp

RESET		=	\033[0m
GREEN		=	\033[32m
YELLOW		=	\033[33m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)Executable $(NAME) compiled successfully!$(RESET)"

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) $(DEPS) $(NAME).d
	@echo "$(YELLOW)Object files and dependencies removed!$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(YELLOW)Executable $(NAME) removed!$(RESET)"

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
