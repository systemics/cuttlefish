_: gamma

PIROOT = /home/vagrant/pi-root
#INCLUDE += -I./
DEFINE += -D__raspberry__ -D__LINUX__
INCLUDE += -I./rtaudio/
INCLUDE += -I$(PIROOT)/usr/include/
INCLUDE += -I$(PIROOT)/usr/local/include/
#LIBRARY += -L$(PIROOT)/usr/lib/
LIBRARY += -L$(PIROOT)/usr/local/lib/
LIBRARY += -L$(PIROOT)/usr/lib/arm-linux-gnueabihf/
LINK += -lasound -lGamma

clean:
	rm -f gamma *.o

RtAudio.o:
	g++ -D__LINUX_ALSA__ -c rtaudio/RtAudio.cpp $(DEFINE) $(INCLUDE) $(LIBRARY) $(LINK)

gamma: RtAudio.o gamma.cpp
	g++ -o main $(DEFINE) $(INCLUDE) $(LIBRARY) $(LINK) RtAudio.o gamma.cpp
