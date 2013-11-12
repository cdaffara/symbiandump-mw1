#
# Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
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
# Description: 
#
# -----------------------------------------------------------------------

# sis files for
my $stubpkgdir = "stub_sis";

my $stubdir = "stubsis";

die "EPOCROOT not defined" if !defined ($ENV{EPOCROOT});
my $makesis = "$ENV{EPOCROOT}epoc32\\tools\\makesis.exe";
my $option = "-s";
my $signsis = "$ENV{EPOCROOT}epoc32\\tools\\signsis.exe";
if ( ! -x $makesis || ! -x $signsis ) {
   die "$makesis and $signsis are not executable";
}

my $sign_cert = "$ENV{EPOCROOT}epoc32\\tools\\Nokia_RnDCert_02.der";
my $sign_key = "$ENV{EPOCROOT}epoc32\\tools\\Nokia_RnDCert_02.key";

# export path for stub sis files
system("mkdir $stubdir\\");
my $target_winscw = "$ENV{EPOCROOT}epoc32\\release\\winscw\\udeb\\z\\system\\install\\";
my $target_armv5 = "$ENV{EPOCROOT}epoc32\\data\\z\\system\\install\\";
system("mkdir $target_winscw\\");
system("mkdir $target_armv5\\");

# get list of package files for stub
opendir DIR, $stubpkgdir;
my @pkgfiles = grep (/\.pkg/, readdir(DIR));
closedir DIR;

# create each stub sis file
my $target;	# needs to be seen by continue
my $target2;
foreach my $entry (@pkgfiles)
	{
	print "\n";
	$entry =~ s/\.pkg//;	# remove .pkg suffix
	my $pkg_file = "$stubpkgdir\\$entry.pkg";
	
	$target = "$target_winscw\\$entry.sis";
	$target2 = "$target_armv5\\$entry.sis";

	my $make_cmd = "$makesis $option $pkg_file $stubdir\\$entry.sis";
	print "$make_cmd\n";
	system($make_cmd);

	my $copy_cmd = "copy /y $stubdir\\$entry.sis $target";
	print "\n$copy_cmd\n";
	system($copy_cmd);

	my $copy_cmd = "copy /y $stubdir\\$entry.sis $target2";
	print "\n$copy_cmd\n";
	system($copy_cmd);
	}



# -----------------------------------------------------------------------

