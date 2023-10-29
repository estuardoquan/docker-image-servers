#!/bin/sh
while true; do
    inotifywait -e modify ${STEPPATH}/site.crt
    nginx -s reload
done
