demo: libTonicLib.a
	g++ -I../../../src ../TonicStandaloneDemo/main.cpp -l rtaudio -L. -l TonicLib -o demo

libTonicLib.a:
	g++ -c -I../../../src -I../../../src/Tonic ../../../src/Tonic/*.cpp
	ar rc libTonicLib.a *.o

clean:
	rm *.o *.a demo*
