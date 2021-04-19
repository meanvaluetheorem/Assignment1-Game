#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bangtal.h>

SceneID scene1, scene2, scene3;
SoundID sound1;
ObjectID button1, button2, marble_numberbutton[4], backbutton[2], startbutton[2], endbutton[3];
ObjectID easybutton, normalbutton, hardbutton, hellbutton;
ObjectID papercup[4], marble, arrow1, arrow2;
ObjectID legcase[8], cover[3], start[2], end[2], mid[4];
int cupX[4] = { 200,450,680,900 };
int marbleX[4] = { 270,520,750,975 };
int marbleY = 500;

TimerID game1_timer1, game1_timer2, game1_timer3;
float timerValue1 = 0.01f, timerValue2, timerValue3;

int index1;		//사용자가 몇번째에 구슬을 놓을건지 결정, 그리고 그 값 저장
int index2;

int mixCount, mixCount_origin;	//컵 섞는 횟수

int index3;

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
//게임① 종료 여부
bool game1_end = false;
//게임② 종료 여부
bool game2_game1_end = false, game2_game2_end = false;






//게임① 세팅 함수
void game1set1() {
	hideObject(startbutton[0]);
	game1_on = true;
	for (int i = 0; i < 4; i++) showObject(marble_numberbutton[i]);
	showMessage("구슬을 놓을 위치를 고르세요.");
}

//게임①-몇번째 버튼 골랐는지 확인하는 함수
int get_Index(ObjectID object) {
	for (int i = 0; i < 4; i++) {
		if (object == marble_numberbutton[i]) return i;
	}
	return -1;
}

//게임①-구슬 떨어뜨리는 함수
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
		easybutton = DIY(easybutton, "난이도 쉬움", "Images/easy.png", scene2, 120, 460);
		normalbutton = DIY(normalbutton, "난이도 보통", "Images/normal.png", scene2, 420, 460);
		hardbutton = DIY(hardbutton, "난이도 어려움", "Images/hard.png", scene2, 780, 460);
		hellbutton = DIY(hellbutton, "난이도 지옥", "Images/hell.png", scene2, 1100, 460);
		showMessage("난이도를 선택해주세요.\n참고로 화살표는 섞는 컵을 알려줍니다.");
	}
}

//게임①-컵 섞어주는 함수
void mix_cup() {
	int a = rand() % 4;
	int b = rand() % 4;
	if (a != b) {
		int temp1 = a, temp2 = b;

		ObjectID object = papercup[a];

		papercup[a] = papercup[b];
		locateObject(papercup[a], scene2, cupX[a] - 10, 30);	//처음 섞는 종이컵을 -10정도 움직이게 함으로써 눈치채도록 만듬

		papercup[b] = object;
		locateObject(papercup[b], scene2, cupX[b] - 10, 30);	//처음 섞는 종이컵을 -10정도 움직이게 함으로써 눈치채도록 만듬

		a = temp2, b = temp1;

		arrow1 = DIY(arrow1, "화살표", "Images/arrow.png", scene2, cupX[a] + 70, 332);
		arrow2 = DIY(arrow2, "화살표", "Images/arrow.png", scene2, cupX[b] + 70, 332);

		if (a == index2) index2 = b;
		else if (b == index2) index2 = a;
		mixCount--;

		setTimer(game1_timer3, timerValue3);
		startTimer(game1_timer3);
	}
	else {
		setTimer(game1_timer2, timerValue2);
		startTimer(game1_timer2);
	}
}






