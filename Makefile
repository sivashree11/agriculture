# ==============================================================================
# Makefile for Smart Agricultural By-Product Marketplace
# Platform: Linux / Ubuntu (g++) & Windows (MinGW)
# ==============================================================================

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = marketplace

# Detect OS
ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    LDFLAGS := -lws2_32
    RM := del /f /q
else
    LDFLAGS := 
    RM := rm -f
endif

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET)
