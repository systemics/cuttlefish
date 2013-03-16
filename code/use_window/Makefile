PORT = 22
HOST = pi
NAME = main
ROOT = $(HOME)/code/pi/root

CXX = arm-linux-gnueabihf-g++
OPT += -Wall
OPT += -O3
INC += -I./
INC += -I$(ROOT)/usr/include/
INC += -I$(ROOT)/opt/vc/include/interface/vcos/pthreads/
INC += -I$(ROOT)/opt/vc/include/
LIB += -L$(ROOT)/opt/vc/lib/
LIB += -lEGL
LIB += -lGLESv2
LIB += -lbcm_host
LIB += -lvcos
LIB += -lvchiq_arm

$(NAME): $(NAME).o EGLConfig.o EGLWindow.o
	$(CXX) -o $@ $^ $(OPT) $(LIB)

header: EGLConfig.hpp.gch EGLWindow.hpp.gch

%.hpp.gch :: %.hpp
	$(CXX) -o $@ $< $(OPT) $(INC)

%.o :: %.cpp header
	$(CXX) -o $@ -c $< $(OPT) $(INC)

deploy:
	scp -P $(PORT) $(NAME) $(HOST):/tmp
	ssh -t -p $(PORT) $(HOST) /tmp/$(NAME)

clean:
	rm -f *.o *.gch $(NAME)
