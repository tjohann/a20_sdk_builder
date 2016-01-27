#!/bin/bash

find . -name "*~" -exec rm {} \;

git grep -I --name-only -z -e '' | xargs -0 sed -i 's/[ \t]\+\(\r\?\)$/\1/'
