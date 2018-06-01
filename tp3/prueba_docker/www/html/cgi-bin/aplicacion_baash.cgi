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
	<div align="center">
		<iframe
			src ="/cgi-bin/baash.cgi"
			width="80%"
			height="420">	
		</iframe>
	</div>

</body>
htmlcode
