var fso = WScript.CreateObject('Scripting.FileSystemObject')
var inpname = "win.rc", tmpname = inpname + '.tmp'
var inp = fso.OpenTextFile(inpname, 1, true)
var tmp = fso.OpenTextFile(tmpname, 2, true)
var re = [
	/^(\s*FILEVERSION\s+\d+,)(\d+)(,\d+,\d+.*)$/i,
	/^(\s*PRODUCTVERSION\s+\d+,)(\d+)(,\d+,\d+.*)$/i,
	/^(\s*VALUE\s*"FileVersion",\s*"\d+\.)(\d+)(\.\d+\.\d+.*)$/i,
	/^(\s*VALUE\s*"ProductVersion",\s*"\d+\.)(\d+)(\.\d+\.\d+.*)$/i
]
while(!inp.AtEndOfStream){
	s = inp.ReadLine()
	for(var i = re.length; --i >= 0;){
		var m = s.match(re[i])
		if(m !== null){
			m[2]++
			s = m[1] + m[2] + m[3]
		}
	}
	tmp.WriteLine(s)
}
tmp.Close()
inp.Close()
inp = fso.GetFile(inpname)
inp.Delete()
tmp = fso.GetFile(tmpname)
tmp.Move(inpname)
