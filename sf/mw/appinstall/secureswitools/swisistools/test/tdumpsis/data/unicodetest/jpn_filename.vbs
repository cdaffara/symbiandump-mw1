Dim WshShell, oExec
Dim fso 

Set fso = CreateObject("Scripting.FileSystemObject")
Set File = fso.CreateTextFile("ゎわこんァア龐龑.pkg", true)
File.WriteLine (";Languages")
File.WriteLine ("&JA")
File.WriteLine ("")
File.WriteLine (";Header")
File.WriteLine ("#{""names large sample UTF8""}, (0xA2000222), 1, 2, 3, TYPE=SA")
File.WriteLine ("")
File.WriteLine ("%{""Vendor""}")
File.WriteLine (":""Vendor""")
File.WriteLine ("")
File.WriteLine (";Mixed UTF8 characters")
File.WriteLine ("""\epoc32\winscw\c\tswi\utf8.txt""-""!:\utf8.txt""")
File.Close()

Set WshShell = CreateObject("WScript.Shell")
Set oExec = WshShell.Exec("makesis -v ゎわこんァア龐龑.pkg")
Do While oExec.Status = 0 
	WScript.Sleep 100 
Loop 

Set onExec = WshShell.Exec("dumpsis -x ゎわこんァア龐龑.sis")

Do While onExec.Status = 0 
	WScript.Sleep 100 
Loop 

if (fso.FolderExists("ゎわこんァア龐龑") And fso.FileExists("ゎわこんァア龐龑\ゎわこんァア龐龑.pkg") And fso.FileExists("ゎわこんァア龐龑\file0")) then
	Set File = fso.CreateTextFile("\epoc32\winscw\c\tswi\tdumpsis\passed.txt", true)
     	File.WriteLine ("Passed")
     	File.Close()
End If