#! /bin/bash

SAVED_PATH=$(pwd)
ROOT_PATH=$(cd $(dirname $0) && pwd);
cd $ROOT_PATH

. ./cfg.sh

if [ `pidof hyperdex-daemon | wc -w` -gt 0 ] ; then
echo -e "\n\033[32;1mStop hyperdex servers\033[0m"
killall hyperdex-daemon
sleep $DELAY
fi

if [ `pidof replicant-daemon | wc -w` -gt 0 ] ; then
echo -e "\n\033[32;1mStop hyperdex coordinators\033[0m"
killall replicant-daemon
sleep $DELAY
fi

if [ `pidof replicant-daemon | wc -w` -gt 0 ] ; then 
echo -e "\n\033[32;1mStop hyperdex coordinators with SIGKILL\033[0m"
killall -s SIGKILL replicant-daemon
sleep $DELAY
fi

cd $SAVED_PATH
