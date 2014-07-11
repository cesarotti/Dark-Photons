#! /bin/bash

if [ -a brems.root ]
    then
        rm -f brems.root
fi

if [ -a inelastic.root ]
    then
        rm -f inelastic.root
fi

hadd brems.root brems/*
hadd inelastic.root inelastic/*

echo "MY SCRIPT: Gathered data from each brems/ and inelastic/ folders and put them into appropriate files"