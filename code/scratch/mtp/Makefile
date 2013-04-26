.SUFFIXES:

%.o : %.cpp
	g++ -c $< -o $@

mtp: mtp.o
	g++ mtp.o -o mtp

run: mtp
	./mtp /dev/input/event0

clean:
	rm *.o mtp
