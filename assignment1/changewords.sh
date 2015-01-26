#!/bin/bash

files=$(find -type f)
for file in $files
do
  content=$(cat $file)
  echo "${content//CU/Carleton}" > $file
done