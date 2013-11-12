#
# Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of the License "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:   Converts mbg file into branding xml file
#

# SETUP
#
my $skinidconstants   = 'epoc32\include\middleware\aknsconstants.h';
my $skiniddefinitions = 'epoc32\include\middleware\aknsconstants.hrh';
my $version =           'v0.1';

# GLOBALS
#

# these hashes store the data for each bitmap.
# each data can be accessed with same key value.
my %bitmaps = ();       # bitmapname => bitmap id
my %masks = ();         # bitmapname => mask id
my %skinidmajor = ();   # bitmapname => major skin id
my %skinidminor = ();   # bitmapname => minor skin id

my $commonpart;         # common part of every mbg

#------------------------------------------------------------------------------
# Trim string
#------------------------------------------------------------------------------
#
sub trim($)
{
	my $string = shift;
	$string =~ s/^[ ]+//;
	$string =~ s/[ ]+$//;
	return $string;
}

#------------------------------------------------------------------------------
# Determines, validates, and returns EPOCROOT.
#------------------------------------------------------------------------------
#
sub getEpocroot
{
    my $epocroot = $ENV{EPOCROOT};
    die "ERROR: Must set the EPOCROOT environment variable.\n"
	if (!defined($epocroot));
    $epocroot =~ s-/-\\-go;	# for those working with UNIX shells
    die "ERROR: EPOCROOT must be an absolute path, " .
	"not containing a drive letter.\n" if ($epocroot !~ /^\\/);
    die "ERROR: EPOCROOT must not be a UNC path.\n" if ($epocroot =~ /^\\\\/);
    die "ERROR: EPOCROOT must end with a backslash.\n" if ($epocroot !~ /\\$/);
    die "ERROR: EPOCROOT must specify an existing directory.\n"
	if (!-d $epocroot);
    return $epocroot;
}

#------------------------------------------------------------------------------
# Prints usage information
#------------------------------------------------------------------------------
#
sub UsageInformation
{
    print <<EOF;
MBG2XML.PL $version
------------------------
converts mbg file into branding xml file. Seeks skin ids for all the
found bitmaps. Script needs to be executed on the same drive as the
development platform.

NOTE! XML file is overwritten!

Usage:
mbg2xml.pl [in file:mbg] [out file:xml] [brand_id] [application_id]
           [language id] [version] [bitmap file extension (MBM, MIF..)]

EOF

    exit;
}

#------------------------------------------------------------------------------
# Parse Avkon skin id file and find ids
#------------------------------------------------------------------------------
#
sub ParseAvkonSkinFile
{
# read file
print "\nParsing file: ".&getEpocroot.$skinidconstants;
open( F, &getEpocroot.$skinidconstants ) || die "Can't open avkon skin file!";
my @readbuffer = <F>;
close( F );
my $longbuf = join( "", @readbuffer );

# try to find each bitmap from file
foreach $item( keys( %bitmaps ) )
    {
    $searchitem = $item;
    $searchitem =~ s/$commonpart//;
    $searchitem =~ s/_//g;
    $_ = $longbuf;
    #print "\n - Seaching: ".$searchitem." = \t";
    if( /static\s+const\s+TAknsItemID\s+KAknsIID($searchitem)\s*=\s*{\s*(\w+)\s*,\s*(\w+)\s*}\s*;/i )
        {
        #print $2." - ".$3;
        $skinidmajor{$item} = $2;
        $skinidminor{$item} = $3;
        }
    else
        {
        #print "not found";
        }
    }
}