//게임② 세팅 함수
void game2set1() {
	hideObject(startbutton[1]);
	game2_on = true;
	showMessage("이 게임은 총 3단계로 구성되어 있습니다.\n첫번째 문제! 사다리 출발지점을 골라주세요.");
	locateObject(endbutton[2], scene3, 1150, 300);

	index3 = rand() % 8;
	showObject(legcase[index3]);

	cover[2] = DIY(cover[2], "가림막2", "Images/cover2.png", scene3, 350, 160);
	cover[0] = DIY(cover[0], "가림막", "Images/cover1.png", scene3, 335, 460);
	cover[1] = DIY(cover[1], "가림막", "Images/cover1.png", scene3, 335, 40);

	start[0] = DIY(start[0], "1출발", "Images/number-1.png", scene3, 500, 520);
	start[1] = DIY(start[1], "2출발", "Images/number-2.png", scene3, 690, 520);

	end[0] = DIY(end[0], "Left도착", "Images/left.png", scene3, 460, 150, false);
	end[1] = DIY(end[1], "Right도착", "Images/right.png", scene3, 690, 150, false);

	mid[0] = DIY(mid[0], "사다리개수1", "Images/number-1.png", scene3, 300, 350, false);
	mid[1] = DIY(mid[1], "사다리개수2", "Images/number-2.png", scene3, 500, 350, false);
	mid[2] = DIY(mid[2], "사다리개수3", "Images/number-3.png", scene3, 700, 350, false);
	mid[3] = DIY(mid[3], "사다리개수4", "Images/number-4.png", scene3, 900, 350, false);
}

//게임②-출발 경우의 수 판별 함수
bool check1_legcase(int i) {
	if ((i % 2) == 0) return true;	//1 출발
	else if ((i % 2) > 0) return false;	//2 출발
}
//게임②-도착 경우의 수 판별 함수
bool check2_legcase(int i) {
	if (i == 1 || i == 2 || i == 5 || i == 6) return true;		//Left 도착
	else if (i == 0 || i == 3|| i == 4 || i == 7) return false;	//Right 도착
}
//게임②-사다리 개수 판별 함수
int check3_legcase(int i) {
	switch (i / 2) {
	case 0: return 0;	//사다리개수 1개
	case 1: return 1;	//사다리개수 2개
	case 2: return 2;	//사다리개수 3개
	case 3: return 3;	//사다리개수 4개
	}
}


void mousecallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == endbutton[0] || object == endbutton[1] || object == endbutton[2]) endGame();

	else if (object == button1) {
		enterScene(scene2);
		showMessage("네개의 컵중에 어디에 구슬이 있는지 알아맞춰 보세요.");
	}
	else if (object == button2) {
		enterScene(scene3);
		showMessage("50대 50의 확률! 사다리타기게임 입니다!");
	}

	//게임 시작 전
	else if (game1_on == false && game2_on == false) {
		if (object == backbutton[0] || object == backbutton[1]) enterScene(scene1);
		else if (object == startbutton[0]) game1set1();
		else if (object == startbutton[1]) game2set1();
	}

	//게임① 시작 후
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
			hideObject(arrow1);
			hideObject(arrow2);
			mixCount = mixCount_origin;
			hideObject(easybutton); hideObject(normalbutton); hideObject(hardbutton); hideObject(hellbutton);
			stopTimer(game1_timer2);	stopTimer(game1_timer3);
		}
		else if (object == marble_numberbutton[index1]) {
			marble = DIY(marble, "구슬", "Images/earthmarble.png", scene2, marbleX[index1], marbleY, true);
			marble_drop(index1);
			index2 = index1;
			for (int i = 0; i < 4; i++) hideObject(marble_numberbutton[i]);
		}
		else if (object == easybutton) {
			timerValue2 = 0.7f, timerValue3 = 0.7f;
			mixCount = 3, mixCount_origin = 3;
			mix_cup();
			hideObject(easybutton); hideObject(normalbutton); hideObject(hardbutton); hideObject(hellbutton);
		}
		else if (object == normalbutton) {
			timerValue2 = 0.52f, timerValue3 = 0.52f;
			mixCount = 8, mixCount_origin = 8;
			mix_cup();
			hideObject(easybutton); hideObject(normalbutton); hideObject(hardbutton); hideObject(hellbutton);
		}
		else if (object == hardbutton) {
			timerValue2 = 0.3f, timerValue3 = 0.3f;
			mixCount = 18, mixCount_origin = 18;
			mix_cup();
			hideObject(easybutton); hideObject(normalbutton); hideObject(hardbutton); hideObject(hellbutton);
		}
		else if (object == hellbutton) {
			timerValue2 = 0.18f, timerValue3 = 0.18f;
			mixCount = 15, mixCount_origin = 15;
			mix_cup();
			hideObject(easybutton); hideObject(normalbutton); hideObject(hardbutton); hideObject(hellbutton);
		}
		else if (game1_end == true) {
			if (object == papercup[index2]) {
				showMessage("정답입니다!!");
				for (int i = 0; i < 4; i++) hideObject(papercup[i]);
				game1_end = false;
			}
			else if (object != papercup[index2]) {
				showMessage("땡! 틀렸습니다^^");
				for (int i = 0; i < 4; i++) hideObject(papercup[i]);
				game1_end = false;
			}
		}
	}

	//게임② 시작 후
	else if (game1_on == false && game2_on == true) {
		int origin = index3;
		int x = origin;

		if (object == backbutton[1]) {
			enterScene(scene3);
			game2_on = false;
			showObject(startbutton[1]);
			locateObject(endbutton[2], scene3, 490, 60);
			hideObject(legcase[index3]);
			for (int i = 0; i < 3; i++) hideObject(cover[i]);
			hideObject(start[0]);	hideObject(start[1]);
			hideObject(end[0]);	hideObject(end[1]);
			for (int i = 0; i < 4; i++) hideObject(mid[i]);
			index3 = origin;
			game2_game1_end = false;	game2_game2_end = false;
		}

		//첫번째 문제 판단
		else if (game2_game1_end == false && game2_game2_end == false) {
			if (check1_legcase(x) == true) {
				if (object == start[0]) {
					showMessage("첫번째 문제 정답!\n두번째 문제! 사다리가 어느 쪽으로 도착할지 골라주세요~");
					hideObject(start[0]);	hideObject(start[1]);
					showObject(end[0]);		showObject(end[1]);
					hideObject(cover[0]);
					game2_game1_end = true;
				}
				else if (object == start[1]) {
					showMessage("땡!^^ 처음부터 다시~");
					hideObject(start[0]);	hideObject(start[1]);
					for (int i = 0; i < 3; i++) hideObject(cover[i]);
				}
			}

			else if (check1_legcase(x) == false) {
				if (object == start[1]) {
					showMessage("첫번째 문제 정답!\n두번째 문제! 사다리가 어느 쪽으로 도착할지 골라주세요~");
					hideObject(start[0]);	hideObject(start[1]);
					showObject(end[0]);		showObject(end[1]);
					hideObject(cover[0]);
					game2_game1_end = true;
				}
				else if (object == start[0]) {
					showMessage("땡!^^ 처음부터 다시~");
					hideObject(start[0]);	hideObject(start[1]);
					for (int i = 0; i < 3; i++) hideObject(cover[i]);
				}
			}
		}

		//두번째 문제 판단
		else if (game2_game1_end == true && game2_game2_end == false) {
			if (check2_legcase(x) == true) {
				if (object == end[0]) {
					showMessage("두번째 문제 정답!\n마지막 문제! 사다리의 개수를 맞춰주세요!");
					hideObject(end[0]);	hideObject(end[1]);
					hideObject(cover[1]);
					for (int i = 0; i < 4; i++) showObject(mid[i]);
					game2_game2_end = true;
				}
				else if (object == end[1]) {
					showMessage("땡!^^ 처음부터 다시~~");
					hideObject(end[0]);	hideObject(end[1]);
					for (int i = 0; i < 3; i++) hideObject(cover[i]);
				}
			}
			else if (check2_legcase(x) == false) {
				if (object == end[1]) {
					showMessage("두번째 문제 정답!\n마지막 문제! 사다리의 개수를 맞춰주세요!");
					hideObject(end[0]);	hideObject(end[1]);
					hideObject(cover[1]);
					for (int i = 0; i < 4; i++) showObject(mid[i]);
					game2_game2_end = true;
				}
				else if (object == end[0]) {
					showMessage("땡!^^ 처음부터 다시~~");
					hideObject(end[0]);	hideObject(end[1]);
					for (int i = 0; i < 3; i++) hideObject(cover[i]);
				}
			}
		}

		//세번째 문제 판단
		else if (game2_game1_end == true && game2_game2_end == true) {
			if (object == mid[0]) {
				if (check3_legcase(x) == 0) {
					showMessage("정답입니다! 축하합니다!");
					for (int i = 0; i < 3; i++) hideObject(cover[i]);
					for (int i = 0; i < 4; i++) hideObject(mid[i]);
				}
				else {
					showMessage("아깝다... 한문제 남았는데...");
					for (int i = 0; i < 3; i++) hideObject(cover[i]);
					for (int i = 0; i < 4; i++) hideObject(mid[i]);
				}
			}
			else if (object == mid[1]) {
				if (check3_legcase(x) == 1) {
					showMessage("정답입니다! 축하합니다!");
					for (int i = 0; i < 3; i++) hideObject(cover[i]);
					for (int i = 0; i < 4; i++) hideObject(mid[i]);
				}
				else {
					showMessage("아깝다... 한문제 남았는데...");
					for (int i = 0; i < 3; i++) hideObject(cover[i]);
					for (int i = 0; i < 4; i++) hideObject(mid[i]);
				}
			}
			else if (object == mid[2]) {
				if (check3_legcase(x) == 2) {
					showMessage("정답입니다! 축하합니다!");
					for (int i = 0; i < 3; i++) hideObject(cover[i]);
					for (int i = 0; i < 4; i++) hideObject(mid[i]);
				}
				else {
					showMessage("아깝다... 한문제 남았는데...");
					for (int i = 0; i < 3; i++) hideObject(cover[i]);
					for (int i = 0; i < 4; i++) hideObject(mid[i]);
				}
			}
			else if (object == mid[3]) {
				if (check3_legcase(x) == 3) {
					showMessage("정답입니다! 축하합니다!");
					for (int i = 0; i < 3; i++) hideObject(cover[i]);
					for (int i = 0; i < 4; i++) hideObject(mid[i]);
				}
				else {
					showMessage("아깝다... 한문제 남았는데...");
					for (int i = 0; i < 3; i++) hideObject(cover[i]);
					for (int i = 0; i < 4; i++) hideObject(mid[i]);
				}
			}
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
			hideObject(arrow1);
			hideObject(arrow2);
			locateObject(marble, scene2, marbleX[index2], marbleY);
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

	sound1 = playsound(sound1, "playgroundsong", "Audios/GhostLegBGM.mp3", true, true);
	
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

	marble_numberbutton[0] = DIY(marble_numberbutton[0], "숫자1", "Images/number-1.png", scene2, 270, 285, false);
	marble_numberbutton[1] = DIY(marble_numberbutton[1], "숫자2", "Images/number-2.png", scene2, 520, 285, false);
	marble_numberbutton[2] = DIY(marble_numberbutton[2], "숫자3", "Images/number-3.png", scene2, 750, 285, false);
	marble_numberbutton[3] = DIY(marble_numberbutton[3], "숫자4", "Images/number-4.png", scene2, 975, 285, false);

	char image[50];
	for (int i = 0; i < 8; i++) {
		sprintf_s(image, "Images/case%d.png", i + 1);
		legcase[i] = DIY(legcase[i], "사다리타기 경우의 수", image, scene3, 300, 20, false);
	}


	startGame(scene1);
}