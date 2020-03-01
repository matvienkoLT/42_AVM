#SET FLOAT PRECISION
#make SET_PRECISION=5 set_precision

# Name of the program.
NAME     := avm
FOLDER_SRC = srcs

# Sources and objects.
SRCS     := $(FOLDER_SRC)/main.cpp	\
			$(FOLDER_SRC)/parser_class.cpp \
			$(FOLDER_SRC)/IOperand.cpp \
			$(FOLDER_SRC)/haystack_class.cpp \
			$(FOLDER_SRC)/console_autocomplete.cpp \

OBJS     := $(SRCS:.cpp=.o)

# Define all the compiling flags.
CXX			:= clang++
FLAGS		:= -std=c++1z -Wall -Werror -Wextra
SET_PRECISION := 3

# Compile and create everything.
all: $(NAME)


# Change a compiler.
set_cxx:
	$(eval CXX = clang++-4.0)

# Set developer mode.
set_dev_flags:
	$(eval FLAGS += -D DEVELOPER_MODE)

set_precision_flags:
	$(eval FLAGS += -D SET_PRECISION=$(SET_PRECISION))

# Compile the program in developer mode.
dev:clean set_dev_flags $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o $(NAME)

set_precision:clean set_precision_flags $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o $(NAME)

clang4:clean set_cxx $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o $(NAME)

# Compile the program with the objects.
$(NAME): $(OBJS)
	$(CXX) $(FLAGS) $(OBJS) -o $@

# # This won't run if the source files don't exist or are not modified.
%.o: %.cpp
		$(CXX) $(FLAGS) -I ./includes/ -o $@ -c $<

# Rule to remove all the object files.
clean:
	@rm -f $(OBJS)
	@rm -f *.gch
	@echo "[INFO] Objects removed!"

# Rule to remove everything that has been created by the makefile.
fclean: clean
	@rm -f $(NAME)
	@echo "[INFO] $(NAME) removed!"

# Rule to re-make everything.
re:     fclean all

# Makes sure that gnu make will still run even if files called
# clean / fclean / all and re already exist in the directory
.PHONY: all clean fclean re