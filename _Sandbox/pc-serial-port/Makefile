CXX = g++
EXE = serial.exe
SRC_MAIN = main.cpp
SRC_CAM= $(wildcard camera/*.cpp)
OBJ_MAIN = $(SRC_MAIN:.cpp=.o)
OBJ_CAM = $(patsubst camera/%.cpp,%.o,$(SRC_CAM)) 
INCLUDE=$(HPATH)
LIB=$(LIBPATH)

all: $(EXE)
$(EXE): $(OBJ_CAM) $(OBJ_MAIN)
	$(CXX) $^ -std=c++11 -L"$(LIB)" -lSVGenSDK64 -o $(EXE)
$(OBJ_MAIN): $(SRC_MAIN)
	$(CXX) -Wall -c $(SRC_MAIN) -I"$(INCLUDE)"
$(OBJ_CAM): $(SRC_CAM)
	$(CXX) -Wall -c $(SRC_CAM) -I"$(INCLUDE)"
	
clean:
	del $(OBJ_CAM) $(OBJ_MAIN) $(EXE)