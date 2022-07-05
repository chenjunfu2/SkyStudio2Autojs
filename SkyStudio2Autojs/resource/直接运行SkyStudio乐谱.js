"auto"
var arr = [];
var time;
var score;
var zuobiaoPath = "/sdcard/Download/SkyMsToJs/zuobiao.txt";
if (files.exists(zuobiaoPath)) {
    eval(files.read(zuobiaoPath));
} else {
    setScreenMetrics(1080, 2340);
    x=[780, 975, 1170, 1365, 1560];
    y=[215, 410, 605];
};
var sheetPath = "/storage/emulated/0/光遇乐谱/" //默认乐谱存放路径

function getScore() {
    files.ensureDir(sheetPath)
    data = files.listDir(sheetPath)
    let showNames = [];
    for (i = 0; i < data.length; i++) {
        if (data[i].search(".txt") != -1) {
            showNames.push(data[i]);
        }
    }
    if (showNames.length == 0) {
        alert("请将乐谱文件存放在该路径下：" + sheetPath)
        exit()
    }
    let choose = dialogs.select("选择乐谱", showNames);
    if (choose != -1) {
        data = files.read(sheetPath + showNames[choose], encoding = "utf-16")
        data = JSON.parse(data)[0]
        var myScore = {};
        myScore.name = data.name;
        myScore.author = data.author;
        myScore.score = change(data.songNotes);
        play(myScore);
    } else {
        toast("你没有选谱子")
    }
}

function change(score) {
    let finalScore = [];
    let partScore = {};
    let pitchesDuration = 0;
    let now,
        nowTime,
        nowPitches,
        lastTime = 0;
    for (i = 0; i < score.length; i++) {
        now = score[i];
        nowTime = now.time;
        nowPitches = now.key;
        nowPitches = nowPitches.slice(4, nowPitches.length)
        partScore.pitches = nowPitches;
        pitchesDuration = Number(nowTime) - Number(lastTime);
        partScore.time = pitchesDuration;
        finalScore.push(partScore);
        partScore = {}; //important
        lastTime = nowTime;
    }
    return finalScore;
}

function play(myScore) {
    //暂停功能
    var s = 0;
    var speedControl = 1.0;
    var window = floaty.window(<frame>
    <vertical>
    <button id="btn" text='继续'/>
    <horizontal>
    <button id="speedLow" text='减速'/>
    <button id="speedHigh" text='加速'/>
    </horizontal>
    <button id="stop" text='停止'/>
    </vertical></frame>);
    window.exitOnClose();
    window.btn.click(() => {
        if (window.btn.getText() != '暂停') {
            s = 1;
            window.btn.setText('暂停')
        } else {
            s = 0;
            window.btn.setText('继续')
        }
    });
    window.speedHigh.click(() => {
        speedControl = speedControl + 0.1
        toast("当前速度：x"+speedControl)
    })
    window.speedLow.click(() => {
        speedControl = speedControl - 0.1
        toast("当前速度：x"+speedControl)
    })
    window.stop.click(() => {
        engines.stopAll()
    })
    toast("正在弹奏：" + myScore.name)
    let score = myScore.score;
    for (let i = 0; i < score.length; i++) {
        while (s != 1) {
            sleep(100)
        }; //暂停功能
        sleep(score[i].time / speedControl);
        strToxy(score[i].pitches);
        press(arr[0], arr[1], 1);
    }
}

function strToxy(pitches) {
    arr = [];
    arr[0] = x[Number(pitches) % 5];
    arr[1] = y[Math.trunc(Number(pitches) / 5)];
}

getScore();
exit();
