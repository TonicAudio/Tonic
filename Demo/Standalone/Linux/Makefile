demo: libTonicLib.a
	g++ -m32 -I../../../src ../TonicStandaloneDemo/main.cpp -l rtaudio -L. -l TonicLib -o demo

libTonicLib.a:
	g++ -m32 -c -I../../../src -I../../../src/Tonic ../../../src/Tonic/*.cpp
	ar rc libTonicLib.a *.o

clean:
	rm -rf *.a *.o demo
