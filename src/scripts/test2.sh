#! /bin/bash
. cfg.sh

echo -e "\n\033[31;1mCoordinators don't respond\n\033[0m"

I=1
while [ $I -le $SERV_C ]
do
  echo -n "$I (${SHOST[$I]}:${SPORT[$I]}) : " 
    if ! ps ax | grep "hyperdex-daemon.*`echo \"${SHOST[$I]}\" | sed -e 's/\./\\\./g'`.*${SPORT[$I]}" > /dev/null 2>&1; then
      echo " down";
    fi
  I=`expr $I + 1`
done

#I=1
#while [ $I -le $SERV_C ]
#do
#  echo -n "$I: "
#  while hyperdex show-config | grep "^server.*`echo \"${SHOST[$I]}\" | sed -e 's/\./\\\./g'`:${SPORT[$I]}.*ASSIGNED"
#    do :
#    done
#  hyperdex show-config | grep "^server.*`echo \"${SHOST[$I]}\" | sed -e 's/\./\\\./g'`:${SPORT[$I]}"
#  I=`expr $I + 1`
#done
