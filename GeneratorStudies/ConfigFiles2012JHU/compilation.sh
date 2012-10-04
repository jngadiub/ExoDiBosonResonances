#!/bin/bash

sed -e 's/MASSRESONANCE/600/' -e 's/WIDTHRESONANCE/0.095/' mod_Parameters_template.F90 > mod_Parameters.F90
make
mv JHUGen JHUGen600
sed -e 's/MASSRESONANCE/1000/' -e 's/WIDTHRESONANCE/0.158/' mod_Parameters_template.F90 > mod_Parameters.F90
make
mv JHUGen JHUGen1000
sed -e 's/MASSRESONANCE/1500/' -e 's/WIDTHRESONANCE/0.237/' mod_Parameters_template.F90 > mod_Parameters.F90
make
mv JHUGen JHUGen1500

time ./JHUGen600 Collider=1 Process=2 VegasNc1=10000 PChannel=0 DecayMode1=0 DecayMode2=1 DataFile=ADPS600
time ./JHUGen1000 Collider=1 Process=2 VegasNc1=10000 PChannel=0 DecayMode1=0 DecayMode2=1 DataFile=ADPS1000
time ./JHUGen1500 Collider=1 Process=2 VegasNc1=10000 PChannel=0 DecayMode1=0 DecayMode2=1 DataFile=ADPS1500
