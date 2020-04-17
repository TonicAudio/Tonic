Tonic
=====

Fast and easy audio synthesis in C++. 

Prefer coding to patching? Love clean syntax? Care about performance? That's how we feel too, and why we made Tonic.

~~~c++
//Tonic is a collection of signal generators and processors
TriangleWave tone1 = TriangleWave();
SineWave tone2 = SineWave();
SineWave vibrato = SineWave().freq(10);
SineWave tremolo = SineWave().freq(1);

//that you can combine using intuitive operators
Generator combinedSignal = (tone1 + tone2) * tremolo;
        
//and plug in to one another
float baseFreq = 200;
tone1.freq(baseFreq + vibrato * 10);
tone2.freq(baseFreq * 2 + vibrato * 20);
~~~

Here look. With Tonic you can create an awesome synthesizer with just a couple lines of code.

~~~c++
ControlXYSpeed speed = ControlXYSpeed().x(addParameter("x")).y(addParameter("y"));    
outputGen = RectWave().freq(100).pwm( 0.1 + (SineWave().freq(0.1) + 1) * 0.1) * SineWave().freq(1) >> LPF12().cutoff(100 + 600 * speed);
~~~

This code creates a rectangle wave oscillator, modulates the timbre of that oscillator by a sine wave, adds some slow tremelo, and sends it through a filter. 
On top of all that, it maps mouse speed to the cutoff frequency. 

You can browse more synth examples [here](http://github.com/TonicAudio/Tonic/tree/master/examples/Demo%20Synths)

### Example Projects

Tonic comes with several example projects, found in the "examples" directory. We plan to add more demo projects for Windows and Linux in the future.

- Several example synth patches in `Demo Synths`
- Standalone example using RTAudio for building a non-interactive synth patch that runs from the command line
- iOS example for making synths on your iPhone/iPad.

See [ofxTonic](https://github.com/TonicAudio/ofxTonic) for examples in openFrameworks.

##### Development

If you are interested in contributing to Tonic, please visit our [Wiki](https://github.com/TonicAudio/Tonic/wiki) and read the guidelines.
