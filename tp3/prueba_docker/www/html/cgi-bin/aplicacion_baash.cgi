#!/usr/bin/perl

print "Content-type: text/html\n\n";
print <<htmlcode;
<html>
<head>
<title>Baash</title>
<a href="/">Index</a>
</head>
<body>
	<h1><center>Aplicacion Baash</center> </h1>
	<hr>
	<form action="/cgi-bin/baash.cgi" target="my_iframe">
		Comando: <input type="text" name="comando"><br>
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
