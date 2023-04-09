#!/bin/sh

ExeOutName="RookieSweeper-Linux-x64"
SrcCodeDir="./src"
ReleaseDir="./release"
Platform="/Linux-x64"

# Make directory even if exists
mkdir -p ${ReleaseDir}

# Build
g++ ${SrcCodeDir}/*.cpp -o ${ReleaseDir}${Platform}/${ExeOutName} -std=c++20 -ggdb3
