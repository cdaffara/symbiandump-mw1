Dim WshShell, oExec
Dim fso, File

Set WshShell = CreateObject("WScript.Shell")
Set oExec = WshShell.Exec("makekeys -cert  -password  乕乖乗乘pass  -len  1024  -dname  ""CN=乣乨乩chris  OU=丹主丼devel  OR=龝龞龠acme CO=JN  EM=test@nowhere.com""  丟両丣kanji_key.key 丁丂七丄kanji_cer.cer")

Do While oExec.Status = 0
     WScript.Sleep 100
Loop

Set fso = CreateObject("Scripting.FileSystemObject")

dim i
i = 0

if (fso.FileExists("丟両丣kanji_key.key")) then 
       i = i + 1
end if
if (fso.FileExists("丁丂七丄kanji_cer.cer")) then 
       i = i + 1
end if

if ( i = 2 ) then
	Set File = fso.CreateTextFile("ex.txt", true)
	File.WriteLine ("Exist")
	File.Close()
End If



