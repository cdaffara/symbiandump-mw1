Dim fso, File

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Prepares unicode data files  '
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

Set fso = CreateObject("Scripting.FileSystemObject")
Set File = fso.CreateTextFile("丗丰丘丵.txt",true)

File.WriteLine("NQP-BR, Successfully created one text file with Unicode name") 
File.Close() 

