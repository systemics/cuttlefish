PORT = 22
HOST = 192.168.0.9
#pi@192.168.2.4
#@192.168.2.2
NAME = main

CXX = arm-linux-gnueabihf-g++ -std=c++0x
AR = ar crs
CXXFLAGS = -D __raspberry__ -U __linux__  -D __LINUX_ALSA__

SYNCITEMS = opt usr/include usr/lib lib code

BUILD_DIR 	= build/
BIN_DIR		= $(BUILD_DIR)bin/
OBJ_DIR		= $(BUILD_DIR)obj/
LIB_DIR		= $(BUILD_DIR)lib/
PCH_DIR 	= $(BUILD_DIR)pch/
EXT_DIR		= ext/

EXEC_TARGETS = tests/%.cpp tests/%.c  examples/%.cpp

PIROOT = $(HOME)/pi-root/

INC_DIR = ctl/
SRC_DIR = src/

VPATH = $(EXT_DIR)/vsr/:\
		$(INC_DIR):\
		$(SRC_DIR):\
		$(PIROOT):\
		$(PIROOT)opt

IPATH += -Ictl
IPATH += -I$(PIROOT)usr/include
IPATH += -I$(PIROOT)usr/local/include
IPATH += -I$(PIROOT)opt/vc/include
IPATH += -I$(PIROOT)opt/vc/include/interface/vcos/pthreads
IPATH += -I$(PIROOT)opt/vc/include/interface/vmcs_host/linux
IPATH += -I$(EXT_DIR)/vsr/
IPATH += -I$(EXT_DIR)/gfx/
IPATH += -I$(EXT_DIR)/cuttlebone/

LDFLAGS += -L$(PIROOT)lib
LDFLAGS += -L$(PIROOT)usr/lib
LDFLAGS += -L$(PIROOT)usr/local/lib
LDFLAGS += -L$(PIROOT)opt/vc/lib 
LDFLAGS += -L$(EXT_DIR)Gamma/build/lib
LDFLAGS += -L$(EXT_DIR)vsr/build/lib
#LDFLAGS += -L$(PIROOT)lib/arm-linux-gnueabihf
#LDFLAGS += -L$(PIROOT)usr/lib/arm-linux-gnueabihf
LDFLAGS += -lEGL
LDFLAGS += -lGLESv2
#LDFLAGS += -lGamma
LDFLAGS += -lpthread
#LDFLAGS += $(PIROOT)usr/lib/arm-linux-gnueabihf/libportaudio.a
#LDFLAGS += $(PIROOT)usr/lib/arm-linux-gnueabihf/libjack.a
LDFLAGS += $(PIROOT)usr/lib/arm-linux-gnueabihf/libasound.so
LDFLAGS += -lGamma
#LDFLAGS += -lsndfile1
#LDFLAGS += -lasound
#LDFLAGS += -ljack
#LDFLAGS += -lportaudio
LDFLAGS += -lbcm_host
LDFLAGS += -llo
LDFLAGS += -lm
LDFLAGS += -lrt
LDFLAGS += -lstdc++
LDFLAGS += -lvchiq_arm
LDFLAGS += -lvcos
LDFLAGS += -lvsr

#USEVSR = 0
#VSRFLAGS = -I$(EXT_DIR)/vsr/ -I$(EXT_DIR)/gfx/ -lvsr 

OBJ_FILES = src/ctl_sound.cpp src/RtAudio.cpp

FORCE:

title:
	@echo "make tests for raspbi graphics"
	@echo $(PIROOT)

$(OBJ_DIR)%.o: $(OBJ_DIR)%.cpp %.h
	@echo "compiling $< to $@"
	$(CXX) $(CXXFLAGS) $(IPATH) -c $< -o $@ 

vsr: FORCE
	@make -C ext/vsr/ vsr RPI=1 GCC=1 #install DESTDIR=BUILDDIR = $(LIB_DIR) 

dir:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)

.PRECIOUS: $(EXEC_TARGETS)
$(EXEC_TARGETS) : title dir FORCE
	@echo "cross compiling executable" $@
	$(CXX) $(CXXFLAGS) $(IPATH) $(OBJ_FILES) $@ -o $(BIN_DIR)$(NAME) $(LDFLAGS) 
	#@make deploy

sync:
	@rsync -avzL $(addPrefix($(HOST):~/, $(SYNCITEMS))) $(PIROOT)

copy:
	parallel-scp -h hosts.txt build/bin/$(NAME) /tmp

many:
	parallel-ssh -h hosts.txt /tmp/$(NAME)

many-kill:
	parallel-ssh -h hosts.txt "pkill $(NAME)"

mpssh:
	mpssh -sqr build/bin/main

mpssh-kill:
	mpssh -s "pkill main"

kill:
	ssh -p $(PORT) $(HOST) "rm -f /tmp/$(NAME)"
	ssh -p $(PORT) $(HOST) "pkill $(NAME)"

deploy:
	ssh -p $(PORT) $(HOST) "rm -f /tmp/$(NAME)"
	scp -P $(PORT) $(BIN_DIR)$(NAME) $(HOST):/tmp
	ssh -t -p $(PORT) $(HOST) /tmp/$(NAME)
