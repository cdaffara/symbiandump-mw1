Dim WshShell, oExec
Dim fso
 

Set WshShell = CreateObject("WScript.Shell")
Set oExec = WshShell.Exec("SignSIS  -s toinstallunicodefilename.SIS  toinstallunicodefilename.SIS  default.cer default.key pass")
Do While oExec.Status = 0
     WScript.Sleep 100
Loop

Set oExec = WshShell.Exec("SignSIS  -s 丗丰丘丵.SIS  丗丰丘丵.SIS  default.cer default.key pass")
Do While oExec.Status = 0
     WScript.Sleep 100
Loop



Set fso = CreateObject("Scripting.FileSystemObject")
fso.DeleteFile "bothdataandsisunicode.SIS", true
fso.DeleteFile "丗丰丘丵.txt", true
