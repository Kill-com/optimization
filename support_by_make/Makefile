ifeq ($(OS),Windows_NT)
    # Windows
    EXT = .dll
    CXXFLAGS += -shared
    LDFLAGS += -Wl,--out-implib,lib$(NAME).a
    RM = del /Q
    MKDIR = mkdir
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        # Linux
        EXT = .so
        CXXFLAGS += -shared -fPIC
    else ifeq ($(UNAME_S),Darwin)
        # macOS
        EXT = .dylib
        CXXFLAGS += -dynamiclib
    else
        # Unknown -  Linux
        EXT = .so
        CXXFLAGS += -shared -fPIC
    endif
endif

CXX = c++
CXXFLAGS += -std=c++17 -Wall -O2
LDFLAGS +=

SRC_DIR ?= 
BUILD_DIR = build
PLUGIN_DIR ?=

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')


PLUGINS = $(patsubst $(SRC_DIR)/%.cpp,%,$(SOURCES))


TARGETS = $(addprefix $(PLUGIN_DIR)/, $(addsuffix $(EXT), $(PLUGINS)))



.PHONY: all clean info

all: $(PLUGIN_DIR) $(TARGETS)

$(PLUGIN_DIR):
	@mkdir -p $(PLUGIN_DIR)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(PLUGIN_DIR)/%$(EXT): $(SRC_DIR)/%.cpp | $(PLUGIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)


clean:
	rm -rf $(PLUGIN_DIR)/*$(EXT)
	rm -rf $(BUILD_DIR)

info:
	@echo "========================================"
	@echo "OS: $(shell uname -s 2>/dev/null || echo Windows)"
	@echo "Extension: $(EXT)"
	@echo "Source folder: $(SRC_DIR)"
	@echo "Plugin folder: $(PLUGIN_DIR)"
	@echo "Found plugins: $(PLUGINS)"
	@echo "========================================"