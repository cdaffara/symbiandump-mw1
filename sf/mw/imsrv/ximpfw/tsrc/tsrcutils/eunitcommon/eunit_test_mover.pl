#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description: XIMP Framework Test Code 
#
#!perl
#
use strict;
use File::Path;

$|=1; # ensures that any progress information sent to the screen is displayed immediately and not buffered

# When this script is used with build tools, move is done only for WINSCW platform 
if ( exists($ENV{'PLATFORM'})  && 
     ($ENV{'PLATFORM'} ne 'WINSCW' ) )
	{
	print "\nEUnit Test DLL Mover: nothing to do for platform.\n";
	exit 0;
	}


# destination folder where EUnit test dlls are placed
my $destFolder = "\\epoc32\\release\\winscw\\udeb\\z\\sys\\bin";

# ABLD command 
# If this script is used with build chain, then "TO_BLDINF" env. variable points to the ABLD folder 
# else use ABLD from current folder
my $abldCommand = ""; 
if (exists($ENV{'TO_BLDINF'}))
	{
	$abldCommand = $ENV{'TO_BLDINF'} . "\\abld build winscw udeb -what";	
	}
else
	{
	$abldCommand = "abld build winscw udeb -what";
	}

# get option
my $option = $ARGV[0];

# get all the files produced by the ABLD
my @allProducedFiles = `$abldCommand`;

# we're interested only from dlls
my @dllList = grep{ $_=~m#^.+\.dll$#i }@allProducedFiles;

print "\n";
foreach my $dll (@dllList)
	{
	chomp($dll);

    # skip those which are not mt_, ut_, t_, dec_
    my @paths = split(/\\/, $dll);
    next unless (grep { $paths[-1] =~ m#^$_.+$#i; } (qw(mt_ ut_ t_ dec_)));

	if( $option eq "-wipe" )
		{
		my($dllPath, $dllName) = $dll =~ m/(.*\\)(.*)$/;
		$dll = $destFolder . "\\" . $dllName;
		print "EUnit Test DLL Mover: CLEANING $dll\n";
		rmtree $dll;
		}
	else
		{
		print "EUnit Test DLL Mover: COPYING $dll to $destFolder\n";
		system "copy $dll ${destFolder}";
		}
	}
	

