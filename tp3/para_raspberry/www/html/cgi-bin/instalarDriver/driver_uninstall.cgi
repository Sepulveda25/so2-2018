#!/bin/bash
echo "-----DESCARGO EL MODULO-----"
rmmod hello
echo "-----IMPRIMO DMESG-----"
dmesg | tail -n 1
