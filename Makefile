CC = gcc
CFLAGS = `pkg-config --cflags gtk4` -I./$(HEADERDIR)
LIBS = `pkg-config --libs gtk4` -ljson-c

TARGET = tetris
SRCDIR = src
HEADERDIR = header
OBJDIR = obj

# Find all .c files in src directory
SRCS = $(wildcard $(SRCDIR)/*.c)
# Find all .h files in header directory
HEADERS = $(wildcard $(HEADERDIR)/*.h)
# Create object files list from source files
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

INCLUDES = -I./$(HEADERDIR)

run: $(TARGET)
	./$(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

# Compile source files to object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Create obj directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(TARGET) $(OBJS)

# Defines make command names to avoid conflicts with file names. If no command is given, 
# make will run the first target (not necessarily the first command).
.PHONY: clean run