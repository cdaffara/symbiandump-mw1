Dim strCN, strOU, strO, strC, strReadLine

Set StdOut = WScript.StdOut
Set objArgs = WScript.Arguments

strCN = "CN=乣乨乩chris"
strOU = "OU=丹主丼devel"
strO  = "O=龝龞龠acme"
strC  = "C=JN"

Dim WshShell, oExec
Dim fso, File, ExePath, INIPath

ExePath = objArgs(0) + "\GetDNFromCert.exe  丁丂七丄kanji_cer.cer dn.ini"
INIPath = objArgs(0) + "\dn.ini"

Set WshShell = CreateObject("WScript.Shell")
Set oExec = WshShell.Exec(ExePath)

Do While oExec.Status = 0
     WScript.Sleep 100
Loop

Set fso = CreateObject("Scripting.FileSystemObject")
Set File = fso.OpenTextFile(INIPath, 1, 0, -1)

Do
strReadLine = File.Readline()
Loop Until strReadLine = "[Section1]"

strReadLine = File.Readline()
if strReadLine <> strCN Then
     WScript.Quit (1)
End If

strReadLine = File.Readline()
if strReadLine <> strOU Then
     WScript.Quit (1)
End If

strReadLine = File.Readline()
if strReadLine <> strO Then
     WScript.Quit (1)
End If

strReadLine = File.Readline()
if strReadLine <> strC Then
     WScript.Quit (1)
End If

Set LogFile = fso.CreateTextFile("ResultMakeKeys.txt")
LogFile.WriteLine("Passed")
LogFile.Close()
File.Close()

fso.DeleteFile INIPath, true
fso.DeleteFile "丟両丣kanji_key.key", true
fso.DeleteFile "丁丂七丄kanji_cer.cer", true