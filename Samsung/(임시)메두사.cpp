#include<iostream>
#include<vector>
#include<queue>
#include<cstring>
#include<algorithm>
using namespace std;

#define Y first
#define X second
typedef pair<int, int> loc;
const int dy_1[] = { 0,0,-1,1 }; // 좌우상하
const int dx_1[] = { -1,1,0,0 };
const int dy_2[] = { -1,1, 0,0, }; // 상하 좌우
const int dx_2[] = { 0,0 ,-1,1};
const int dr[4][3] = { {-1,-1,-1}, {1,1,1}, {0,-1,1}, {0,-1,1} }; // 상, 하, 좌, 우
const int dc[4][3] = { {0,-1,1}, {0,-1,1}, {-1,-1,-1}, {1,1,1} };
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
	Warrior.push_back({ 0,0,0 }); // dummy
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
	int visited[50][50] = { 0 };
	queue<loc> q;
	q.push(Park);
	visited[Park.Y][Park.X] = 1;
	while (!q.empty()) {
		loc now = q.front(); q.pop();
		for (int i = 0; i < 4; i++) {
			int ny = now.Y + dy_2[i];
			int nx = now.X + dx_2[i];
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
		int ny = Snake.Y + dy_1[i];
		int nx = Snake.X + dx_1[i];
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


int invisable[50][50] = { 0 };
int visited[50][50] = { 0 };
int sketch_Snake_Sight(int dir, int dir_idx, int (&tmp)[50][50]) {
	// 안쪽에서 바깥쪽으로 탐색해야함.
	memset(invisable, 0, sizeof(invisable));
	memset(visited, 0, sizeof(visited));
	int cnt = 0;
	int ny = Snake.Y + dr[dir][dir_idx];
	int nx = Snake.X + dc[dir][dir_idx];
	if (ny < 0 || nx < 0 || ny >= N || nx >= N) return 0;
	tmp[ny][nx] = 1;
	if (Warrior_Map[ny][nx].size() != 0) {
		return Warrior_Map[ny][nx].size();
	}
	queue<loc> q;
	q.push({ny,nx});

	while (!q.empty()) {
		loc now = q.front(); q.pop();

		for (int i = 0; i < 2; i++) {
			int now_dir = dir_idx;
			if (i == 0) now_dir = 0;
			loc next = { now.Y + dr[dir][now_dir], now.X + dc[dir][now_dir] };
			if (next.Y < 0 || next.X < 0 || next.Y >= N || next.X >= N) continue;
			if (visited[next.Y][next.X] == 1) continue;
			if (invisable[next.Y][next.X] == 1) continue;
			visited[next.Y][next.X] = 1;
			int Warrior_num = Warrior_Map[next.Y][next.X].size();
			if (Warrior_num == 0) {
				tmp[next.Y][next.X] = 1;
				q.push(next);
			}
			else {
				tmp[next.Y][next.X] = 1;
				cnt += Warrior_num;
				// invisable 맵 생성
				queue<loc> inv_q;
				inv_q.push({ next.Y,next.X });
				while (!inv_q.empty()) {
					loc now = inv_q.front(); inv_q.pop();
					for (int i = 0; i < 2; i++) {
						int now_dir = dir_idx;
						if (i == 0) now_dir = 0;
						loc next = { now.Y + dr[dir][now_dir], now.X + dc[dir][now_dir] };
						if (next.Y < 0 || next.X < 0 || next.Y >= N || next.X >= N) continue;
						if (invisable[next.Y][next.X] == 1) continue;
						invisable[next.Y][next.X] = 1;
						inv_q.push({ next.Y, next.X });
					}
				}
			}
		}
	}
	return cnt;

}
void make_Snake_Sight() {
	int max = 0;

	memset(Snake_Sight, 0, sizeof(Snake_Sight));
	for (int dir = 0; dir < 4; dir++) {
		int cnt = 0;
		int tmp_Snake_Sight[50][50] = {};
		// center
		queue<loc> center_q;
		center_q.push(Snake);
		while (!center_q.empty()) {
			loc now = center_q.front(); center_q.pop();
			loc next = { now.Y + dr[dir][0], now.X + dc[dir][0] };
			if (next.Y < 0 || next.X < 0 || next.Y >= N || next.X >= N) continue;
			int Warrior_num = Warrior_Map[next.Y][next.X].size();
			if (Warrior_num == 0) {
				tmp_Snake_Sight[next.Y][next.X] = 1; // 돌이 되는곳
				center_q.push(next);
			}
			else {
				tmp_Snake_Sight[next.Y][next.X] = 1; // 돌이 되는곳
				cnt += Warrior_num;
				break;
			}
		}
		cnt += sketch_Snake_Sight(dir, 1, tmp_Snake_Sight);
		cnt += sketch_Snake_Sight(dir, 2, tmp_Snake_Sight);

		if (cnt > max) {
			max = cnt;
			memcpy(Snake_Sight, tmp_Snake_Sight, sizeof(Snake_Sight));
		}
	}
	rock_warrior += max;
}
int getdist(loc a, loc b) {
	return abs(a.Y - b.Y) + abs(a.X - b.X);
}
void Warrior_move(int option) {
	for (int i = 1; i <= M; i++) {
		if (Warrior[i].died) continue;
		if (Snake_Sight[Warrior[i].y][Warrior[i].x] == 1) continue; // 돌이 된 기사
		int mindist = getdist({ Warrior[i].y ,Warrior[i].x }, Snake);
		if (mindist == 0) continue;
		bool flag = false;
		int nextY, nextX;
		for (int j = 0; j < 4; j++) {
			int ny, nx;
			if (option == 1) {
				ny = Warrior[i].y + dy_1[j];
				nx = Warrior[i].x + dx_1[j];
			}
			if (option == 2) {
				ny = Warrior[i].y + dy_2[j];
				nx = Warrior[i].x + dx_2[j];
			}
			if (ny < 0 || nx < 0 || ny >= N || nx >= N) continue;
			if (Snake_Sight[ny][nx] == 1) continue;
			int nowdist = getdist({ ny,nx }, Snake);
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
void dbg_print() {
	int print_map[50][50] = { 0 };
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (Warrior_Map[i][j].size() != 0) print_map[i][j] = 2;
		}
	}
	print_map[Snake.Y][Snake.X] = -1;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << print_map[i][j]<< " ";
		}
		cout << endl;
	}
	cout << endl;

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
		//dbg_print();
		make_Snake_Sight();
		// [3] 전사들의 이동
		// 메두사 시선 맵에 없는 전사들은 메두사와 가까워지는 방향으로 이동(좌우상하)
		dbg_print();
		Warrior_move(2);
		Warrior_move(1);
		// [4] 전사의 공격
		Warrior_Attack();
		cout << sum_dist << " " << rock_warrior << " " << attack_warrior << '\n';
	}




}
