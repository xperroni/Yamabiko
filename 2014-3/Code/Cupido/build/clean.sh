#!/bin/bash

# Remove all files and folder, save from shell script files
rm -fR $(ls | grep -v '\.sh')
