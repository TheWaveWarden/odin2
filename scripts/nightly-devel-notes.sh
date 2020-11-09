#!/bin/bash

cat <<- EOH
# Automated build of Odin, this is not the stable release

If you are looking for the most recent released version, visit https://thewavewarden.com/odin2

The downloads below are the packages created from the most recent commits on the devel branch.
They contain the newest features in development, but are to be tested.
Patches created by these builds are not guaranteed to be compatible to following releases.

EOH
date
echo ""
echo "Git commit:"
git log --pretty=oneline | head -1
