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
# Description:   Writes a header file based on brand xml file
#

#------------------------------------------------------------------------------
# Prints usage information
#------------------------------------------------------------------------------
#
sub UsageInformation
{
    print <<EOF;
XML2h.PL
Finds all element id's in brand XML file and writes a C++ header file based on
them. Destination file is overwritten!

Usage:
xml2h.pl [in file:xml] [out file:h]
EOF

    exit;
}

#------------------------------------------------------------------------------
# Reads XML file and finds element id's
#------------------------------------------------------------------------------
#
sub ReadXMLFile
{
    # Parse XML file
    open( INPUT, $ARGV[0] ) || die "ERROR: Can't open file: '$ARGV[0]'";

    # join lines
    my $xmlcontent = join( "", <INPUT> );
    close(INPUT);

    # remove all comments
    $xmlcontent =~ s/<!--[.\s]*-->//g;

    # find elements id's
    $_ = $xmlcontent;
    my @elements = /<.*element[. ]* id="([\w\d_]+)".*>/g;

    #debug print
#   foreach $item (@elements) print "$item\n";
    return @elements;
}

#------------------------------------------------------------------------------
# Write Header file
#------------------------------------------------------------------------------
#
sub WriteHeaderFile
{
    my @ids = @_;
    open( OUTPUT, ">$ARGV[1]" ) || die "Can't open file: '$ARGV[1]'";

    # Write header
    print OUTPUT <<EOF;
// Branding item header file created with XML2H. DO NOT MODIFY!

EOF

    # Write items
    foreach $item (@ids)
        {
        $const = $item;
        substr( $const, 0, 1 ) =~ tr/[a-z]/[A-Z]/;
        print OUTPUT <<EOF;
_LIT8( K$const, \t"$item");
EOF
        }

    print OUTPUT "\n//End of file\n";
    close( OUTPUT );
}

#------------------------------------------------------------------------------
# Main function
#------------------------------------------------------------------------------
#

# Check parameters
my $count = @ARGV;
if( $count != 2 )
    {
    UsageInformation();
    }

# Parse XML file
my @ids = ReadXMLFile();

# Write XML file
WriteHeaderFile( @ids );

# Done!
$count = @ids;
print "Processed succesfully $count elements."

# end of file
