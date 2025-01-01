CC = gcc
CFLAGS = `pkg-config --cflags gtk4` -I$(HEADERDIR) -g -DDEBUG -O0
LIBS = `pkg-config --libs gtk4` -ljson-c

TARGET = bin/tetris
SRCDIR = src
HEADERDIR = include
OBJDIR = obj
BINDIR = bin

# Recursively find all .c files in src directory and subdirectories
SRCS = $(shell find $(SRCDIR) -type f -name '*.c')
# Find all .h files in header directory
HEADERS = $(wildcard $(HEADERDIR)/*.h)
# Create object files list from source files, maintaining directory structure
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

#When using VSC to debug this the following lines needed to be removed. 
#run: $(TARGET)
#	./$(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Create directories if they don't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(TARGET) $(OBJDIR) $(BINDIR)

# Defines make command names to avoid conflicts with file names. If no command is given, 
# make will run the first target (not necessarily the first command).
.PHONY: clean run