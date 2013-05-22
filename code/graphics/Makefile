PORT = 22
HOST = pi@192.168.2.4
#@192.168.2.2
NAME = main

CXX = arm-linux-gnueabihf-g++ -std=c++0x
#AR = ar crs
CXXFLAGS = -D __raspberry__ -U __linux__

SYNCITEMS = opt usr/include usr/lib lib code

BUILD_DIR 	= build/
BIN_DIR		= $(BUILD_DIR)bin/
OBJ_DIR		= $(BUILD_DIR)obj/
LIB_DIR		= $(BUILD_DIR)lib/
PCH_DIR 	= $(BUILD_DIR)pch/
EXT_DIR		= ../../ext/

EXEC_TARGETS = tests/%.cpp tests/%.c

PIROOT = $(HOME)/code/pi/root/

INC_DIR = ctl/
SRC_DIR = src/

VPATH = $(EXT_DIR)/vsr/build/pch/:\
		$(INC_DIR):\
		$(SRC_DIR):\
		$(PIROOT):\
		$(PIROOT)opt

IPATH = -Ictl -I$(PIROOT)usr/include -I$(PIROOT)opt/vc/include -I$(PIROOT)opt/vc/include/interface/vcos/pthreads -I$(PIROOT)opt/vc/include/interface/vmcs_host/linux
LDFLAGS =  -L$(PIROOT)usr/lib -lstdc++ -lm
LDFLAGS += -L$(PIROOT)/opt/vc/lib/ -lGLESv2 -lEGL -lstdc++ -lm -lbcm_host -lvcos -lvchiq_arm -llo -lrt

USEVSR = 0
VSRFLAGS = -L$(EXT_DIR)/vsr/build/lib/ -lvsr -I$(EXT_DIR)/vsr/build/pch/ -I$(EXT_DIR)/vsr/ -I$(EXT_DIR)/vsr/VSR/Elements/ -I$(EXT_DIR)/vsr/VSR/


FORCE:

title:
	@echo "make tests for raspbi graphics"
	@echo $(PIROOT)

$(OBJ_DIR)%.o: $(OBJ_DIR)%.cpp %.h
	@echo "compiling $< to $@"
	$(CXX) $(CXXFLAGS) $(IPATH) -c $< -o $@ 

# vsr:
# 	@make --no-print-directory -C $@ #install DESTDIR=BUILDDIR = $(LIB_DIR)

dir:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)

.PRECIOUS: $(EXEC_TARGETS)
$(EXEC_TARGETS) : title dir FORCE
	@echo "cross compiling executable" $@
	$(CXX) $(CXXFLAGS) $(IPATH) $@ -o $(BIN_DIR)$(NAME) $(LDFLAGS) $(VSRFLAGS)
	#@make deploy

sync:
	@rsync -avzL $(addPrefix($(HOST):~/, $(SYNCITEMS))) $(PIROOT)

copy:
	parallel-scp -h hosts.txt build/bin/main /tmp

many: copy
	parallel-ssh -h hosts.txt /tmp/main

many-kill:
	parallel-ssh -h hosts.txt "pkill main"

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
