#!/usr/bin/env bash
cd a3200-sdk-lite-v1.2/
python2 waf configure build $1 --with-os freertos --enable-if-i2c --with-rtable cidr --disable-param-cmd --with-console 4
cd -
