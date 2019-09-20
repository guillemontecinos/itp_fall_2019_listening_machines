# Listening Machines - Assignments

## Week 1: Digital Reverb
Based on the code developed during class 1 I developed a digital reverb using the following [code](https://stackoverflow.com/questions/5318989/reverb-algorithm):

```
int delayMilliseconds = 500; // half a second
int delaySamples = 
    (int)((float)delayMilliseconds * 44.1f); // assumes 44100 Hz sample rate
float decay = 0.5f;
for (int i = 0; i < buffer.length - delaySamples; i++)
{
    // WARNING: overflow potential
    buffer[i + delaySamples] += (short)((float)buffer[i] * decay);
}
```
* [Further resources 1](https://dsp.stackexchange.com/questions/2792/high-quality-reverb-algorithm)
* [Further resources 2](http://sites.music.columbia.edu/cmc/MusicAndComputers/chapter5/05_02.php)

## Week 2
For this assignment I animated a pre built mesh I developed last semester. I realized that MFCC coeficients behave very organically so that would be a good idea to animate my mesh threads with the 13 coefs the algorithm calculates.
Because of time I couldn't use all coefs, so my next steps will be to interpolate all verts of the mesh with the 13 coefs, which also I'd like to normalize to have variations between 0 and 1.