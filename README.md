# SkyStudio2Autojs
转换sky studio乐谱到自动化弹琴脚本<br>

把要转换的（多个）skystudio乐谱（只接受未加密的json格式，切记！）<br>
拖动到SkyStudio2Autojs.exe上，就会自动进行转换<br>
也可以双击程序打开cmd使用命令行参数进行转换<br>
命令行内键入程序名，后跟/h或/?获得详细命令帮助<br>
如果提示转换失败或生成的文件是空文件，请检查源乐谱文件是不是未加密的json格式！<br>

如果出现bug请找 QQ:1060495688 反馈<br>

附（未加密的json格式乐谱示例）：<br>
[{"name":"示例乐谱","author":"Unknown","transcribedBy":"Unknown","isComposed":true,"bpm":480,"bitsPerPage":16,"pitchLevel":0,"isEncrypted":false,"songNotes":[{"time":0,"key":"2Key7"},{"time":500,"key":"2Key7"},{"time":1000,"key":"1Key0"},{"time":1000,"key":"1Key4"},{"time":1000,"key":"2Key11"},{"time":1500,"key":"2Key9"},{"time":2000,"key":"1Key0"},{"time":2000,"key":"1Key2"},{"time":2000,"key":"2Key9"}]}]
