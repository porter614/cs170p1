#!/bin/bash


echo "Shell Test"
mkdir shell_test_tmp
ls -sa shell_test_tmp
echo "Begin shell test_tmp"
echo "content" > shell_test_tmp/some_file
cat < shell_test_tmp/some_file | wc
echo "End shell test_tmp"
lm
ls -sa shell_test_tmp
rm -rf shell_test_tmp