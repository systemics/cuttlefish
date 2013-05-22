PORT = 22
HOST = pi-z
#NAME = mtp
NAME = playsaw
ROOT = $(HOME)/code/pi/root

CXX = arm-linux-gnueabihf-g++ -std=c++0x
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

$(NAME): $(NAME).o RtAudio.o
	$(CXX) -o $@ $^ $(OPT) $(LIB)

%.o :: %.cpp
	$(CXX) -o $@ -c $< $(OPT) $(INC)

kill:
	ssh $(HOST) "pkill $(NAME) && rm /tmp/$(NAME)"

mpssh:
	mpssh -l home -s -r mtp

deploy:
	ssh $(HOST) "rm -f /tmp/$(NAME)"
	scp $(NAME) $(HOST):/tmp
	ssh -t $(HOST) /tmp/$(NAME)

clean:
	rm -f *.o *.gch $(NAME)
