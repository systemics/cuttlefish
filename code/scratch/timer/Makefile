HOST = nonce-pi
NAME = test_timer
ROOT = $(HOME)/code/pi/root

CXX = arm-linux-gnueabihf-g++
OPT += -Wall
OPT += -O3
INC += -I./
INC += -I$(ROOT)/usr/include/
INC += -I$(ROOT)/opt/vc/include/interface/vmcs_host/linux/
INC += -I$(ROOT)/opt/vc/include/interface/vcos/pthreads/
INC += -I$(ROOT)/opt/vc/include/
LIB += -L$(ROOT)/opt/vc/lib/
LIB += -lrt
LIB += -lEGL
LIB += -lGLESv2
LIB += -lbcm_host
LIB += -lvcos
LIB += -lvchiq_arm

$(NAME): $(NAME).o Timer.o
	$(CXX) -o $@ $^ $(OPT) $(LIB)

%.o :: %.cpp
	$(CXX) -o $@ -c $< $(OPT) $(INC) $(LIB)

kill:
	ssh $(HOST) "pkill $(NAME) ; rm /tmp/$(NAME)"

deploy:
	scp $(NAME) $(HOST):/tmp/
	ssh $(HOST) /tmp/$(NAME)

clean:
	rm -f *.o *.gch $(NAME)
