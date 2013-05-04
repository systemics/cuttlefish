PORT = 22
HOST = pi@192.168.2.2
NAME = create_timer_man_example
ROOT = $(HOME)/code/pi/root

CXX = arm-linux-gnueabihf-gcc
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

$(NAME): $(NAME).o
	$(CXX) -o $@ $^ $(OPT) $(LIB)

%.o :: %.c
	$(CXX) -o $@ -c $< $(OPT) $(INC)

kill:
	ssh $(HOST) "pkill $(NAME) && rm /tmp/$(NAME)"

deploy:
	ssh -o 'LocalCommand=scp $(NAME) $(HOST):/tmp/' $(HOST) /tmp/$(NAME)

clean:
	rm -f *.o *.gch $(NAME)
