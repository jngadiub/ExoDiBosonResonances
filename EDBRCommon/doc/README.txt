

KinematicFit: 
cvs co -d Francesco/KinFitter/src UserCode/pandolf/KinematicFit

remove duplicate Files (we take them from the CMSSW release)
rm -f Francesco/KinFitter/src/T*
rm -f Francesco/KinFitter/src/LeptonNeutrinoKinFitter.*

remove spurious semi-colons in Francesco/KinFitter/src/GlobalFitter.cc

XXX => we probably want to update this with our own solution, that takes jet resolutions from the Db
