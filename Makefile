CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -MMD -MP
TARGET   := bin/app_suffix

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,obj/%.o,$(SRCS)) obj/app_main.o

# Detección de sistema operativo para comandos de consola
ifdef OS
   RM = del /Q /F /S
   FIX_PATH = $(subst /,\,$1)
   MKDIR = if not exist $(subst /,\,$1) mkdir $(subst /,\,$1)
else
   RM = rm -rf
   FIX_PATH = $1
   MKDIR = mkdir -p $1
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	@$(call MKDIR,bin)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

obj/%.o: src/%.cpp
	@$(call MKDIR,obj)
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/app_main.o: apps/app_main.cpp
	@$(call MKDIR,obj)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@$(call RM,obj bin)

-include $(OBJS:.o=.d)

.PHONY: all clean
