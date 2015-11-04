#!/bin/bash

# Clones dependencies
# !Run this script from the project root!
# In terminal cd to project root then "sh cloneaddons.sh"

# switch to addons directory
cd ../../addons

# passing no arguments means read only
if [ -z $1 ]; then
CLONE="git clone https://github.com/"
echo "Cloning read-only"
else
CLONE="git clone git@github.com:"
echo "Cloning writable"
fi

# git clone specific revision of the addons
# this ensures that things won't break if repositories are updated

${CLONE}elliotwoods/ofxCvGui
# cd xxx
# git checkout xxx
# cd ..
${CLONE}elliotwoods/ofxKinectForWindows2
${CLONE}elliotwoods/ofxLiquidEvent
${CLONE}elliotwoods/ofxGrabCam
${CLONE}elliotwoods/ofxTextInputField
${CLONE}elliotwoods/ofxAssets
${CLONE}elliotwoods/ofxSingleton
${CLONE}elliotwoods/ofxAddonLib
${CLONE}arturoc/ofxTween
${CLONE}kylemcdonald/ofxCv

# final step is to cd back to the start dir
cd ../apps/ProjectPendleton