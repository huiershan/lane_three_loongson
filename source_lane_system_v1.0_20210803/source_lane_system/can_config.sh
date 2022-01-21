#!/bin/sh
canconfig can0 stop
canconfig can0 bitrate 100000 
canconfig can0 ctrlmode loopback off  listen-only off tripple-sampling on one-shot off  berr-reporting off
canconfig can0 start

canconfig can1 stop
canconfig can1 bitrate 100000 
canconfig can1 ctrlmode loopback off  listen-only off tripple-sampling off one-shot off  berr-reporting off
canconfig can1 start
