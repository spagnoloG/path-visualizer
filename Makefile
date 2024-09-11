CXX = g++
CXXFLAGS = -std=c++17 `pkg-config --cflags --libs opencv4`
LDFLAGS = -lvtkCommonCore -lvtkFiltersSources -lvtkRenderingCore -lhdf5 -lstdc++fs
SOURCES = src/main.cpp src/dataloader.cpp
INCLUDES = -Iinclude
TARGET = build/path_visualizer

all: $(TARGET)

$(TARGET): $(SOURCES)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET)
