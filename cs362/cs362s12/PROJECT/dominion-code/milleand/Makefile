NAME = dominion
LIB = lib$(NAME).a
EXE = playdom
TEST = testdom

SHELL = sh
CC = gcc
AR = ar
REMOVE = rm -f
REMOVEDIR = rm -rf

LIB_SRC =	dominion.c \
			rngs.c \

EXE_SRC =	playdom.c \

TEST_SRC =	GinkoTest.c \
			main.c \
			rngs.c \
			suite.c \
			unit.c \

DEBUG = -g
OPTIMIZATION = -O3
INCLUDEDIRS = 
OBJDIR = obj

CFLAGS =	$(INCLUDEDIRS) \
			-Wall \
			-Wextra \
			-Wmissing-prototypes \
			-Wmissing-declarations \
			-Wstrict-prototypes \
			-std=gnu99 \
			$(OPTIMIZATION) \
			$(DEBUG) \
			-fprofile-arcs \
			-ftest-coverage \

LIBDIRS =	-L.
LDFLAGS =	$(LIBDIRS) -lm

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MMD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
ALL_CFLAGS = $(CFLAGS) $(GENDEPFLAGS)

LIB_OBJ = $(LIB_SRC:%.c=$(OBJDIR)/%.o)
EXE_OBJ = $(EXE_SRC:%.c=$(OBJDIR)/%.o)
TEST_OBJ = $(TEST_SRC:%.c=$(OBJDIR)/%.o)

all: $(LIB) $(EXE) $(TEST)

$(LIB): $(LIB_OBJ)
	@echo
	@echo Creating shared library: $@
	@$(AR) q $@ $^ 2> /dev/null

# Link object files to executable
$(EXE): $(EXE_OBJ)
	@echo
	@echo Linking: $@
	@$(CC) -o $@ $(ALL_CFLAGS) $^ -l$(NAME) $(LDFLAGS) 

# Link object files to executable
$(TEST): $(TEST_OBJ)
	@echo
	@echo Linking: $@
	@$(CC) -o $@ $(ALL_CFLAGS) $^ -l$(NAME) $(LDFLAGS) 

# Compile: create object files from C source files.
$(OBJDIR)/%.o : %.c
	@echo Compiling: $<
	@$(CC) -c $(ALL_CFLAGS) $< -o $@ 

clean:
	@echo Removing: $(LIB)
	@$(REMOVE) $(LIB)
	@echo Removing: $(EXE)
	@$(REMOVE) $(EXE)
	@echo Removing: $(TEST)
	@$(REMOVE) $(TEST)
	@echo Removing: $(LIB_SRC:%.c=$(OBJDIR)/%.o)
	@$(REMOVE) $(LIB_SRC:%.c=$(OBJDIR)/%.o)
	@echo Removing: $(EXE_SRC:%.c=$(OBJDIR)/%.o)
	@$(REMOVE) $(EXE_SRC:%.c=$(OBJDIR)/%.o)
	@echo Removing: $(TEST_SRC:%.c=$(OBJDIR)/%.o)
	@$(REMOVE) $(TEST_SRC:%.c=$(OBJDIR)/%.o)
	@echo Removing: $(LIB_SRC:.c=.d)
	@$(REMOVE) $(LIB_SRC:.c=.d)
	@echo Removing: $(EXE_SRC:.c=.d)
	@$(REMOVE) $(EXE_SRC:.c=.d)
	@echo Removing: $(TEST_SRC:.c=.d)
	@$(REMOVE) $(TEST_SRC:.c=.d)
	@echo Removing directory: .dep
	@$(REMOVEDIR) .dep
	@echo Removing directory: $(OBJDIR)
	@$(REMOVEDIR) $(OBJDIR)

# Create object files directory
$(shell mkdir $(OBJDIR) 2>/dev/null)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# Listing of phony targets.
.PHONY : clean
