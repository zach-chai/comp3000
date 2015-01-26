#!/bin/bash

files=$(find -type f)
for file in $files
do
  # echo "Carleton is a guy who goes to Carleton" >> $file
  content=$(cat $file)
  echo "${content//UC/Carleton}" > $file
done