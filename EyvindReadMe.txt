Steps I implemented to model positron annihilation

1. Define new material: liquid hydrogen (B2aDetectorConstruction.cc)

2. Set Verbosity (optional) in the RunManager (exampleB2a.cc)

3. Change beam to positron 

4. Define target solid

5. change number of tracking chambers to 2 (or remove completely)

6. Define CsI for crystals

7. Define photon detector solid. Boxes/ cylinders are easy. (B2aDetectorConstruction.cc)

8. Define photon detector logical volume

9. Place photo detector in world PV

10. describe sensitive detector class for photons. To only count photons, the PDGID of a photon (gamma) is 22

11. Define hit class

12. Define physics list (http://geant4.cern.ch/support/proc_mod_catalog/processes/)

13. Data storage (still working on that)


Let me know if you need help finding where some of this things are.

