#! /bin/bash

SAVED_PATH=$(pwd)
ROOT_PATH=$(cd $(dirname $0) && pwd);
cd $ROOT_PATH

./dbstart.sh && { echo -e "\n\033[32;1m\nCreate the database\n\033[0m"; ./spaceinit.py; }

cd $SAVED_PATH



