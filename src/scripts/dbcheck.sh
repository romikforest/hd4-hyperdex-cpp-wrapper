#! /bin/bash

SAVED_PATH=$(pwd)
ROOT_PATH=$(cd $(dirname $0) && pwd);
cd $ROOT_PATH

. cfg.sh

C_CNT=`pidof replicant-daemon | wc -w`
S_CNT=`pidof hyperdex-daemon | wc -w`

if [ $C_CNT -eq 0 -a $S_CNT -eq 0 ] ; then

  echo -e "\n\033[33;1mThe HyperDex claster is not started\n\033[0m"

else

  echo -e "\n\033[33;1mHyperDex claster is started: $C_CNT coordinators and $S_CNT daemons\n\033[0m"
  
  connstr=""

  I=1
  while [ $I -le $COORD_C ]
  do
    if ps ax | grep "replicant-daemon.*`echo \"${CHOST[$I]}\" | sed -e 's/\./\\\./g'`.*${CPORT[$I]}" > /dev/null 2>&1; then
      connstr=`replicant conn-str -h ${CHOST[1]} -p ${CPORT[1]}`
      echo -e "\nconn-str is:\n$connstr\n"
      break;
    fi
    I=`expr $I + 1`
  done
  
  if [ "$connstr" = "" ] ; then
    echo -e "\n\033[31;1mCoordinators don't respond\n\033[0m"
    exit
  fi
  
  echo -e "The coordinators are:"
  echo $connstr | sed -e 's/,/\n/g' | sed -e 's/^\(.\+\)$/& &/'\
  | sed -e 's/ /"; replicant poke -h /' | sed -e 's/:/ -p /g' | sed -e 's/ -p /:/'\
  | sed -e 's/$/ \&\& echo " - alive"/' | sed -e 's/^/echo -n "/'\
  | bash - 2>/dev/null

  echo -e "\nThe servers are:\n"

  I=1
  while [ $I -le $COORD_C ]
  do
    if ps ax | grep "replicant-daemon.*`echo \"${CHOST[$I]}\" | sed -e 's/\./\\\./g'`.*${CPORT[$I]}" > /dev/null 2>&1; then
      hyperdex show-config -h ${CHOST[$I]} -p ${CPORT[$I]} | grep ^server
      break;
    fi
    I=`expr $I + 1`
  done

fi

cd $SAVED_PATH
