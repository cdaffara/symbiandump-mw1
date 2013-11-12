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
my $platform = lc($ARGV[0]);

die "EPOCROOT not defined" if !defined ($ENV{EPOCROOT});
my $makesis = "$ENV{EPOCROOT}epoc32\\tools\\makesis.exe";
my $signsis = "$ENV{EPOCROOT}epoc32\\tools\\signsis.exe";
if ( ! -x $makesis || ! -x $signsis ) {
   die "$makesis and $signsis are not executable";
}

my $sign_cert = "$ENV{EPOCROOT}epoc32\\tools\\Nokia_RnDCert_02.der";
my $sign_key = "$ENV{EPOCROOT}epoc32\\tools\\Nokia_RnDCert_02.key";

if ($platform =~/winscw/i)
{
	my $winscwpkgdir = "winscw";
	my $winscwdir = "winscwsis";

	# export path for winscw sis files
	system("mkdir $winscwdir\\");
	my $target_dir = "$ENV{EPOCROOT}epoc32\\release\\winscw\\udeb\\z\\apparctest\\apparctestsisfiles\\";
	system("mkdir $target_dir\\");

	# get list of package files for winscw
	opendir DIR, $winscwpkgdir;
	my @pkgfiles = grep (/\.pkg/, readdir(DIR));
	closedir DIR;

	# create and sign each sis file for winscw
	my $target;	# needs to be seen by continue
	foreach my $entry (@pkgfiles)
		{
		print "\n";
		$entry =~ s/\.pkg//;	# remove .pkg suffix
		my $pkg_file = "$winscwpkgdir\\$entry.pkg";
		
		$target = "$target_dir\\$entry.sis";

		my $make_cmd = "$makesis $pkg_file $winscwdir\\$entry-tmp.sis";
		print "$make_cmd\n";
		system($make_cmd);

		my $sign_cmd = "$signsis $winscwdir\\$entry-tmp.sis $winscwdir\\$entry.sis $sign_cert $sign_key";
		print "\n$sign_cmd\n";
		system($sign_cmd);

		my $copy_cmd = "copy /y $winscwdir\\$entry.sis $target";
		print "\n$copy_cmd\n";
		system($copy_cmd);
		}
}

if ($platform =~ /armv5/i)
{
	my $armv5pkgdir = "armv5";
	my $armv5dir = "armv5sis";

	# export path for armv5 sis files
	system("mkdir $armv5dir\\");
	my $target_dir_armv5 = "$ENV{EPOCROOT}epoc32\\data\\Z\\System\\apparctestsisfiles\\";
	system("mkdir $target_dir_armv5\\");

	# get list of package files for armv5
	opendir DIR, $armv5pkgdir;
	my @armv5pkgfiles = grep (/\.pkg/, readdir(DIR));
	closedir DIR;

	# create and sign each sis file for armv5
	my $targetarmv5;	# needs to be seen by continue
	foreach my $entry1 (@armv5pkgfiles)
		{
		$entry1 =~ s/\.pkg//;	# remove .pkg suffix
		my $pkg_file = "$armv5pkgdir\\$entry1.pkg";
		
		$targetarmv5 = "$target_dir_armv5\\$entry1.sis";

		my $make_cmd = "$makesis $pkg_file $armv5dir\\$entry1-tmp.sis";
		print "$make_cmd\n";
		system($make_cmd);

		my $sign_cmd = "$signsis $armv5dir\\$entry1-tmp.sis $armv5dir\\$entry1.sis $sign_cert $sign_key";
		print "\n$sign_cmd\n";
		system($sign_cmd);

		my $copy_cmd = "copy /y $armv5dir\\$entry1.sis $targetarmv5";
		print "\n$copy_cmd\n";
		system($copy_cmd);
		}
}

# -----------------------------------------------------------------------

