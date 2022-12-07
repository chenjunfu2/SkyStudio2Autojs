# SkyStudio2Autojs
转换SkyStudio乐谱到Autojs自动化脚本<br>
支持未加密的Json格式乐谱和ABC格式乐谱。<br>

如果出现bug请找 QQ:1060495688 反馈<br>

Windows版：
把要转换的（多个）SkyStudio乐谱拖动到SkyStudio2Autojs.exe上，就会自动进行转换，<br>
也可以双击程序打开交互模式进行转换、修改设置和导出辅助脚本。<br>
如果提示失败或生成的文件是空文件，请检查源乐谱文件是不是未加密的json格式或ABC格式！<br>

Android版：
安装后打开程序进入交互模式，进行转换、修改设置和导出辅助脚本。<br>
默认源目录是/storage/emulated/0/SkyStudio2Autojs/<br>
默认目标目录是/storage/emulated/0/脚本/<br>
在转换模式下，通过输入源目录下的乐谱文件名，可以转换乐谱并生成脚本在目标目录下。<br>
默认目标目录是Autojs程序的脚本目录，相当于转换后直接导入Autojs。<br>

附：
未加密的json格式乐谱示例：<br>
\[{"name":"示例乐谱","author":"Unknown","transcribedBy":"Unknown","isComposed":true,"bpm":480,"bitsPerPage":16,"pitchLevel":0,"isEncrypted":false,"songNotes":\[{"time":0,"key":"2Key7"},{"time":500,"key":"2Key7"},{"time":1000,"key":"1Key0"},{"time":1000,"key":"1Key4"},{"time":1000,"key":"2Key11"},{"time":1500,"key":"2Key9"},{"time":2000,"key":"1Key0"},{"time":2000,"key":"1Key2"},{"time":2000,"key":"2Key9"}]}]<br>

ABC格式乐谱示例：<br>
<DontCopyThisLine> 480 0 16<br>
B3 . . . B3 . . . A1A5C2 . . . B5 . . . A1A3B5 . . . . . . . . . . . . . . . <br>

转换后的js脚本示例：<br>
var time=125;var time2=250;var time4=500;var pressoffset=18;var pressoffset2=36;var stop=1;var speedControl=1;var zuobiaoPath = "/sdcard/Download/SkyMsToJs/zuobiao.txt";<br>
if (files.exists(zuobiaoPath)) {eval(files.read(zuobiaoPath));}else{setScreenMetrics(1080, 2340);var x=[780,975,1170,1365,1560];var y=[215,410,605];}<br>
function ran() {return Math.random()*pressoffset2-pressoffset;}<br>
function c4() {press(x[0]+ran(),y[0]+ran(),1);}<br>
function d4() {press(x[1]+ran(),y[0]+ran(),1);}<br>
function e4() {press(x[2]+ran(),y[0]+ran(),1);}<br>
function f4() {press(x[3]+ran(),y[0]+ran(),1);}<br>
function g4() {press(x[4]+ran(),y[0]+ran(),1);}<br>
function a4() {press(x[0]+ran(),y[1]+ran(),1);}<br>
function b4() {press(x[1]+ran(),y[1]+ran(),1);}<br>
function c5() {press(x[2]+ran(),y[1]+ran(),1);}<br>
function d5() {press(x[3]+ran(),y[1]+ran(),1);}<br>
function e5() {press(x[4]+ran(),y[1]+ran(),1);}<br>
function f5() {press(x[0]+ran(),y[2]+ran(),1);}<br>
function g5() {press(x[1]+ran(),y[2]+ran(),1);}<br>
function a5() {press(x[2]+ran(),y[2]+ran(),1);}<br>
function b5() {press(x[3]+ran(),y[2]+ran(),1);}<br>
function c6() {press(x[4]+ran(),y[2]+ran(),1);}<br>
var window = floaty.window(<frame><vertical><button id="btn" text='开始'/><horizontal><button id="speedLow" text='减速' w="80"/><button id="speedHigh" text='加速' w="80"/></horizontal><horizontal><button id="speed" text='1.0x' w="80"/><button id="stop" text='停止' w="80"/></horizontal></vertical></frame>);<br>
window.exitOnClose();<br>
window.btn.click(()=>{if(stop) {stop = 0;window.btn.setText('暂停');}else{stop = 1;window.btn.setText('继续');}})<br>
window.speedHigh.click(()=>{speedControl=(speedControl\*10+1)/10;window.speed.setText(speedControl+'x');})<br>
window.speedLow.click(()=>{if(speedControl<=0.1){return;}speedControl=(speedControl\*10-1)/10;window.speed.setText(speedControl+'x');})<br>
window.speed.click(()=>{speedControl=1;window.speed.setText(speedControl+'x');})<br>
window.stop.click(()=>{engines.stopAll();})<br>
function start() {while (stop) {sleep(100);}}<br>
function t1() {while (stop) {sleep(100)}sleep(time/speedControl);}<br>
function t2() {while (stop) {sleep(100)}sleep(time2/speedControl);}<br>
function t4() {while (stop) {sleep(100)}sleep(time4/speedControl);}<br>
start();<br>
c5();t4();c5();t4();c4();g4();g5();t4();e5();t4();c4();e4();e5();<br>
