#!/bin/bash

host="http://localhost:3000"
for i in `seq 1 7`
do
  key=$RANDOM
  f=$(fortune)
  value=$(echo $f | cut -d ' ' -f 1 )
  #echo "($key,$value)"
  echo "($i,$value)"
  #curl -X POST "$host/value?key=$key&value=$value" &
  curl -X POST "$host/value?key=$i&value=$value" & 2>/dev/null
done

