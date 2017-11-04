#! /bin/bash

SAVED_PATH=$(pwd)
ROOT_PATH=$(cd $(dirname $0) && pwd);
cd $ROOT_PATH

echo -e "\n\033[34;1mStop HyperDex claster and clear database\n\033[0m"

. cfg.sh

if [ `pidof hyperdex-daemon | wc -w` -gt 0 ] ; then
echo -e "\n\033[32;1mStop hyperdex servers\033[0m"
killall hyperdex-daemon
sleep 5
fi

#if [ `pidof replicant-daemon | wc -w` -gt 0 ] ; then
#hyperdex wait-until-stable -h 127.0.0.1 -p 1982
#fi

if [ `pidof replicant-daemon | wc -w` -gt 0 ] ; then
echo -e "\n\033[32;1mStop hyperdex coordinators\033[0m"
killall replicant-daemon
sleep 5
fi

if [ `pidof replicant-daemon | wc -w` -gt 0 ] ; then 
echo -e "\n\033[32;1mStop hyperdex coordinators with SIGKILL\033[0m"
killall -s SIGKILL replicant-daemon
sleep 1
fi

echo -e "\n\033[32;1mRemove logs and database\n\033[0m"

I=1
while [ $I -le $COORD_C ]
do
rm -Rf $DB/coord$I
rm -Rf $LOGS/coord$I
I=`expr $I + 1`
done

I=1
while [ $I -le $SERV_C ]
do
rm -Rf $DB/data$I
rm -Rf $LOGS/serv$I
I=`expr $I + 1`
done

cd $SAVED_PATH


