#!/usr/bin/perl

print "Content-type: text/html\n\n";
my $archivo = "path.txt"; # al ingrsar se borra el contenido de path.txt
open(FILE, "> $archivo"); #abrir el archivo para escritura
flock(FILE, 2) if $usar_flock; #usar flock() si disponible
print FILE "$\ "; #escribir nueva entrada
close(FILE); #cerrar archivogi);
print <<htmlcode;
<html>
<head>
<title>Baash</title>
<a href="/">Pagina principal</a>
</head>
<body>
	<h1><center>Aplicacion Baash</center> </h1>
	<hr>
	<form action="/cgi-bin/baash.cgi" target="my_iframe">
		Comando: <input type="text/plain" name="comando"><br>
		<input type="submit" value="Enter">
	</form>
	<hr>
	<div align="center">
		<iframe
			name="my_iframe"
			width="80%"
			height="420">	
		</iframe>
	</div>

</body>
htmlcode
