#!/bin/bash

cat <<- EOH
# Nightly Build of Devel Branch

### This is not the stable release, if you're unsure what to download, use the stable release from https://thewavewarden.com/odin2

The downloads below are the packages created from the most recent commits on the devel branch.
They contain the newest features in development, but are to be tested.
Presets created with these builds are not guaranteed to be compatible to following releases.

EOH
date
echo ""
echo "Git commit:"
git log --pretty=oneline | head -1
