Dim WshShell, oExec
Dim fso_result, fso_pkg 

Set fso = CreateObject("Scripting.FileSystemObject")
'Cleanup routine for all files created

if (fso.FileExists("ゎわこんァア龐龑.pkg")) then
	fso.DeleteFile "ゎわこんァア龐龑.pkg ", true
End If


if (fso.FileExists("ゎわこんァア龐龑.sis")) then
	fso.DeleteFile "ゎわこんァア龐龑.sis ", true
End If

if (fso.FileExists("\epoc32\winscw\c\tswi\tdumpsis\passed.txt")) then
	fso.DeleteFile "\epoc32\winscw\c\tswi\tdumpsis\passed.txt", true
End If

if (fso.FolderExists("\epoc32\winscw\c\tswi\tdumpsis\ゎわこんァア龐龑")) then
	fso.DeleteFolder "\epoc32\winscw\c\tswi\tdumpsis\ゎわこんァア龐龑", true
End If

