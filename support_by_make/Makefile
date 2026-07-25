# Makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = collect_plugins
SOURCES = collect_plugins.cpp

all: $(TARGET)
	./$(TARGET) $(ARGS)  # Автоматический запуск после сборки

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

.PHONY: all clean