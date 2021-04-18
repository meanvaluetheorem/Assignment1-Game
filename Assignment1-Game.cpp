#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bangtal.h>

SceneID scene1, scene2, scene3;
SoundID sound1;
ObjectID button1, button2, marble_numberbutton[4], backbutton[2], startbutton[2], endbutton[3], mixbutton;
ObjectID papercup[4], marble, arrow1, arrow2;
int cupX[4] = { 200,450,680,900 };
int marbleX[4] = { 270,520,750,975 };
int marbleY = 500;

TimerID game1_timer1, game1_timer2, game1_timer3;
float timerValue1 = 0.01f, timerValue2 = 0.5f, timerValue3 = 0.5f;

int index1;		//사용자가 몇번째에 구슬을 놓을건지 결정, 그리고 그 값 저장
int answer;

int mixCount = 5;	//컵 섞는 횟수
int mixCount_origin = 5;

//음악재생 함수
SoundID playsound(SoundID sound, const char* soundname, const char* soundfile, bool playing , bool loop) {
	sound = createSound(soundfile);
	if (playing == true) {
		playSound(sound, loop);
	}
	return sound;
}

//물체배열 함수
ObjectID DIY(ObjectID object, const char* objectname, const char* filename, SceneID scene, int x, int y, bool shown = true) {
	object = createObject(filename);
	locateObject(object, scene, x, y);
	if (shown == true) {
		showObject(object);
	}
	return object;
}

//게임 실행 중인가 아닌가 여부
bool game1_on = false, game2_on = false;
//게임 종료 여부
bool game1_end = false;

//야바위게임 세팅 함수
void game1set1() {
	hideObject(startbutton[0]);
	game1_on = true;
	for (int i = 0; i < 4; i++) showObject(marble_numberbutton[i]);
	showMessage("구슬 놓을 위치를 고르세요.");
}

//몇번째 버튼 골랐는지 확인하는 함수
int get_Index(ObjectID object) {
	for (int i = 0; i < 4; i++) {
		if (object == marble_numberbutton[i]) return i;
	}
	return -1;
}

//구슬 떨어뜨리는 함수
void marble_drop(int i) {
	marbleY -= 7;
	locateObject(marble, scene2, marbleX[i], marbleY);

	int marbleFinalY = 70;
	if (marbleY > marbleFinalY) {
		setTimer(game1_timer1, timerValue1);
		startTimer(game1_timer1);
	}
	else {
		for (int i = 0; i < 4; i++) {
			papercup[i] = DIY(papercup[i], "컵", "Images/papercup.png", scene2, cupX[i], 30, true);
			scaleObject(papercup[i], 1.6f);
		}
		mixbutton = DIY(mixbutton, "섞기버튼", "Images/mix.png", scene2, 620, 380);
		scaleObject(mixbutton, 1.2f);
		showMessage("화살표가 뜨는 컵끼리 섞는 것입니다.");
	}
}

//컵 섞어주는 함수
void mix_cup() {
	int a = rand() % 4;
	int b = rand() % 4;
	if (a != b) {
		int temp1 = a, temp2 = b;

		ObjectID object = papercup[a];

		papercup[a] = papercup[b];
		locateObject(papercup[a], scene2, cupX[a] - 10, 30);

		papercup[b] = object;
		locateObject(papercup[b], scene2, cupX[b] - 10, 30);

		a = temp2, b = temp1;

		arrow1 = DIY(arrow1, "화살표", "Images/arrow.png", scene2, cupX[a] + 70, 332);
		arrow2 = DIY(arrow2, "화살표", "Images/arrow.png", scene2, cupX[b] + 70, 332);

		if (a == index1) index1 = a;
		else if (b == index1) index1 = b;
		mixCount--;

		setTimer(game1_timer3, timerValue3);
		startTimer(game1_timer3);
	}
}

//사다리게임 세팅 함수
void game2set1() {
	hideObject(startbutton[1]);
	game2_on = true;
}

void mousecallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == button1) {
		enterScene(scene2);
		showMessage("네개의 컵중에 어디에 구슬이 있는지 알아맞춰 보세요.");
	}
	else if (object == button2) enterScene(scene3);
	else if (object == endbutton[0] || object == endbutton[1] || object == endbutton[2]) endGame();

	else if (game1_on == false && game2_on == false) {
		if (object == backbutton[0] || object == backbutton[1]) enterScene(scene1);
		else if (object == startbutton[0]) {
			game1set1();
		}
		else if (object == startbutton[1]) game2set1();
	}

	else if (game1_on == true && game2_on == false) {
		index1 = get_Index(object);

		if (object == backbutton[0]) {
			enterScene(scene2);
			game1_on = false;
			showObject(startbutton[0]);
			for (int i = 0; i < 4; i++) {
				hideObject(papercup[i]);
				hideObject(marble_numberbutton[i]);
			}
			locateObject(marble, scene2, marbleX[0], 500);
			hideObject(marble);
			marbleY = 500;
			hideObject(mixbutton);
			hideObject(arrow1);
			hideObject(arrow2);
			mixCount = mixCount_origin;
		}
		else if (object == marble_numberbutton[index1]) {
			marble = DIY(marble, "구슬", "Images/earthmarble.png", scene2, marbleX[index1], marbleY, true);
			marble_drop(index1);
			for (int i = 0; i < 4; i++) hideObject(marble_numberbutton[i]);
		}
		else if (object == mixbutton) {
			mix_cup();
			hideObject(mixbutton);
		}
		else if (game1_end == true) {
			if (object == papercup[index1]) {
				showMessage("정답입니다!!");
				for (int i = 0; i < 4; i++) hideObject(papercup[i]);
				game1_end = false;
			}
			else if (object != papercup[index1]) {
				showMessage("틀렸습니다...");
				for (int i = 0; i < 4; i++) hideObject(papercup[i]);
				game1_end = false;
			}
		}
	}
	else if (game1_on == false && game2_on == true) {
		if (object == backbutton[1]) {
			enterScene(scene3);
			game2_on = false;
			showObject(startbutton[1]);
		}
	}
}

void timercallback(TimerID timer) {
	if (timer == game1_timer1) marble_drop(index1);

	else if (timer == game1_timer2) {
		if (mixCount > 0) {
			mix_cup();
			setTimer(game1_timer3, timerValue3);
			startTimer(game1_timer3);
		}
		else if (mixCount <= 0) {
			showMessage("다 섞었습니다!\n정답을 골라주세요.");
			answer = index1;
			hideObject(arrow1);
			hideObject(arrow2);
			locateObject(marble, scene2, marbleX[answer], marbleY);
			game1_end = true;
		}
	}

	else if (timer == game1_timer3) {
		if (mixCount >= 0) {
			setTimer(game1_timer2, timerValue2);
			startTimer(game1_timer2);
			hideObject(arrow1);
			hideObject(arrow2);
		}
	}
}

int main() {
	setMouseCallback(mousecallback);
	setTimerCallback(timercallback);

	srand(time(NULL));

	scene1 = createScene("놀이터(대기실)", "Images/playground.png");
	scene2 = createScene("야바위", "Images/Cups and balls with cats.png");
	scene3 = createScene("사다리타기", "Images/Ladder.png");

	sound1 = playsound(sound1, "playground", "Audios/GhostLegBGM.mp3", true, true);
	
	game1_timer1 = createTimer(timerValue1);
	game1_timer2 = createTimer(timerValue2);
	game1_timer3 = createTimer(timerValue3);


	button1 = DIY(button1, "tirckygame", "Images/button-playwithtrickycats.png", scene1, 490, 1);
	scaleObject(button1, 0.55f);

	button2 = DIY(button1, "ladder", "Images/button-playwithladder.png", scene1, 140, 50);
	scaleObject(button2, 0.7f);

	backbutton[0] = DIY(backbutton[0], "뒤로가기", "Images/back.png", scene2, 0, 580);
	backbutton[1] = DIY(backbutton[1], "뒤로가기", "Images/back.png", scene3, 0, 570);

	startbutton[0] = DIY(startbutton[0], "게임1 시작버튼", "Images/start.png", scene2, 1150, 380);
	startbutton[1] = DIY(startbutton[1], "게임2 시작버튼", "Images/start.png", scene3, 680, 60);

	endbutton[0] = DIY(endbutton[0], "종료", "Images/end.png", scene1, 1150, 300);
	endbutton[1] = DIY(endbutton[1], "종료", "Images/end.png", scene2, 1150, 300);
	endbutton[2] = DIY(endbutton[2], "종료", "Images/end.png", scene3, 490, 60);

	//야바위 구슬 배치
	//marble = DIY(marble, "구슬", "Images/earthmarble.png", scene2, marbleX[i], marbleY);


	marble_numberbutton[0] = DIY(marble_numberbutton[0], "숫자1", "Images/number-1.png", scene2, 270, 285, false);
	marble_numberbutton[1] = DIY(marble_numberbutton[1], "숫자2", "Images/number-2.png", scene2, 520, 285, false);
	marble_numberbutton[2] = DIY(marble_numberbutton[2], "숫자3", "Images/number-3.png", scene2, 750, 285, false);
	marble_numberbutton[3] = DIY(marble_numberbutton[3], "숫자4", "Images/number-4.png", scene2, 975, 285, false);


	startGame(scene1);
}