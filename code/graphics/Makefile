PORT = 22
HOST = pi-la
NAME = main

CXX = arm-linux-gnueabihf-g++
AR = ar crs
CXXFLAGS = -D __raspberry__ -U __linux__

SYNCITEMS = /opt /usr/include /usr/lib /lib

BUILD_DIR 	= build/
BIN_DIR		= $(BUILD_DIR)bin/
OBJ_DIR		= $(BUILD_DIR)obj/
LIB_DIR		= $(BUILD_DIR)lib/
PCH_DIR 	= $(BUILD_DIR)pch/

EXEC_TARGETS = tests/%.cpp tests/%.c

PIROOT = $(HOME)/code/pi/root/

INC_DIR = ctl/
SRC_DIR = src/

VPATH = $(INC_DIR):\
		$(SRC_DIR):\
		$(PIROOT):\
		$(PIROOT)opt

IPATH = -Ictl -I$(PIROOT)usr/include -I$(PIROOT)opt/vc/include -I$(PIROOT)opt/vc/include/interface/vcos/pthreads -I$(PIROOT)opt/vc/include/interface/vmcs_host/linux
LDFLAGS =  -L$(PIROOT)usr/lib -lstdc++ -lm
LDFLAGS += -L$(PIROOT)/opt/vc/lib/ -lGLESv2 -lEGL -lstdc++ -lm -lbcm_host -lvcos -lvchiq_arm -llo

#OBJS = 

FORCE:

title:
	@echo "make tests for raspbi graphics"
	@echo $(PIROOT)

$(OBJ_DIR)%.o: $(OBJ_DIR)%.cpp %.h
	@echo "compiling $< to $@"
	$(CXX) $(CXXFLAGS) $(IPATH) -c $< -o $@ 

dir:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)

.PRECIOUS: $(EXEC_TARGETS)
$(EXEC_TARGETS) : title dir FORCE
	@echo "cross compiling executable" $@
	$(CXX) $(CXXFLAGS) $(IPATH) $@ -o $(BIN_DIR)$(NAME) $(LDFLAGS)
	@make deploy

sync:
	@rsync -avzL $(addPrefix($(HOST):, $(SYNCITEMS))) $(PIROOT)

deploy:
	scp -P $(PORT) $(BIN_DIR)$(NAME) $(HOST):/tmp
	ssh -t -p $(PORT) $(HOST) /tmp/$(NAME)
