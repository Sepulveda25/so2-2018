#!/usr/bin/perl

print "Content-type: text/html\n\n";
print <<htmlcode;
<html>
<head>
<title>Modulos</title>
<a href="/">Pagina principal</a>
</head>
<body>
	<h1><center>Modulos de kenel</center></h1>
	<hr>
	<div style="float:left; width:50%;">
		MODULOS<br />
		<iframe
			src ="/cgi-bin/listar_modulos.cgi"
			width="98%"
			height="460">	
		</iframe>
	</div>
	<div style="float:left; width:50%;">
		DRIVER<br />
		<form action="/cgi-bin/subida.cgi" method="post" enctype="multipart/form-data" target="my_iframe">
			<p>Seleccionar archivo de driver (.tar.gz): <input type="file" name="file"  /></p>
			<p><input type="submit" name="Submit" value="Instalar" /></p>
		</form>
		<form action="/cgi-bin/instalarDriver/call_uninstall.cgi" method="post" target="my_iframe">
			<p><input type="submit" name="Submit" value="Desinstalar" /></p>
		</form>

		<hr>
		<iframe name="my_iframe" width="98%" height="301"></iframe>

	</div>

</body>
htmlcode
