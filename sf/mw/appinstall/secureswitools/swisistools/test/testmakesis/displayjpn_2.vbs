Dim fso

Set fso = CreateObject("Scripting.FileSystemObject")
if (fso.FileExists("ゎわこんァア龐龑.SIS")) then 
      Set File = fso.CreateTextFile("\epoc32\winscw\c\tswi\passed.txt", true)
     	File.WriteLine ("Passed")
     	File.Close()
End If

