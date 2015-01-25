#!/bin/bash

dir=${1:-.}
files=$(find $dir -type f -printf "%s %f\n"|sort -nr)
echo "$files" | sed -e 's/[0-9]//g' -e 's/ //g'
