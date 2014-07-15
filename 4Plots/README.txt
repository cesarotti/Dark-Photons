Map to '4Plots'

1. Root file
   Nomenclature
   TTree: "Signal"
   TFile: "Signal"
   Leaves: "numHits", "energyTot", "XPosition", "YPosition", "Particle_ID",
    "Theta"
   File: "complete.root"

2. Physics List
   The physics list I use is a G4VUserPhysics list. It is PositronPhysicsList1. It is included in many of my git files and in the file I uploaded for 'standard'. You have the options of a) using a Geant physics list (I suggest FTFP_BERT, which is instead a G4VModularPhysicsList), b) modifying my physics list, or c) including your own physics list. You change it in the file darkPhoton.cc. 

3. Hit Collection
   The hits and detectors we are using are BasicHit and BasicHitSD.

BasicHit keeps track of position, total energy of the particle, and the type of particle. 

BasicHitSD registers the hits. Here you can change the energy threshold to registering a hit, or include/exclude what particles you detect via PDGID. Whether smearing is used or not is defined in the DetectorConstructor.cc file, under the method ConstructSDandField(). SetSmearing(true) for smearing and false for none.

4. Build script: for fast compilation run ./buildstand.sh from the terminal

5. Plotting C File
   The file is called plots.C. To use it, go to the folder with your latest run data stored via the GUI file browser, and paste 'plots.C' into it. Then go to that directio in the terminal. Open ROOT from the terminal, then enter
 .L plots.C++ to compile or
 .x plots.C++ to compile and run
Plots should show up.
