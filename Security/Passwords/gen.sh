#!/bin/bash

echo "Generating user/password pairs"
USER=1
while [ $USER ]
do
echo -n "user: "
read USER
if [[ $USER = "" ]] 
then
	exit 0
fi
echo -n "password: "
read PASSWD
echo -n $USER >> passwords.txt
echo -n " " >> passwords.txt
SALT=`openssl rand -base64 5`
echo -n $SALT >> passwords.txt
echo -n " " >> passwords.txt
ARG=`echo -n $SALT ' ' $PASSWD`
echo -n $ARG | shasum >> passwords.txt
done