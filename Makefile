#---------------------
# feedback-system Makefile
#
# ---------------------
EXE = Feedback

CXXFLAGS = -I/home/visakoe1/ohjelmointi/rapidjson/include -Wall
LDFLAGS=-g
LDLIBS= -lSDL2main -lSDL2 -lSDL2_ttf

SRC_DIR = src
OBJ_DIR = obj
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)		
$(info $$SRC is [${SRC}])
$(info $$OBJ is [${OBJ}])

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
