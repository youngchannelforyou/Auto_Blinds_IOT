var autoBtnisOn = true;
var autoBtn = document.getElementById("autoBtn");

var upBtnisOn = false;
var upBtn = document.getElementById("upBtn");
upBtn.disabled = true;

var downBtnisOn = false;
var downBtn = document.getElementById("downBtn");
downBtn.disabled = true;

var modeBtnisOn = true;
var modeBtn = document.getElementById("modeBtn");

var AIBtnisOn = false;
var Aibtn = document.getElementById("AIBtn");

autoBtn.addEventListener("click", function () {
    autoBtnisOn = !autoBtnisOn;

    autoBtn.innerHTML = autoBtnisOn ? '자동' : '수동';

    // 자동 모드이면
    if (autoBtnisOn) {
        upBtn.innerHTML = '올리기';
        upBtn.style.backgroundColor = "gray";
        upBtnisOn = false;
        downBtn.innerHTML = '내리기';
        downBtn.style.backgroundColor = "gray";
        downBtnisOn = false;
        upBtn.disabled = true;
        downBtn.disabled = true;
        modeBtn.disabled = false;
        Aibtn.disabled = true;
        modeBtn.style.backgroundColor = "#4CAF50";
    } else { // 수동 모드일때 
        upBtn.style.backgroundColor = "#4CAF50";
        downBtn.style.backgroundColor = "#4CAF50";
        modeBtn.disabled = true;
        Aibtn.disabled = false;
        modeBtn.style.backgroundColor = "gray";
        upBtn.disabled = false;
        downBtn.disabled = false;
    }
    sendData("/stoprun");
    sendData("/auto");
});

Aibtn.addEventListener("click", function () {
    AIBtnisOn = !AIBtnisOn;

    if (AIBtnisOn == true) {
        Aibtn.disabled = true
        downBtn.style.backgroundColor = "gray";
        sendData("/ai")
    } else {
        Aibtn.disabled = false
        Aibtn.style.backgroundColor = "#4CAF50";
        sendData("/ai");
    }
});

upBtn.addEventListener("click", function () {
    upBtnisOn = !upBtnisOn;

    upBtn.innerHTML = upBtnisOn ? '올라가는 중' : '올리기';

    if (upBtnisOn == true) {
        downBtn.disabled = true
        downBtn.style.backgroundColor = "gray";
        sendData("/uprun")
    } else {
        downBtn.disabled = false
        downBtn.style.backgroundColor = "#4CAF50";
        sendData("/stoprun");
    }
});

downBtn.addEventListener("click", function () {

    downBtnisOn = !downBtnisOn;

    downBtn.innerHTML = downBtnisOn ? '내려가는 중' : '내리기';

    if (downBtnisOn == true) {
        upBtn.disabled = true
        upBtn.style.backgroundColor = "gray";
        sendData("/downrun")
    } else {
        upBtn.disabled = false
        upBtn.style.backgroundColor = "#4CAF50";
        sendData("/stoprun");
    }
});

modeBtn.addEventListener("click", function () {
    if (!upBtnisOn) {
        modeBtnisOn = !modeBtnisOn;

        modeBtn.innerHTML = modeBtnisOn ? '주간 모드' : '야간 모드';

        sendData('/autotype');
    }
});

function sendData(path) {
    var xhr = new XMLHttpRequest();
    xhr.open("POST", path, true);
    xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    xhr.onreadystatechange = function () {
        if (xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
            console.log("Data sent: " + path);
        } else {
            console.log("Error:", xhr.status);
        }
    };
    xhr.send();
}

function getCurrentTime() {
    var now = new Date();
    var hours = now.getHours();
    var minutes = now.getMinutes();
    var seconds = now.getSeconds();

    // Leading zeros padding
    hours = String(hours).padStart(2, '0');
    minutes = String(minutes).padStart(2, '0');
    seconds = String(seconds).padStart(2, '0');

    var currentTime = hours + ':' + minutes + ':' + seconds;

    return currentTime;
}

function updateTime() {
    var currentTimeElement = document.getElementById("currentTime");
    currentTimeElement.textContent = getCurrentTime();
}

// Update time every second
setInterval(updateTime, 1000);
