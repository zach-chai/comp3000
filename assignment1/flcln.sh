#!/bin/bash

dir=${1:-.}
files=$(find $dir -type f -printf "%s %f\n"|sort -nr)
echo "$files" | sed -e 's/[0-9]//g' -e 's/ //g'
files0=$(find $dir -type f -size 0)
if [[ -n "$files0" ]] ; then
  echo "There are files with size 0"
  echo "$files0"
  echo "would you like to delete? (yes|no)"
  read response
else
  echo "no files size 0"
fi
if [[ "$response" = "yes" ] ; then
  $(find $dir -type f -size 0 -delete)
fi