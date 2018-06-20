TARGET=sensors_simulation
TARGET_BINARY=$(TARGET).bin
CPP_FLAGS=-W -Wall -g -std=c++11
SRC=$(wildcard *.cpp)
all: $(TARGET)
$(TARGET):
	$(CXX) $(CPP_FLAGS) $(SRC) -lpthread -o $(TARGET_BINARY)
clean:
	rm -f $(TARGET_BINARY)
