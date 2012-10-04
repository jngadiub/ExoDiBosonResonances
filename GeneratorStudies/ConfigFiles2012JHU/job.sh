#!/bin/bash

export MYDIR=`pwd`
echo "Untarring generator..."
tar -xzf JHUGenerator.v2.1.4.tar.gz
cp mod_Parameters_template.F90 JHUGenerator
cp compilation.sh JHUGenerator
cd JHUGenerator
echo "Compiling and testing generator..."
source compilation.sh
echo "Running generator with parameters:"
echo $@
$@
cd $MYDIR
mv JHUGenerator/*lhe .
