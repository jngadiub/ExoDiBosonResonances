import sys

# Very dumb script.
# Make the tmp.txt with cmsLs $PATHWITHFILES | grep root | awk '{print $5}' > tmp.txt
# Run this with makeCffInputFiles.py <name_of_outputfile.py>

f = open("tmp.txt","r")
o = open(sys.argv[1],"w")

o.write("import FWCore.ParameterSet.Config as cms\n\n")
o.write("cmgFiles = cms.untracked.vstring()\n")
o.write("source = cms.Source(\"PoolSource\",\n")
o.write("                                                noEventSort = cms.untracked.bool(True),\n")
o.write("                                                duplicateCheckMode = cms.untracked.string(\"noDuplicateCheck\"),\n")
o.write("                                                fileNames = cmgFiles\n")
o.write("                                                )\n\n")
o.write("cmgFiles.extend([\n")
o.write("\n")

for line in f:
	newline = line.rstrip()
	o.write("       '"+newline+"',\n")

o.write("    ])\n")
o.close()
f.close()
