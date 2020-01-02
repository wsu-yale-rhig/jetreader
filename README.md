# WSU Jet Group StPicoDst Reader 

### Nick Elsey & Putschke Group, Wayne State University

Libjetreader provides a high-level wrapper on top of the STAR StPicoDstReader that enables simple interfacing to FastJet tools for jet finding while following the WSU jet group's common analysis needs and patterns, such as event quality cuts, track and tower quality cuts, and integration of bad tower lists and bad run lists. The library also allows for the use of YAML configuration files to save the complete state of a jetreader object, which can be useful for bookkeeping and reproducibility during experimentation.

## Requirements

Libjetreader has been designed and tested on Linux and MacOS operating systems, and requires a compiler supporting the C++17 standard. The library has a few external dependencies that must be installed prior to building. 

1) ROOT, the high-energy physics data analysis framework. Builds tested with ROOT 6. (https://root.cern.ch/)
2) FastJet, the jet finding software package. Builds tested with Fastjet 3.3.X (http://fastjet.fr/)

Some third-party dependencies are required at build time but are included as git submodules. (See installation instructions for details)

1) YAML-CPP (https://github.com/jbeder/yaml-cpp)
2) StPicoEvent - STAR StPicoDst framework (https://github.com/nickelsey/StPicoEvent) 
3) googletest - if tests are enabled (https://github.com/google/googletest)
4) benchmark - if tests are enbabled (https://github.com/google/benchmark)

## Installation 

Once ROOT and FastJet are installed, building should be relatively simple

```
git clone --recurse-submodules git@github.com:nickelsey/jetreader.git
cd jetreader && mkdir build && cd  build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/location ..
make && make install
```

## Examples

A short example can be found in ```jetreader/examples/test_reader.cc ```, that illustrates the basic workflow of using libjetreader to analyze StPicoDst data from STAR.