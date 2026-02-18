# Makefile for Stock Portfolio Tracker (Linux/WSL)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
LDFLAGS = -lcurl

TARGET = portfolio_tracker
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC) stock.hpp api.hpp http.hpp
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

install-deps:
	@echo "Installing libcurl and build tools in WSL..."
	sudo apt update
	sudo apt install -y build-essential libcurl4-openssl-dev

.PHONY: all run clean install-deps
