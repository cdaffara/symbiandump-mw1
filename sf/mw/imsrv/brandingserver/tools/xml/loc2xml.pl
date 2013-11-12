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
# Description:   Converts loc file into branding xml file
#

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
# Prints usage information
#------------------------------------------------------------------------------
#
sub UsageInformation
{
    print <<EOF;
LOC2XML.PL
converts loc file into branding xml file. XML file is overwritten!

Usage:
loc2xml.pl [in file:loc] [out file:xml] [brand_id] [application_id]
           [language id] [version]
EOF

    exit;
}

#------------------------------------------------------------------------------
# Reads LOC file and constructs associative array
#------------------------------------------------------------------------------
#
sub ReadLocFile
{
# Parse LOC file
my $infile = shift;
open(INPUT, $infile) || die "Can't open file: '$infile'";   # Open the file
my %array = ();

while( <INPUT> )
    {
    $_ = trim( $_ );
    if( /^\#define\s+qtn.+\s+\".+\"/i )
        {
        # find ID
        /(qtn[\w\d_]+)/i;
        $id = $1;
        $id =~ s/_//g;      # remove all '_' chars

        #find value
        /\"(.+)\"/i;
        $value = $1;

        $array{$id} = $value;
#        print "$id \t: \'$value\'\n\r"
        }
    }
close(INPUT);			    # Close the file

return %array;
}

#------------------------------------------------------------------------------
# Write XML file from given array
#------------------------------------------------------------------------------
#
sub WriteXmlFile
{
my %strings = @_;
open( OUTPUT, ">$ARGV[1]" ) || die "Can't open file: '$ARGV[1]'";


print OUTPUT <<EOF;
<?xml version="1.0" encoding="ISO-8859-1"?>
<branding>
    <brand>
        <application_id>$ARGV[3]</application_id>
        <brand_id>$ARGV[2]</brand_id>
        <brand_language_id>$ARGV[4]</brand_language_id>
        <brand_version>$ARGV[5]</brand_version>

EOF

while( ($key, $value) = each(%strings) )
    {
    print OUTPUT <<EOF;
        <element id="$key" type="text">
         <element_value>$value</element_value>
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
if( $count != 6 )
    {
    UsageInformation();
    }

# Parse LOC file
my %strings = &ReadLocFile( $ARGV[0] );

# Write XML file
WriteXmlFile( %strings );

# Done!
my @list = %strings;
$count = @list / 2;
print "Processed succesfully $count strings."

# end of file
