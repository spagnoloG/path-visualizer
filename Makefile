CXX = g++
CXXFLAGS = -std=c++17 `pkg-config --cflags opencv4` -lstdc++fs
LDFLAGS = `pkg-config --libs opencv4` -lvtkCommonCore -lvtkFiltersSources -lvtkRenderingCore -lhdf5

all: dataset_loader

dataset_loader: main.cpp
	$(CXX) $(CXXFLAGS) -o dataset_loader main.cpp $(LDFLAGS)

clean:
	rm -f dataset_loader
