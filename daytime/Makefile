# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = daytime_client
SOURCE = main.cpp

# Сборка по умолчанию
all: $(TARGET)

# Сборка клиента
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Очистка
clean:
	rm -f $(TARGET)

# Пересборка
rebuild: clean all

.PHONY: all clean rebuild
