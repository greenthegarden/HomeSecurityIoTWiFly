#!/usr/bin/env bash

PORT="/dev/ttyACM1"
ENV="uno"

pio run --upload-port ${PORT} -e ${ENV} -t upload 
