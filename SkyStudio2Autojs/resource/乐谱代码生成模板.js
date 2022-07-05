var time=/*t1*/;var time2=/*t2*/;var time4=/*t4*/;var pressoffset=/*o*/;var pressoffset2=/*o2*/;var stop=1;var speedControl=1;var zuobiaoPath = "/sdcard/Download/SkyMsToJs/zuobiao.txt";
if (files.exists(zuobiaoPath)) {eval(files.read(zuobiaoPath));}else{setScreenMetrics(1080, 2340);var x=[780,975,1170,1365,1560];var y=[215,410,605];}
/*random*/function ran() {return Math.random()*pressoffset2-pressoffset;}
function c4() {press(x[0]+ran(),y[0]+ran(),1);}
function d4() {press(x[1]+ran(),y[0]+ran(),1);}
function e4() {press(x[2]+ran(),y[0]+ran(),1);}
function f4() {press(x[3]+ran(),y[0]+ran(),1);}
function g4() {press(x[4]+ran(),y[0]+ran(),1);}
function a4() {press(x[0]+ran(),y[1]+ran(),1);}
function b4() {press(x[1]+ran(),y[1]+ran(),1);}
function c5() {press(x[2]+ran(),y[1]+ran(),1);}
function d5() {press(x[3]+ran(),y[1]+ran(),1);}
function e5() {press(x[4]+ran(),y[1]+ran(),1);}
function f5() {press(x[0]+ran(),y[2]+ran(),1);}
function g5() {press(x[1]+ran(),y[2]+ran(),1);}
function a5() {press(x[2]+ran(),y[2]+ran(),1);}
function b5() {press(x[3]+ran(),y[2]+ran(),1);}
function c6() {press(x[4]+ran(),y[2]+ran(),1);}
/*nrandom*/function c4() {press(x[0],y[0],1);}
function d4() {press(x[1],y[0],1);}
function e4() {press(x[2],y[0],1);}
function f4() {press(x[3],y[0],1);}
function g4() {press(x[4],y[0],1);}
function a4() {press(x[0],y[1],1);}
function b4() {press(x[1],y[1],1);}
function c5() {press(x[2],y[1],1);}
function d5() {press(x[3],y[1],1);}
function e5() {press(x[4],y[1],1);}
function f5() {press(x[0],y[2],1);}
function g5() {press(x[1],y[2],1);}
function a5() {press(x[2],y[2],1);}
function b5() {press(x[3],y[2],1);}
function c6() {press(x[4],y[2],1);}
/*window*/var window = floaty.window(<frame><vertical><button id="btn" text='开始'/><horizontal><button id="speedLow" text='减速' w="80"/><button id="speedHigh" text='加速' w="80"/></horizontal><horizontal><button id="speed" text='1.0x' w="80"/><button id="stop" text='停止' w="80"/></horizontal></vertical></frame>);
window.exitOnClose();
window.btn.click(()=>{if(stop) {stop = 0;window.btn.setText('暂停');}else{stop = 1;window.btn.setText('继续');}})
window.speedHigh.click(()=>{speedControl=(speedControl*10+1)/10;window.speed.setText(speedControl+'x');})
window.speedLow.click(()=>{if(speedControl<=0.1){return;}speedControl=(speedControl*10-1)/10;window.speed.setText(speedControl+'x');})
window.speed.click(()=>{speedControl=1;window.speed.setText(speedControl+'x');})
window.stop.click(()=>{engines.stopAll();})
function start() {while (stop) {sleep(100);}}
function t1() {while (stop) {sleep(100)}sleep(time/speedControl);}
function t2() {while (stop) {sleep(100)}sleep(time2/speedControl);}
function t4() {while (stop) {sleep(100)}sleep(time4/speedControl);}
start();
/*nwindow*/function t1() {sleep(time);}
function t2() {sleep(time2);}
function t4() {sleep(time4);}
sleep(/*s*/);/*end*/