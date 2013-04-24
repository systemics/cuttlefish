#MAKEFILE FOR MAKING ON THE RASPBERRY PI ITSELF

CXX=g++

#define raspberry and undefine linux
CXXFLAGS = -D __raspberry__ -U __linux__

#libraries
LDFLAGS =  -L/usr/lib -L/opt/vc/lib/ -lGLESv2 -lEGL -lstdc++ -lm -lbcm_host -lvcos

#include path
IPATH = -Ictl -I/usr/include -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads

#executables
EXEC= tests/%.cpp tests/%.c

BIN_DIR = build/bin/
OBJ_DIR = build/obj/

OBJS = ctl_gl_shader.o 


FORCE:

title:
	@echo "make tests for raspbi graphics"


$(OBJ_DIR)%.o: $(OBJ_DIR)%.cpp %.h
	@echo "compiling $< to $@"
	$(CXX) $(CXXFLAGS) $(IPATH) -c $< -o $@ 

dir:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

.PRECIOUS: $(EXEC)
$(EXEC) : title dir FORCE
	@echo "making executable"
	$(CXX) $(CXXFLAGS) $(IPATH) $@ -o $(BIN_DIR)$(*F) $(LDFLAGS)
	@cd $(BIN_DIR) && ./$(*F)
