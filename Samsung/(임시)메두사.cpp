#include<iostream>
#include<vector>
#include<queue>
#include<cstring>
#include<algorithm>
using namespace std;

#define Y first
#define X second
typedef pair<int, int> loc;
const int dy[] = { 0,0,-1,1 }; // 좌우상하
const int dx[] = { -1,1,0,0};
struct info {
	int y, x;
	bool died;
};
int N, M;
int Field[50][50];
int Snake_Sight[50][50];
vector<int> Warrior_Map[50][50];
vector<info> Warrior;
loc Snake;
loc Park;
int sum_dist, rock_warrior, attack_warrior; // 전사가 이동한 거리의 합, 메두사로 인해 돌이 된 전사의 수, 메두사를 공격한 전사의 수
void init() {
	cin >> N >> M;
	cin >> Snake.Y >> Snake.X;
	cin >> Park.Y >> Park.X;
	Warrior.push_back({0,0,0}); // dummy
	for (int i = 1; i <= M; i++) {
		int y, x;
		cin >> y >> x;
		Warrior.push_back({ y,x,false });
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> Field[i][j];
		}
	}
}
void make_Warrior_Map() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			Warrior_Map[i][j].clear();
		}
	}
	for (int i = 1; i <= M; i++) {
		if (Warrior[i].died) continue;
		Warrior_Map[Warrior[i].y][Warrior[i].x].push_back(i);
	}
}
void Snake_move() {
	int visited[50][50] = {0};
	queue<loc> q;
	q.push(Park);
	visited[Park.Y][Park.X] = 1;
	while (!q.empty()) {
		loc now = q.front(); q.pop();
		for (int i = 0; i < 4; i++) {
			int ny = now.Y + dy[i];
			int nx = now.X + dx[i];
			if (ny < 0 || nx < 0 || ny >= N || nx >= N) continue;
			if (visited[ny][nx] > 0) continue;
			if (Field[ny][nx] == 1) continue;
			visited[ny][nx] = visited[now.Y][now.X] + 1;
			q.push({ ny,nx });
		}
	}
	int minV = 1e9;
	int nextY, nextX;
	for (int i = 0; i < 4; i++) {
		int ny = Snake.Y + dy[i];
		int nx = Snake.X + dx[i];
		if (ny < 0 || nx < 0 || ny >= N || nx >= N) continue;
		if (Field[ny][nx] == 1) continue;
		if (minV > visited[ny][nx]) {
			minV = visited[ny][nx];
			nextY = ny;
			nextX = nx;
		}
	}
	Snake.Y = nextY;
	Snake.X = nextX;

	int Warrior_num = Warrior_Map[nextY][nextX].size();
	if (Warrior_num != 0) {
		for (int i = 0; i < Warrior_num; i++) {
			int nidx = Warrior_Map[nextY][nextX][i];
			Warrior[nidx].died = true; // 사망처리
		}
	}
}
void sketch() {


}
void make_Snake_Sight() {
	int max = 0;
	const int dy[] = { -1,-1,0,1,1,1,0,-1}; 
	const int dx[] = { 0,1,1,1,0,-1,-1,-1 };
	int tmp[50][50] = { 0 };
	for (int dir = 0; dir < 8; dir+=2) { // 아래, 오른, 위, 왼


	}

}
void Warrior_move() {
	for (int i = 1; i <= M; i++) {
		if (Warrior[i].died) continue;
		if (Snake_Sight[Warrior[i].y][Warrior[i].x] == 1) continue; // 돌이 된 기사
		int mindist = getdist({ Warrior[i].y ,Warrior[i].x}, Snake);
		if (mindist == 0) continue;
		bool flag = false;
		int nextY, nextX;
		for (int j = 0; j < 4; j++) {
			int ny = Warrior[i].y + dy[j];
			int nx = Warrior[i].x + dx[j];
			if (ny < 0 || nx < 0 || ny >= N || nx >= N) continue;
			if (Snake_Sight[ny][nx] == 1) continue;
			int nowdist = getdist({ny,nx}, Snake);
			if (mindist > nowdist) { 
				flag = true;
				nextY = ny;
				nextX = nx;
				break;
			}
		}
		if (flag) { // 더 가까워지는 방향이 나타난다면
			Warrior[i].y = nextY;
			Warrior[i].x = nextX;
			sum_dist++;
		}
	}
}
void Warrior_Attack() {
	for (int i = 1; i <= M; i++) {
		if (Warrior[i].died) continue;
		if (Warrior[i].y == Snake.Y && Warrior[i].x == Snake.X) {
			Warrior[i].died = true;
			attack_warrior++;
		}
	}
}
int getdist(loc a,loc b) {
	return abs(a.Y - b.Y) + abs(a.X - b.X);
}
int main() {
	init();
	while (1) {
		sum_dist = 0;
		rock_warrior = 0;
		attack_warrior = 0;
		make_Warrior_Map(); // 전사 맵
		// [1] 메두사의 이동
		Snake_move();
		if (Park == Snake) {
			cout << 0 << '\n';
			break;
		}
		// [2] 메두사의 시선
		memset(Snake_Sight, 0, sizeof(Snake_Sight));
		make_Snake_Sight();
		// [3] 전사들의 이동
		// 메두사 시선 맵에 없는 전사들은 메두사와 가까워지는 방향으로 이동(좌우상하)
		Warrior_move();
		Warrior_move();
		// [4] 전사의 공격
		Warrior_Attack();
		cout << sum_dist << " "<< rock_warrior << " " << attack_warrior << '\n';
	}


	

}
