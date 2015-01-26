cmd=$1

if [[ $cmd = "print" ]] ; then
  echo "print books here"
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