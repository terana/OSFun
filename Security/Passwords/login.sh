#!/bin/bash

echo "Authorisation"
echo -n "user: "
read USER
echo -n "password: "
read PASSWD
IFS=' '
arr=(`cat passwords.txt | grep $USER`)
if [ "${arr[0]}" != "$USER" ]
then
	echo "NO"
	exit 0
fi
HAVE=`echo -n ${arr[1]} $PASSWD`
HAVE=(`echo -n $HAVE | shasum`)
WANT=${arr[2]}
if [ "$WANT" == "$HAVE" ]
then
	echo "OK"
else
	echo "NO"
fi
