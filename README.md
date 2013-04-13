Tonic
=====

Fast and easy audio synthesis in C++. 

Prefer coding to patching? Love clean syntax? Care about performance? That's how we feel too, and why we made Tonic. 

Here look. With Tonic you can create an awesome synthesizer with just a couple lines of code.


    ControlXYSpeed speed = ControlXYSpeed().x(addParameter("x")).y(addParameter("y"));    
    outputGen = RectWave().freq(100).pwm( 0.1 + (SineWave().freq(0.1) + 1) * 0.1) * SineWave().freq(1) >> LPF12().cutoff(100 + 600 * speed);

This code creates a rectangle wave oscillator, modulates that oscillator by a sine wave, adds some slow tremelo, and sends it through a filter. 
On top of all that, it maps mouse speed to the cutoff frequency. 

*Why don't you download Tonic and fire up the demo Xcode project right now?*

