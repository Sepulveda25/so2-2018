FROM ubuntu:latest

RUN apt-get update
#####INSTALACION Y CONFIGURACION DE APACHE2##############

RUN apt-get install -y apache2

#########Instalacion de perl y bibliotecas#####

RUN apt-get install perl -y

RUN apt-get install libapache2-mod-perl2 build-essential -y

RUN apt-get install libcgi-session-perl -y

########Instalacion de supervisor####

RUN apt-get install -qy --no-install-recommends supervisor

##########Configuracion de pagina ejemplo

RUN mkdir -p /var/www/html/cgi-bin

RUN chown -R $USER:$USER /var/www/html/cgi-bin

RUN chmod -R 755 /var/www/

COPY apache_inicio.sh /script/apache_inicio.sh

ENTRYPOINT ["/usr/bin/supervisord"]







