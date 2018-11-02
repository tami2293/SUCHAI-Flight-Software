#!/bin/bash
SUBMODULES=/home/pi/Spel/sub_modules/
cd $SUBMODULES
direwolf -t 0 -c aprs/aprs_telem.conf  > ~/Spel/mission-history/aprs-"$(date "+%FT%T")".log 2>&1
# cd  ~/Spel
