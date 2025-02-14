#!/bin/sh
TARGET=${1:?Error}

while true; do
    inotifywait -e modify ${TARGET}
    nginx -s reload
done
