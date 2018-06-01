#!/bin/bash
cd /var/www/html/cgi-bin
gcc recursos_sistema.c -o recursos_sistema.cgi
gcc baash.c -o baash.cgi
