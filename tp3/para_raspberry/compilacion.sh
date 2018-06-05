#!/bin/bash
cd /var/www/html/cgi-bin
sudo gcc recursos_sistema.c -o recursos_sistema.cgi
sudo gcc baash.c -o baash.cgi
sudo gcc listar_modulos.c -o listar_modulos.cgi

