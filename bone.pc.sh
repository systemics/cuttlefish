#!/bin/sh
DIRECTORY=`echo $1 |cut -d'/' -f1`
FILENAME=`echo $1 |cut -d'/' -f2 |cut -d'.' -f1 | sed -e "s|/|_|g"`
TARGET=${FILENAME}
RENDERTARGET="${TARGET}_RENDER"
SIMTARGET="${TARGET}_SIM"
echo Directory is $DIRECTORY
echo Filename is $FILENAME
echo RenderTarget Name is $RENDERTARGET
echo SIMTarget Name is $SIMTARGET

#if [ $2 lt 2 ]; then
#echo Compiling $1 as $SIMTARGET to SIMULATOR
#make $1 NAME=$SIMTARGET SIMBUILD=1 
#make copyone HOST=pi@192.168.7.2 NAME=$SIMTARGET

#fi
#if [ $2 lt 3 ]; then
echo Compiling $1 as $RENDERTARGET to RENDERERS 
make $1 NAME=$RENDERTARGET && make copy NAME=$RENDERTARGET #&& make many NAME=$RENDERTARGET
#fi



