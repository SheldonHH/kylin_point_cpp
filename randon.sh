#!/bin/bash
length=12
i=1

seq=(1 2 3 4 5 a b c d e f g h i j k l m n o p q r s t u v w x y z)

num_seq=${#seq[@]}

while [ "$i" -le "$length" ]
do
	 seqrand[$i]=${seq[$((RANDOM%num_seq))]}
	  let "i=i+1"
  done

  for j in ${seqrand[@]}
  do
	   echo -n $j
   done
   echo