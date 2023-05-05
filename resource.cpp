#include<stdio.h>
#include<Windows.h>
#include<graphics.h>
#include<time.h>
#include<TCHAR.H>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
int score = 0;
bool gamestop=false;
enum My {
	WIDTH = 500,
	HEIGHT = 808,
	SPEED = 1,
	planeWidth = 94,
	planeHeight = 103,
	BULLET_NUM = 15,
	bulletSpeed = 1,
	ENEMY_NUM = 10,
	ELITE,
	SMALL,
	ELITE_ENEMY_WIDTH = 92,
	ELITE_ENEMY_HEIGHT = 110,
	SMALL_ENEMY_WIDTH = 90,
	SMALL_ENEMY_HEIGHT = 90,
	
};
//define fundemental features of objects 
struct plane {
	double x;
	double y;
	bool live;
	double width;
	double height;
	int hp;
	int type;//type of enemies
}player,bullet[BULLET_NUM],enemy[ENEMY_NUM];

//load image in the application
IMAGE bk;
IMAGE game_role_1;
IMAGE game_role_2;
IMAGE bullet_1;
IMAGE bullet_2;
IMAGE small_enemy_1;
IMAGE small_enemy_2;
IMAGE elite_enemy_1;
IMAGE elite_enemy_2;

//to make things happan reguarly 
bool timer(int ms, int id) {
	static DWORD t[10];
	if (clock() - t[id] > ms) {
		t[id] = clock();
		return true;
	}
	return false;
}
//to load the images
void loadImg(){
	//load the background image
	loadimage(&bk,"./game_plane_image/bk.jpg");
	//load images of player
	loadimage(&game_role_1, "./game_plane_image/game_role_1.png");
	loadimage(&game_role_2, "./game_plane_image/game_role_2.png");
	//load images of bullet
	loadimage(&bullet_1, "./game_plane_image/bullet_1.png");
	loadimage(&bullet_2, "./game_plane_image/bullet_2.png");
	//load the images of enemies
	loadimage(&small_enemy_1, "./game_plane_image/small_enemy_1.png");
	loadimage(&small_enemy_2, "./game_plane_image/small_enemy_2.png");
	loadimage(&elite_enemy_1, "./game_plane_image/elite_enemy_1.png");
	loadimage(&elite_enemy_2, "./game_plane_image/elite_enemy_2.png");
}
//init ststus of the enemies
void enemyHp(int i){
	int temp = rand() % 10;
	//create various types of enemies randomly
	if (temp>=0&&temp<=2) {
		enemy[i].type = ELITE;
		enemy[i].hp = 3;
		enemy[i].width = ELITE_ENEMY_WIDTH;
		enemy[i].height = ELITE_ENEMY_HEIGHT;
	}
	else {
		enemy[i].type = SMALL;
		enemy[i].hp = 1;
		enemy[i].width = SMALL_ENEMY_WIDTH;
		enemy[i].height = SMALL_ENEMY_HEIGHT;
	}
}
//init the datas of the game
void gameInit() {
	loadImg();
	player.x = WIDTH / 2;
	player.y = HEIGHT - 100;
	player.live = true;
	//init status of the bullet
	for (int i = 0; i < BULLET_NUM; i++) {
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].live = false;
	}
	//init status of the enemies
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].live = false;
		enemyHp(i);
	}
}
//draw the images of the objects
void gameDraw(){
	putimage(0, 0, &bk); 
	putimage(player.x, player.y,&game_role_2,NOTSRCERASE);
	putimage(player.x, player.y,&game_role_1,SRCINVERT);
	//draw the bullet 
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bullet[i].live) {
			putimage(bullet[i].x, bullet[i].y, &bullet_2, NOTSRCERASE);
			putimage(bullet[i].x, bullet[i].y, &bullet_1, SRCINVERT);
		}
	}
	//draw the enemies
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (enemy[i].live) {
			if (enemy[i].type == ELITE) {
				putimage(enemy[i].x, enemy[i].y, &elite_enemy_2, NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &elite_enemy_1, SRCINVERT);
			}
			else {
				putimage(enemy[i].x, enemy[i].y, &small_enemy_2, NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &small_enemy_1, SRCINVERT);
			}
		}
	}
}
//to create bullets
void createBullet() {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (!bullet[i].live) {
			bullet[i].x = player.x + planeWidth/2;
			bullet[i].y = player.y;
			bullet[i].live = true;
			break;
		}
	}
}
//to make bullet move regularly
void BulleMove(int bulletSpeed) {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bullet[i].live) {
			bullet[i].y -= bulletSpeed;
			if (bullet[i].y < 0) {
				bullet[i].live = false;
			}
		}
	}
}
//to move player and shot bullets
void playerMove(double speed) {
	if (GetAsyncKeyState(VK_UP)|| GetAsyncKeyState('W')) {
		if (player.y > 0) {
			player.y -= speed;
		}
	}
	if (GetAsyncKeyState(VK_DOWN)||GetAsyncKeyState('S')) {
		if (player.y + planeHeight/2 < HEIGHT) {
			player.y += speed;
		}
	}
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) {
		if (player.x + planeWidth/2> 0) {
			player.x -= speed;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT)|| GetAsyncKeyState('D')) {
		if (player.x + planeWidth/2 < WIDTH) {
			player.x += speed;
		}
	}
	static DWORD t1 = 0, t2 = 0;
	if (GetAsyncKeyState(VK_SPACE) && timer(150,1)) {
		createBullet();
	}
}
// to create enmeies
void createEnemy() {
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (!enemy[i].live) {
			enemy[i].live = true;
			enemy[i].x = rand() % (WIDTH - SMALL_ENEMY_WIDTH / 2);
			enemy[i].y = 0;
			enemyHp(i);
			break;
		}
	}
}
//to make enemies move
void enemyMove(double speed) {
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (enemy[i].live) {
			enemy[i].y += speed;
			if (enemy[i].y > HEIGHT) {
				enemy[i].live = false;
				gamestop = true;
			}
		}
	}
}
//to strike enemies and add the scores
void strike() {
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (!enemy[i].live) {
			continue;
		}
		for (int j = 0; j < BULLET_NUM; j++) {
			if (!bullet[j].live) {
				continue;
				}
			//Judge that the bullet strikes the enemy
			if (bullet[j].x > enemy[i].x && bullet[j].x<enemy[i].width+enemy[i].x
				&& bullet[j].y>enemy[i].y && bullet[j].y < enemy[i].y + enemy[i].height) {
				bullet[j].live = false;
				enemy[i].hp--;
			}
		}
		if (enemy[i].hp <= 0) {
			enemy[i].live = false;
			if (enemy[i].type == SMALL) {
				score++;
			}
			else {
				score += 3;
			}
			//printf("%d\n", score);
		}
	}
}
//to show scores on the windows
void showScore(int score) {
	TCHAR time_text[50];
	setbkmode(TRANSPARENT);
	_stprintf_s(time_text, _T("SCORE:%d"), score);
	settextcolor(WHITE);
	settextstyle(80, 0, _T("宋体"));
	outtextxy(WIDTH/5, HEIGHT/2, time_text);
}

int main() {
	initgraph(WIDTH, HEIGHT);
	gameInit();
	mciSendString("open ./game_plane_image/Do-You-Hear-The-People-Sing.mp3 alias bgm", 0, 0, 0);
	mciSendString("play bgm repeat", 0, 0, 0);
	//two way soft-closing drawing
	BeginBatchDraw();
	while (1) {
		showScore(score);
		gameDraw();
		FlushBatchDraw();
		playerMove(0.5);
		BulleMove(bulletSpeed);
		static DWORD t1, t2;
		if (timer(500,0)) {
			createEnemy();
		}
		if (timer(10, 2)) {
			enemyMove(3.75);
		}
		strike();
		showScore(score);
		if (gamestop) {
			break;
		}
	}
	EndBatchDraw();
	while (1) {

		showScore(score);
	}
	return 0;
}
