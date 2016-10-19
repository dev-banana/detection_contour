ARG = ${MODE}

ifeq ($(ARG), "CLI")
	MODE = "CLI"
else
	MODE = "GUI"
endif


# MODE = "CLI" 

PROGRAM = tp1

BIN_DIR = bin
OBJ_DIR = obj
INCLUDE_DIR = -I/usr/include/ -I./inc


ifeq ($(MODE), "GUI")
	SRC_DIR = src/gui
	PACKAGES= opencv glibmm-2.4 glib-2.0 gtkmm-3.0 gtk+-3.0 gdkmm-3.0
else
	SRC_DIR = src/cli
	PACKAGES= opencv
endif


#*******

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(addprefix $(OBJ_DIR)/,$(notdir $(SRCS:.cpp=.o)))

#*******

CPPFLAGS = -g -Wall -std=c++11

LIB_DIR = -L/usr/include/
LIBS = -L/usr/local/lib -fopenmp -lglib-2.0 -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core

GTK_LDFLAGS = $(shell pkg-config --libs $(PACKAGES))

#*************************

#*******  FLAGS  *********

#*************************

LDFLAGS  = $(LIB_DIR) $(LIBS) $(GTK_LDFLAGS)
CFLAGS  = `pkg-config --cflags $(PACKAGES)`

#****************************

#******     REGLES     ******

#****************************

$(BIN_DIR)/$(PROGRAM): $(OBJ)

	g++ $(CPPFLAGS) $+ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp

	g++ $(CPPFLAGS) -c $(CFLAGS) $(INCLUDE_DIR) $< -o $@

	

###############################

default: $(BIN_DIR)/$(PROGRAM)

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/$(PROGRAM)

run : $(BIN_DIR)/$(PROGRAM)

	./$(BIN_DIR)/$(PROGRAM)
