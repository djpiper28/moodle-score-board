#!/bin/bash
find src test | grep "\\.[ch]$" | xargs astyle --formatted --style=kr
