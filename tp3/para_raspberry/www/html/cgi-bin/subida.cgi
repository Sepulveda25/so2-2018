#!/usr/bin/perl -wT

use strict;
use CGI;
use CGI::Carp qw ( fatalsToBrowser );  # Remove for production use
use File::Basename;

$CGI::POST_MAX = 1024 * 5000;  # maximum upload filesize is 5000K
my $safe_filename_characters = "a-zA-Z0-9_.-";
my $upload_dir = "/var/www/html/cgi-bin/instalarDriver";

#
# Build the form
#

my $query = new CGI;
my $filename = $query->param("file");

if ( !$filename )
{
    print $query->header ( );
    print "There was a problem uploading your file (try a smaller file).";
    exit;
}

my ( $name, $path, $extension ) = fileparse ( $filename, '..*' );
$filename = "file";
$filename =~ tr/ /_/;
$filename =~ s/[^$safe_filename_characters]//g;

           if ( $filename =~ /^([$safe_filename_characters]+)$/ )
{
    $filename = $1;
}
else
{
    die "Filename contains invalid characters";
}

my $upload_filehandle = $query->upload("file");

open ( UPLOADFILE, ">$upload_dir/$filename" ) or die "$!";
binmode UPLOADFILE;

while ( <$upload_filehandle> )
{
    print UPLOADFILE;
}

close UPLOADFILE;

print $query->header ( );
print <<END_HTML;
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
    <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <style type="text/css">
            img {border: none;}
        </style>
    </head>
    <body>
        INSTALACION DRIVER<br />
        <iframe
             src ="/cgi-bin/instalarDriver/call_install.cgi"
             width="98%"
             height="258">
        </iframe>
    </body>
</html>
END_HTML
