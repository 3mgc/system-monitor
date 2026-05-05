#!/bin/bash
g++ -o monitorJsn monitorJson.cpp

if [ ! -d "venv" ]; then
  echo "Creando entorno virtual e instalando flask..."
  python3 -m venv venv
  source venv/bin/activate
  pip install flask
else
  source venv/bin/activate
fi
./monitorJsn & 
MONITOR_PID=$!

python sha.py
kill $MONITOR_PID


