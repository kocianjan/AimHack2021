#/bin/bash

raspivid --flush --exposure night -n --listen --verbose -t 0 -o tcp://0.0.0.0:3333 -w 640 -h 480
