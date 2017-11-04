#! /bin/bash

SAVED_PATH=$(pwd)
ROOT_PATH=$(cd $(dirname $0) && pwd);
cd $ROOT_PATH

C_CNT=`pidof replicant-daemon | wc -w`
S_CNT=`pidof hyperdex-daemon | wc -w`

if [ $C_CNT -gt 0 -o $S_CNT -gt 0 ] ; then

  echo -e "\n\033[31;1mThe HyperDex claster is already started. There are other scripts to manage the started clasters\n\033[0m"
  exit 1;
  
fi

echo -e "\n\033[34;1mStart HyperDex claster\n\033[0m"

. ./cfg.sh

function waitCoordinator
{
  #$1 - coordinator's index
  cr=1
  if ps ax | grep "replicant-daemon.*`echo \"${CHOST[$1]}\" | sed -e 's/\./\\\./g'`.*${CPORT[$1]}" > /dev/null; then
    while ! replicant conn-str -h ${CHOST[$1]} -p ${CPORT[$1]} > /dev/null 2>&1
    do
      if ! ps ax | grep "replicant-daemon.*`echo \"${CHOST[$1]}\" | sed -e 's/\./\\\./g'`.*${CPORT[$1]}" > /dev/null 2>&1; then
        return 1;
      fi
      echo -n "."
      sleep 0.1
      cr=0
    done
    return 0
  fi

#  return 1
  
  #Or:
#  cr=1
#  while ! replicant poke -h ${CHOST[$1]} -p ${CPORT[$1]} 1>/dev/null 2>&1
#  do
#    if ! ps ax | grep "replicant-daemon.*`echo \"${CHOST[$1]}\" | sed -e 's/\./\\\./g'`.*${CPORT[$1]}" > /dev/null 2>&1; then
#      return 1;
#    fi
#    echo -n "."
#    sleep 1
#    cr=0
#  done
  
#  if [ $cr -eq 0 ] ; then echo; fi
  
#  return 0
}

mkdir -p $DB && mkdir -p $LOGS || exit 1

echo -e "\n\033[32;1mStart coordinator 1: ${CHOST[1]}:${CPORT[1]} Data: $DB/coord1 Logs: $LOGS/coord1\033[0m"

mkdir -p $DB/coord1 && mkdir -p $LOGS/coord1 && hyperdex coordinator -d -l ${CHOST[1]} -p ${CPORT[1]} -L $LOGS/coord1 -D $DB/coord1 || exit 1
if ! waitCoordinator 1 ; then
  echo -e "\n\033[31;1mUnable to start first coordinator\n\033[0m"
  exit 1
fi

I=2
while [ $I -le $COORD_C ]
do

echo -e "\n\033[32;1mStart coordinator $I: ${CHOST[$I]}:${CPORT[$I]} connected to ${CHOST[1]}:${CPORT[1]} Data: $DB/coord$I Logs: $LOGS/coord$I\033[0m"

mkdir -p $DB/coord$I && mkdir -p $LOGS/coord$I || exit 1
hyperdex coordinator -d -l ${CHOST[$I]} -p ${CPORT[$I]} -c ${CHOST[1]} -P ${CPORT[1]} -L $LOGS/coord$I -D $DB/coord$I || exit 1
I=`expr $I + 1`
done

I=1
while [ $I -le $SERV_C ]
do

echo -e "\n\033[32;1mStart daemon $I: ${SHOST[$I]}:${SPORT[$I]} connected to ${CHOST[$I]}:${CPORT[$I]} Data: $DB/data$I Logs: $LOGS/serv$I\033[0m"

mkdir -p $DB/data$I && mkdir -p $LOGS/serv$I || exit 1
if ! waitCoordinator $I ; then
  echo -e "\n\033[31;1mServer $I can't be started because corresponding coordinator $I was not started.\n\033[0m"
else
  hyperdex daemon -d -l ${SHOST[$I]} -p ${SPORT[$I]} -c ${CHOST[$I]} -P ${CPORT[$I]} -D $DB/data$I -L $LOGS/serv$I || exit 1
fi
I=`expr $I + 1`
done

echo -e "\n\033[32;1mWait for daemons:\033[0m"

I=1
while [ $I -le $SERV_C ]
do
  echo -n "$I (${SHOST[$I]}:${SPORT[$I]}) : " 
  while ! hyperdex show-config -h ${CHOST[1]} -p ${CPORT[1]} | grep "^server.*`echo \"${SHOST[$I]}\" | sed -e 's/\./\\\./g'`:${SPORT[$I]}.*AVAILABLE" > /dev/null 2>&1
  do
    if ! ps ax | grep "hyperdex-daemon.*`echo \"${SHOST[$I]}\" | sed -e 's/\./\\\./g'`.*${SPORT[$I]}" > /dev/null 2>&1; then
      echo " down";
      break;
    fi
    echo -n "."
    sleep 0.1
  done
  hyperdex show-config -h ${CHOST[1]} -p ${CPORT[1]} | grep "^server.*`echo \"${SHOST[$I]}\" | sed -e 's/\./\\\./g'`:${SPORT[$I]}"
  I=`expr $I + 1`
done

./dbcheck.sh

cd $SAVED_PATH
