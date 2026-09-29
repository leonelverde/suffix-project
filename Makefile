CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude

SRCS := $(wildcard src/*.cpp) apps/app_main.cpp
HDRS := $(wildcard include/*.h)

# Detección de sistema operativo
ifdef OS
   EXE    := .exe
   RM_CMD = -@if exist $(TARGET) del /Q $(TARGET)
else
   EXE    :=
   RM_CMD = rm -f $(TARGET)
endif

TARGET := app_suffix$(EXE)

all: $(TARGET)

$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	$(RM_CMD)

.PHONY: all clean