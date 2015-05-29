audio: audio.cpp
	g++ -std=c++0x -Wall -D__LINUX_ALSA__ -I../../ext/gfx/ -I../../ext/cuttlebone/ -I../../ext/Gamma/ -o audio audio.cpp ../../ext/Gamma/src/Sync.cpp ../../ext/Gamma/src/SoundFile.cpp -lrtaudio -lasound -lpthread -lsndfile

clean:
	rm audio
