# Listenint Machines Notes

## Class 1
* PCM: Pulse-code modulation
* [Essentia](https://essentia.upf.edu/documentation/documentation.html)
* threading in coding

## Class 2: Signal processing (kind of)
In a signal processing pipeline some filters are applied
* Frame cutter: cuts a frame
* Windowing: normalizes the frame to get rid of pops at the ends
* Spectrum: calculates the FFT
* pools in essentia are databases
* Pools are [Circular Buffers](https://en.wikipedia.org/wiki/Circular_buffer)

## Class 3
* [Justin Salamon](http://www.justinsalamon.com/)
* Similarity matrix
* Music synchoronization
* [HPCP](https://en.wikipedia.org/wiki/Harmonic_pitch_class_profiles): Beat detection, Beatogram in Essentia
* [Essentia algorithm overview](https://essentia.upf.edu/documentation/algorithms_overview.html)
* `YamlOutput` exports the data on a pool as a Yaml or JSON 

```c++
if(recording){
    Pool &pool = mltk.pool;
    const char* stats[4] = {"mean", "var", "min", "max"};
    standard::Algorithm* aggr = standard::AlgorithmFactory::create("PoolAggregator", "defaultStats", arrayToVector<>;
}
```

* [scikit](https://scikit-learn.org) is ML in py
* Magenta [GANSynth](https://magenta.tensorflow.org/gansynth)
* [Git Stashing](https://calendar.google.com/calendar/r/week)

# Class 4
* [Vibert Thio](https://vibertthio.com/portfolio/projects/)
* [Fundamentals of Musical Composition - Schoenberg Arnold](https://monoskop.org/images/d/da/Schoenberg_Arnold_Fundamentals_of_Musical_Composition_no_OCR.pdf)
* [TWENTIETH CENTURY HARMONY - VINCENT PERSICHETTI](http://www.thereelscore.com/PortfolioStuff/PDFFiles/PersichettiNotes.pdf)
* [Dan Tepfer](http://www.dantepfer.com/)
* Maps in C++, like dictionaries
```c++
map<string, float> stringFloatMap;
```