#------------------------------------------------------------------------------
# Parse Avkon skin id definition file and find values for ids
#------------------------------------------------------------------------------
#
sub ParseAvkonSkinDefFile
{
# read file
print "\nParsing file: ".&getEpocroot.$skiniddefinitions;
open( F, &getEpocroot.$skiniddefinitions ) || die "Can't open avkon skin def file!";
my @readbuffer = <F>;
close( F );

my $longbuf = join( "", @readbuffer );

# Parse major ids
print "\nParsing major skin ids";
while( ( $key, $id ) = each( %skinidmajor ) )
    {
    $searchitem = $id;
    $_ = $longbuf;
    #print "\n - Seaching: ".$searchitem." = \t";
    if( /\s+$searchitem\s*=\s*([a-fx0-9]+)/i )
        {
        #print $1;
        $skinidmajor{$key} = hex($1);
        }
    else
        {
        #print "not found";
        }
    }

# Parse minor ids
print "\nParsing minor skin ids";
while( ( $key, $id ) = each( %skinidminor ) )
    {
    $searchitem = $id;
    $_ = $longbuf;
    #print "\n - Seaching: ".$searchitem." = \t";
    if( /\s+$searchitem\s*=\s*([a-fx0-9]+)/i )
        {
        #print $1;
        $skinidminor{$key} = hex($1);
        }
    else
        {
        #print "not found";
        }
    }

}

#------------------------------------------------------------------------------
# Reads MBG file and constructs associative array
#------------------------------------------------------------------------------
#
sub ReadMbgFile
{
# Parse LOC file
my $infile = shift;
open(INPUT, $infile) || die "Can't open file: '$infile'";   # Open the file


while( <INPUT> )
    {
    $_ = trim( $_ );
    if( /^\s*(E[\w_]*) [=] ([0-9]+),/i )
        {
        $_ = $1;
        $value = $2;
        if( /(E[\w_]*)_mask/ )
            {
            $masks{$1} = $value;
            }
        else
            {
            $bitmaps{$1} = $value;
            }
        #$id =~ s/_//g;      # remove all '_' chars
        }
    elsif( /^\s*(E[\w]*)LastElement\s*/ )
        {
        print "\nCommon part for bitmaps: '$1'\n";
        $commonpart = $1;
        }
    }
close(INPUT);			    # Close the file
}

#------------------------------------------------------------------------------
# Write branding XML file
#------------------------------------------------------------------------------
#
sub WriteXmlFile
{
my %strings = @_;
$len = length $ARGV[0];
$bitmap_file_id = substr($ARGV[0], 0,  $len - 4);
$bitmap_file = $bitmap_file_id . "." . $ARGV[6];
$bitmap_file_id = $bitmap_file_id . "_" . $ARGV[6];

open( OUTPUT, ">$ARGV[1]" ) || die "Can't open file: '$ARGV[1]'";

print OUTPUT <<EOF;
<?xml version="1.0" encoding="ISO-8859-1"?>
<branding>
    <brand>
        <application_id>    $ARGV[3]</application_id>
        <brand_id>          $ARGV[2]</brand_id>
        <brand_language_id> $ARGV[4]</brand_language_id>
        <brand_version>     $ARGV[5]</brand_version>
        <element type="file" id=\"$bitmap_file_id\">
        	<element_value>
                <file_name>c:\\private\\102828DD\\import\\$bitmap_file</file_name>
        	</element_value>
        </element>

EOF


while( ($key, $value) = each(%bitmaps) )
    {
    # strip underscores
    $modifkey = $key;
    $modifkey =~ s/_//g;

    print OUTPUT <<EOF;
        <element id="$modifkey" type="bitmap">
        	<element_value>
        		<bitmap_file_id> $bitmap_file_id</bitmap_file_id>
        		<bitmap_id>      $value </bitmap_id>
        		<mask_id>        $masks{$key} </mask_id>
        		<skin_id_major> $skinidmajor{$key} </skin_id_major>
        		<skin_id_minor>   $skinidminor{$key} </skin_id_minor>
         	</element_value>
        </element>

EOF
    }

print OUTPUT "    </brand>\n";
print OUTPUT "</branding>\n";

close( OUTPUT );
}

#------------------------------------------------------------------------------
# Main function
#------------------------------------------------------------------------------
#

# Check parameters
my $count = @ARGV;
if( $count != 7 )
    {
    UsageInformation();
    }

# Parse MBG file for bitmaps
print "\nParsing input file: $ARGV[0]";
ReadMbgFile( $ARGV[0] );

# Parse avkon skin files and find skin id's for all the found bitmaps
ParseAvkonSkinFile();
ParseAvkonSkinDefFile();

# Write XML file
WriteXmlFile();

# Done!
my @list = %bitmaps;
$count = @list / 2;
print "\n\nProcessed succesfully $count bitmaps.\nDone!\n"

# end of file
