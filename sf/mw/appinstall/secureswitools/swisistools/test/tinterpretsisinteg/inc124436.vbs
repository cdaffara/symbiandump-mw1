Dim FSys, newfullpath
Dim WshShell 
Set WshShell = WScript.CreateObject("WScript.Shell") 
Set FSys = CreateObject("Scripting.FileSystemObject")
newfullpath = FSys.BuildPath(WshShell.CurrentDirectory, "\cdrive\private\81111105\import\12Schůzka.amr") 
if FSys.FileExists(newfullpath) Then
WScript.Quit (1)
Else
WScript.Quit (0)
End If