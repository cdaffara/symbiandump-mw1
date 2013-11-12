# Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
# Description:
# ResourceGenerator
# Generates resources for the TzLocalization component
# 
#

$numResources = $ARGV[0];

$numResources > 0 or die("Please specify how many resources to create.");

srand();

for($i=0; $i<$numResources; $i++)
	{
	print "RESOURCE LOCALIZED_TIMEZONE r_tz_".($i+1)."\n";
	print "\t{\n";
	print "\ttime_zone_id = ".8*($i+1).";\n";
	print "\tstandard_name = \"".randomChar()."Greenwich Mean Time".($i+1)."\";\n";
	print "\tdaylight_name = \"".randomChar()."British Summer Time".($i+1)."\";\n";
	print "\tshort_standard_name = \"".randomChar()."GMT".($i+1)."\";\n";
	print "\tshort_daylight_name = \"".randomChar()."BST".($i+1)."\";\n";
	print "\tcities = \n";
	print "\t\t{\n";
	print "\t\tTIMEZONE_CITY\n";
	print "\t\t\t{\n";
	print "\t\t\tcity_name = \"".randomChar()."London".($i+1)."\";\n";
	print "\t\t\tcity_group_id = 1;\n";
	print "\t\t\t},\n";
	print "\t\tTIMEZONE_CITY\n";
	print "\t\t\t{\n";
	print "\t\t\tcity_name = \"".randomChar()."Manchester".($i+1)."\";\n";
	print "\t\t\tcity_group_id = 1;\n";
	print "\t\t\t},\n";
	print "\t\tTIMEZONE_CITY\n";
	print "\t\t\t{\n";
	print "\t\t\tcity_name = \"".randomChar()."Birmingham".($i+1)."\";\n";
	print "\t\t\t}\n";
	print "\t\t};\n";
	print "\t}\n";
	}

sub randomChar
	{
	# find out ascii number for "A"
	my $numA = ord("A");
	# find out ascii number for "A"
	my $numZ = ord("Z");
	
	# generate a random letter and return it
	my $randomNumber = rand($numZ - $numA);
	my $randomChar = chr($numA + int($randomNumber));
	return $randomChar;
	}