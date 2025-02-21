TARGET = downloader # executable

SOURCES = main.cpp downloader.cpp # source code files

CXX = g++ # compiler i am using

CXXFLAGS = -Wall -g # flags

OBJECTS = $(SOURCES:.cpp=.o) # for every source file make an object

LDLIBS = -lcurl # using libcurl

all: $(TARGET) # make the executable when i type in make

$(TARGET): $(OBJECTS) 
		$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDLIBS) -o $(TARGET)

# Rule to create object files
%.o: %.cpp
		$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up (remove executable and object files)
clean:
		rm -f $(TARGET) $(OBJECTS)

# Tell Make that "clean" and "all" are not real files
.PHONY: clean all