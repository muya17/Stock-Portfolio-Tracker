# Makefile for Stock Portfolio Tracker (Linux/WSL)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
LDFLAGS = -lcurl

# Qt settings
QT_CXXFLAGS = $(shell pkg-config --cflags Qt5Widgets Qt5Core Qt5Gui) -fPIC
QT_LDFLAGS = $(shell pkg-config --libs Qt5Widgets Qt5Core Qt5Gui)
MOC = moc

# Targets
TARGET_CLI = portfolio_tracker
TARGET_GUI = portfolio_tracker_qt
SRC_CLI = main.cpp
SRC_GUI = main_qt.cpp mainwindow.cpp
MOC_SRC = moc_mainwindow.cpp

all: $(TARGET_CLI) $(TARGET_GUI)

# CLI version
$(TARGET_CLI): $(SRC_CLI) stock.hpp api.hpp http.hpp
	$(CXX) $(CXXFLAGS) $(SRC_CLI) -o $(TARGET_CLI) $(LDFLAGS)

# Qt GUI version
$(TARGET_GUI): $(SRC_GUI) $(MOC_SRC) stock.hpp api.hpp http.hpp mainwindow.h
	$(CXX) $(CXXFLAGS) $(QT_CXXFLAGS) $(SRC_GUI) $(MOC_SRC) -o $(TARGET_GUI) $(LDFLAGS) $(QT_LDFLAGS)

# Generate MOC file
$(MOC_SRC): mainwindow.h
	$(MOC) mainwindow.h -o $(MOC_SRC)

cli: $(TARGET_CLI)

gui: $(TARGET_GUI)

run-cli: $(TARGET_CLI)
	./$(TARGET_CLI)

run-gui: $(TARGET_GUI)
	./$(TARGET_GUI)

clean:
	rm -f $(TARGET_CLI) $(TARGET_GUI) $(MOC_SRC)

install-deps:
	@echo "Installing dependencies in Linux/WSL..."
	sudo apt update
	sudo apt install -y build-essential libcurl4-openssl-dev qtbase5-dev qt5-qmake

.PHONY: all cli gui run-cli run-gui clean install-deps
