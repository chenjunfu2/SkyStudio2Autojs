#pragma once

#include <stdio.h>

bool Read2Str(FILE* f, const wchar_t* str)
{
	int i;
	wchar_t c;
rer:
	i = 0;
	while (str[i] && !feof(f) && (c = fgetwc(f)) != str[i])
		continue;

	while (str[++i] && !feof(f) && (c = fgetwc(f)) == str[i])
		continue;

	if (feof(f) && str[i])
		return false;

	if (str[i])
		goto rer;

	return true;
}

bool ReadUntilChar(FILE* f, char c, wchar_t* str, int size)
{
	int i;
	for (i = 0; i < size && !feof(f) && (str[i] = fgetwc(f)) != c; ++i)
		continue;

	if (feof(f) || i >= size && str[size - 1] != c || str[i] != c)
		return false;

	if (str[i] == c)
	{
		str[i] = 0;
		return true;
	}

	return false;
}

unsigned long strcpy(char* tar, const char* sour)
{
	unsigned long i = 0;
	while (tar[i] = sour[i])
		++i;
	return i;
}

unsigned long strlen(const char* str)
{
	unsigned long i = 0;
	while (str[i])
		++i;
	return i + 1;
}


const static char* noteFunc[] =
{
	"c4();" ,
	"d4();" ,
	"e4();" ,
	"f4();" ,
	"g4();" ,

	"a4();" ,
	"b4();" ,
	"c5();" ,
	"d5();" ,
	"e5();" ,

	"f5();" ,
	"g5();" ,
	"a5();" ,
	"b5();" ,
	"c6();" ,
};

const static char* timeFunc[] =
{
	"t1();",
	"t2();",
	"t4();",
};

const static char* code[] =
{
	"var time=",//0
	"var time2=",//1
	"var time4=",//2
	"var pressoffset=",//3
	"var pressoffset2=",//4
	"var stop=1;",//5
	"var speedControl=1;",
	"var speed=1;",
	"var window = floaty.window(<frame><vertical><button id=\"btn\" text=\'\xE5\xBC\x80\xE5\xA7\x8B\'/><horizontal><button id=\"speedLow\" text=\'\xE5\x87\x8F\xE9\x80\x9F\' w=\"80\"/><button id=\"speedHigh\" text=\'\xE5\x8A\xA0\xE9\x80\x9F\' w=\"80\"/></horizontal><horizontal><button id=\"speed\" text=\'1.0x\' w=\"80\"/><button id=\"stop\" text=\'\xE5\x81\x9C\xE6\xAD\xA2\' w=\"80\"/></horizontal></vertical></frame>);",
	"window.exitOnClose();",
	"window.btn.click(()=>{if(window.btn.getText() != \'\xE6\x9A\x82\xE5\x81\x9C\') {stop = 0;window.btn.setText(\'\xE6\x9A\x82\xE5\x81\x9C\');}else{stop = 1;window.btn.setText(\'\xE7\xBB\xA7\xE7\xBB\xAD\');}})",
	"window.speedHigh.click(()=>{speedControl+=0.1;speed=1/speedControl;window.speed.setText(speedControl.toFixed(1)+\'x\');})",
	"window.speedLow.click(()=>{if(speedControl.toFixed(1)<=0.1){return;}speedControl-=0.1;speed=1/speedControl;window.speed.setText(speedControl.toFixed(1)+\'x\');})",
	"window.speed.click(()=>{speedControl=1;speed=1;window.speed.setText(speedControl.toFixed(1)+\'x\');})",
	"window.stop.click(()=>{engines.stopAll();})",
	"var zuobiaoPath = \"/sdcard/Download/SkyMsToJs/zuobiao.txt\";",
	"if (files.exists(zuobiaoPath)) {eval(files.read(zuobiaoPath));}else{setScreenMetrics(1080, 2340);var x=[780,975,1170,1365,1560];var y=[215,410,605];}",
	"function ran() {return Math.random()*pressoffset2-pressoffset;}",
	"function c4() {press(x[0]+ran(),y[0]+ran(),1);}",
	"function d4() {press(x[1]+ran(),y[0]+ran(),1);}",
	"function e4() {press(x[2]+ran(),y[0]+ran(),1);}",
	"function f4() {press(x[3]+ran(),y[0]+ran(),1);}",
	"function g4() {press(x[4]+ran(),y[0]+ran(),1);}",
	"function a4() {press(x[0]+ran(),y[1]+ran(),1);}",
	"function b4() {press(x[1]+ran(),y[1]+ran(),1);}",
	"function c5() {press(x[2]+ran(),y[1]+ran(),1);}",
	"function d5() {press(x[3]+ran(),y[1]+ran(),1);}",
	"function e5() {press(x[4]+ran(),y[1]+ran(),1);}",
	"function f5() {press(x[0]+ran(),y[2]+ran(),1);}",
	"function g5() {press(x[1]+ran(),y[2]+ran(),1);}",
	"function a5() {press(x[2]+ran(),y[2]+ran(),1);}",
	"function b5() {press(x[3]+ran(),y[2]+ran(),1);}",
	"function c6() {press(x[4]+ran(),y[2]+ran(),1);}",
	"function start() {while (stop) {sleep(100);}}",
	"function t1() {while (stop) {sleep(100)}sleep(time*speed);}",
	"function t2() {while (stop) {sleep(100)}sleep(time2*speed);}",
	"function t4() {while (stop) {sleep(100)}sleep(time4*speed);}",
	"start();",
};