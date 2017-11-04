#! /bin/bash

DB=db
LOGS=logs

DELAY=3

#Fault Tolerance
#FT=2

#COORD_C=`expr $FT \* 2 + 1`
#SERV_C=`expr $FT + 1`

COORD_C=1
SERV_C=1

CHOST[1]=127.0.0.1
CHOST[2]=127.0.0.1
CHOST[3]=127.0.0.1
CHOST[4]=127.0.0.1
CHOST[5]=127.0.0.1

SHOST[1]=127.0.0.1
SHOST[2]=127.0.0.1
SHOST[3]=127.0.0.1

CPORT[1]=1982
CPORT[2]=1983
CPORT[3]=1984
CPORT[4]=1985
CPORT[5]=1986

SPORT[1]=2012
SPORT[2]=2013
SPORT[3]=2014