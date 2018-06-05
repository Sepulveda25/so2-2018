#!/bin/bash
echo "-----DESCOMPRIMIENDO-----"
tar zxvf file
echo "-----INSTALO EL MODULO-----"
make
echo "-----CARGO EL MODULO-----"
insmod ./hello.ko
echo "-----IMPRIMO DMESG-----"
dmesg | tail -n 1
echo "-----VERIFICO-----"
lsmod | grep 'hello'


