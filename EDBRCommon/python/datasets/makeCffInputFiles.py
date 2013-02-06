import sys

# Very dumb script.
# Make the tmp.txt with cmsLs $PATHWITHFILES | grep root | awk '{print $5}' > tmp.txt
# Run this with makeCffInputFiles.py <name_of_outputfile.py>

f = open("tmp.txt","r")
o = open(sys.argv[1],"w")

o.write("import FWCore.ParameterSet.Config as cms\n")
o.write("maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )\n")
o.write("readFiles = cms.untracked.vstring()\n")
o.write("secFiles = cms.untracked.vstring()\n")
o.write("source = cms.Source ('PoolSource',fileNames = readFiles, secondaryFileNames = secFiles)\n")
o.write("readFiles.extend([\n")

i = 0
for line in f:
    newline = line.rstrip()
    if i is 255:
        o.write("]);\n")
        o.write("readFiles.extend([\n")
        i = 0
    o.write("'"+newline+"',\n")
    i = i+1

o.write("]);\n")
o.close()
f.close()
