#!/usr/bin/perl
# Recurcusos del sistema
print "Content-type: text/html\n\n";
print <<htmlcode;
<html>
<head>
<title>Recursos </title>
<a href="/">Pagina principal</a>
</head>
<body>
	<h1><center>Recursos del sistema </center> </h1>
	<hr>
	<div align="center">
		<iframe
			src ="/cgi-bin/recursos_sistema.cgi"
			width="80%"
			height="420">	
		</iframe>
	</div>
</body>
htmlcode
