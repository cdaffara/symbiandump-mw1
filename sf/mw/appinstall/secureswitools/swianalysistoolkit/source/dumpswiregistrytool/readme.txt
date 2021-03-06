DUMPSWIREGISTRYTOOL
====================

DumpSWIRegistryTool interprets SWI Registry and displays it's contents.


USAGE
------

The command line syntax is:

DumpSWIRegistrytool  [-v] [-h] [RegistryPath] [-p package_uid] [-r package_uid package_name vendor_name] 

where:

package_uid 	- UID of the package

package_name	- name of the package obtained when -p option is used.Required for registry entry to view details 
		  of a particular registry entry.

vendor_name	- name of vendor obtained when -p option is used.Required for registry entry to view details 
		  of a particular registry entry.

RegistryPath	- swiregistry path - OPTIONAL if Environment PATH is set .

-h | --help

	Print this (help) message

-v | --version

	version of DumpSWICertstore tool

-p | -- package uid

	Displays the top level information about a package (i.e all the registry entries),when it's package uid is specified.

-r | --registry entry

	Displays controller information about a particular package ,
	when it's registry entry(consiting of package uid,package name and vendor name) are specified.
	The registry entry can be chosen from the information displayed by -p option.

NOTE: Set the environment variable DUMPSWIREGISTRY to the sisregistry path - DEFAULT: 
	"DUMPSWIREGISTRY=\epoc32\<platform>\c\sys\install\sisregistry\

     All inputs must be specified in the same order as mentioned above.



Example Usage:
--------------

CASE 1:When registry path is not specified,it takes the default path as set in the environment path.

INPUT: dumpswiregistrytool -p 81231235  OR dumpswiregistrytool \security\swi\test\tsisregistrytest\data -p 81231235
-----

where \security\swi\test\tsisregistrytest\data is the registry path which will overwrite the default path.

OUTPUT:
------

Package Information:
-------------------

Registry Entry :\epoc32\winscw\c\sys\install\sisregistry\81231235\00000000.reg
Package UID :81231235
Package Name :TestAppInUse Suite
Vendor :Unique Vendor Name
Package Index :0
Drives :4
No of executables :3
Package Version :1.00
Languages :English
Selected Drive :C
File Major Version :4
File Minor Version :0
Package Vendor Localized Name :Vendor
Package Install Type :Installation
Is Package in ROM :0
Is Package Preinstalled :0
Is Package Signed :1
Package Trust :SisPackageCertificateChainNoTrustAnchor

Press any key to continue

INPUT: dumpswiregistrytool -r 81231235 "TestAppInUse Suite" "Unique Vendor Name"
-----
			OR dumpswiregistrytool \security\swi\test\tsisregistrytest\data 81231235 -r "TestAppInUse Suite" 													"Unique Vendor Name"

where \security\swi\test\tsisregistrytest\data is the registry path which will overwrite the default path.


OUTPUT:
------

Controller Information:
----------------------

Registry Entry :\epoc32\winscw\c\sys\install\sisregistry\81231235\00000000.reg
Package UID :81231235
Package Name :TestAppInUse Suite
Vendor :Unique Vendor Name
Package Index :0

Controller1 Version :1.00
Controller1 Algorithm :SHA1

Number of Dependencies : 0
Number of Embedded Packages : 0
SISProperty Count:0
Number Of File Descriptions :6

SIS File Description:

Target1:c:\resource\apps\HelloWorld.rsc
File Operation Type :Install
Algorithm :SHA1
SID :0

Target2:c:\resource\apps\HelloWorld.mbm
File Operation Type :Install
Algorithm :SHA1
SID :0

Target3:c:\private\10003a3f\import\apps\HelloWorld_reg.RSC
File Operation Type :Install
Algorithm :SHA1
SID :0

Target4:c:\sys\bin\HelloWorld.exe
File Operation Type :Install
Algorithm :SHA1
SID :90008ace

Target5:c:\sys\bin\console_app.exe
File Operation Type :Install
Algorithm :SHA1
SID :89334567

Target6:c:\sys\bin\runtestexe.exe
File Operation Type :Install
Algorithm :SHA1
SID :89334568

Validation Status : Validated
Revocation Status : OcspNotPerformed


Press any key to continue