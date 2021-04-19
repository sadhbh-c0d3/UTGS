#!/bin/bash
find ../Useless -iname "*.o" | xargs rm
rm -f build.log
rm -f failed.log
