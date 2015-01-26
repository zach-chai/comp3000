#!/bin/bash

nocase=1

while getopts ":f::i" opt; do
  case $opt in
    f)
      only=$OPTARG
      ;;
    i)
      nocase=0
      ;;
    \?)
      echo "Invalid option: -$OPTARG"
      ;;
    :)
      echo "Option -$OPTARG requires an argument."
  esac
done

shift $((OPTIND - 1))
cmd=$1

if [[ $cmd = "print" ]] ; then

  if [ ${#only} -gt 0 ] ; then
    if [ $nocase = 1 ] ; then
    books=$(cat ./books.txt | grep $only)
    else
      books=$(cat ./books.txt | grep -i $only)
    fi
  else
    books=$(cat ./books.txt)
  fi
  while read line
  do
    if [[ ${#line} -gt 1 ]] ; then
      author=${line%%:*}
      line=${line#*:}
      title=${line%%:*}
      line=${line#*:}
      publisher=${line%%:*}
      year=${line#*:}
      printf "%-20s %-20s\n" "Author(s):" "$author"
      printf "%-20s %-20s\n" "Booktitle:" "$title"
      printf "%-20s %-20s\n" "Publisher:" "$publisher"
      printf "%-20s %-20s\n\n" "Year:" "$year"
    fi
  done <<< "$books"
elif [[ $cmd -gt 0 ]] ; then
  echo "enter books here"
  for i in $(seq 1 $cmd)
  do
    echo "Author(s):"
    read author
    echo "Booktitle:"
    read title
    echo "Publisher:"
    read publisher
    echo "Year"
    read year
    echo "$author:$title:$publisher:$year" >> books.txt
  done
else
  echo "invalid"
fi