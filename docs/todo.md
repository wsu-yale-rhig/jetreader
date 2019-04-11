## goals

1) concise & unified high level interface to provide a simple experience for anyone who is using default settings and workflow.
2) fully expose the underlying PicoDst data structures for those doing work outside of the normal analysis flow, doing QA, etc. (probably as easy as just publically inheriting from StPicoDstReader)

## current objectives for library 

1) automatic event/track/tower selection based on criteria selected at runtime.
2) tower id -> tower eta/phi mapping
3) tower eta -> corrected eta on-the-fly
4) hadronic correction (do we even include MIP? Seems like an opportunity for analysis errors. At the least, 100% hadronic correction should be set as default)
5) a method to return a list of fastjet::PseudoJets of all tracks and towers that have passed quality cuts, with towers with eta-correction and hadronic-correction.
  
## nice to haves

1) ability to set reader state via external configuration to reduce the frequency of recompilation. (JSON or XML parsing needed? Libraries are freely available. Worth considering)
2) logging and command line flag tools. I am considering building static lib versions of gflags and glog into libjetreader to allow for easier command line control and robust logging.

## for the future

1) it may be possible to optionally pull fastjet into the build structure, so that the user only needs ROOT installed. However, FastJet currently does not support cmake OR git, which would mean it must be manually implemented, and manually updated. Also, we want to avoid having any clashes with locally installed fastjet versions.