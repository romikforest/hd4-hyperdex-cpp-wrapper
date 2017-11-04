mkdir -p db
mkdir -p db/coord1
mkdir -p db/data1
mkdir -p logs
mkdir -p logs/coord1
mkdir -p logs/serv1


hyperdex coordinator -d -l 127.0.0.1 -p 1982 -D db/coord1 -L logs/coord1
sleep 3
hyperdex wait-until-stable

hyperdex daemon -d --listen=127.0.0.1 --listen-port=2012 --coordinator=127.0.0.1 --coordinator-port=1982 --data=db/data1 -L logs/serv1
sleep 3
hyperdex wait-until-stable

./dbcheck.sh

./spaceinit.py

