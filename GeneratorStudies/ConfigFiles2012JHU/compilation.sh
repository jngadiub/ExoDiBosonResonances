#!/bin/bash

#700 1.77043
#800 2.02334
#900 2.27626
#1000 2.52918
#1100 2.7821
#1200 3.03502
#1300 3.28793
#1400 3.54085
#1500 3.79377
#1600 4.04669
#1700 4.29961
#1800 4.55252
#1900 4.80544
#2000 5.05836

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
