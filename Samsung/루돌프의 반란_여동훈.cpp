#include <iostream>
#include <math.h>
#include <queue>
using namespace std;

struct loc_info {
	int y, x;
};
struct queue_info {
	int y, x ,idx;
};
struct santa_info {
	int y, x;
	int score;
	int stuned;
	bool lived;
};
int N, M, P, C, D; // 게임판 크기, 턴수, 산타수, 루돌프 힘, 산타 힘
loc_info dolp;
santa_info santa[30];
int map_santa[50][50];
int dy[] = {-1,0,1,0,-1,-1,1,1}; // 상우하좌, 대각선
int dx[] = {0,1,0,-1,-1,1,-1,1};

void init() {
	cin >> N >> M >> P >> C >> D;
	int y, x;
	cin >> y >> x;
	dolp = { y - 1, x - 1 };
	for (int i = 0; i < P; i++) {
		int num, y, x;
		cin >> num >> y >> x;
		santa[num - 1] = { y - 1,x - 1, 0, false,true };
		map_santa[y - 1][x - 1] = num; // 맵 만들기
	}
}
int get_dist(loc_info first, loc_info second) {
	int dist = pow(first.y - second.y, 2) + pow(first.x - second.x, 2);
	return dist;
}
void chain_move(int dir, int idx) { // 연쇄효과로 움직임
	queue<queue_info> q;
	q.push({ santa[idx-1].y, santa[idx-1].x, idx-1 });

	// 산타 데이터 최신화, 맵 최신화
	while (!q.empty()) {
		queue_info now = q.front(); q.pop();
		int ny = now.y + dy[dir];
		int nx = now.x + dx[dir];
		santa[now.idx].y = ny; // 데이터 최신화
		santa[now.idx].x = nx;
		if (ny >= N || nx >= N || ny < 0 || nx < 0) { // 범위 초과시 사망
			santa[now.idx].lived = false;
			return;
		}
		if (map_santa[ny][nx] != 0) { // 다른 산타가 있다면 큐에 넣기
			q.push({ ny,nx,map_santa[ny][nx] -1});
		}
		map_santa[ny][nx] = now.idx + 1; // 맵 최신화
	}
}
void dolp_move() {
	// 타겟 산타 찾기
	int target_idx = 0;
	int minV = 1e9;
	for (int i = 0; i < P; i++) {
		if (santa[i].lived == false) continue;
		int dist = get_dist({ santa[i].y,santa[i].x }, dolp);
		if (minV > dist) {
			minV = dist;
			target_idx = i;
		}
		else if (minV == dist) { // 거리가 같다면 
			if (santa[target_idx].y < santa[i].y) { // r 좌표가 크다면
				target_idx = i;
			}
			else if (santa[target_idx].y == santa[i].y) {
				if (santa[target_idx].x < santa[i].x) { // c 좌표가 크다면
					target_idx = i;
				}
			}
		}
	}
	// 루돌프 이동
	minV = 1e9;
	int dir = 0;
	loc_info next_pos;
	for (int i = 0; i < 8; i++) {
		loc_info next_dolp = { dolp.y + dy[i], dolp.x + dx[i] };
		int dist = get_dist(next_dolp, { santa[target_idx].y ,santa[target_idx].x });
		if (next_dolp.y >= N || next_dolp.x >= N || next_dolp.y < 0 || next_dolp.x < 0) continue;
		if (minV > dist) {
			minV = dist;
			next_pos = next_dolp;
			dir = i;
		}
	}
	dolp = next_pos;
	// 산타와 충돌 발생 하면
	if (santa[target_idx].y == dolp.y && santa[target_idx].x == dolp.x) { // 충돌 발생
		santa[target_idx].score += C; // 점수 획득
		map_santa[santa[target_idx].y][santa[target_idx].x] = 0; // 해당 자리 비우기(맵 최신화)
		santa[target_idx].stuned = 2;

		int ny = santa[target_idx].y + C * dy[dir];
		int nx = santa[target_idx].x + C * dx[dir];
		santa[target_idx].y = ny; // 산타 데이터 최신화
		santa[target_idx].x = nx;
		if (ny >= N || nx >= N || ny < 0 || nx < 0) { // 밖으로 빠져나감
			santa[target_idx].lived = false;
		}
		else if (map_santa[ny][nx] != 0) { // 다른 산타와 만남 (상호작용 발생)
			chain_move(dir, map_santa[ny][nx]);
			map_santa[ny][nx] = target_idx + 1; // (맵 최신화)
		}
		else {
			map_santa[ny][nx] = target_idx + 1; // (맵 최신화)
		}
		
	}
}
void santa_move(int idx) {
	int now_dist = get_dist(dolp, { santa[idx].y, santa[idx].x });
	int dir = 0;
	int flag = false;
	for (int i = 0; i < 4; i++) {
		int ny = santa[idx].y + dy[i];
		int nx = santa[idx].x + dx[i];
		if (ny >= N || nx >= N || ny < 0 || nx < 0) continue;
		if (map_santa[ny][nx] != 0) continue;
		int dist = get_dist(dolp, { ny, nx });
		if (now_dist > dist) {
			now_dist = dist;
			dir = i;
			flag = true;
		}
	}
	if (flag == false) return;
	// 산타의 다음 방향
	int ny = santa[idx].y + dy[dir];
	int nx = santa[idx].x + dx[dir];
	if (ny == dolp.y && nx == dolp.x) { // 루돌프와 충돌
		santa[idx].score += D; // 점수 획득
		dir = (dir + 2) % 4; // 방향 교체
		map_santa[santa[idx].y][santa[idx].x] = 0; // 기존 산타가 있던 자리 초기화
		santa[idx].stuned = 2; // 스턴걸림
		ny = ny + D * dy[dir];
		nx = nx + D * dx[dir];
		santa[idx].y = ny; // 산타 데이터 최신화
		santa[idx].x = nx;
		if (ny >= N || nx >= N || ny < 0 || nx < 0) { // 밖으로 빠져나감
			santa[idx].lived = false;
		}
		else if (map_santa[ny][nx] != 0) { // 다른 산타와 만남 (상호작용 발생)
			chain_move(dir, map_santa[ny][nx]);
			map_santa[ny][nx] = idx + 1; // (맵 최신화)
		}
		else {
			map_santa[ny][nx] = idx + 1; // (맵 최신화)
		}
	}
	else { // 루돌프와 산타 둘다 없음(해당 방향으로 진행)
		map_santa[santa[idx].y][santa[idx].x] = 0;
		santa[idx].y += dy[dir];
		santa[idx].x += dx[dir];
		map_santa[santa[idx].y][santa[idx].x] = idx + 1;
	}
}

void live_bonus() {
	for (int i = 0; i < P; i++) {
		if (santa[i].lived == true) {
			santa[i].score++;
		}
	}
}
void stun_check() {
	for (int i = 0; i < P; i++) {
		if (santa[i].lived == false) continue;
		if (santa[i].stuned > 0) {
			santa[i].stuned--;
		}
	}
}
bool death_check() {
	for (int i = 0; i < P; i++) {
		if (santa[i].lived == true) {
			return false;
		}
	}
	return true;
}
void print_score() {
	for (int i = 0; i < P; i++) {
		cout << santa[i].score << " ";
	}
}
void debug_print(int map[50][50]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << map[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}


int main() {
	// 여기에 코드를 작성해주세요.
	init();
	for (int t = 0; t < M; t++) { // M번 움직임
		if (death_check()) break;

		// 루돌프 움직임
		dolp_move();

		//cout << "루돌프 이동 후"<< "\n";
		//debug_print(map_santa);
		//print_score();
		//cout << "\n";
		//cout << "(" << dolp.y << "," << dolp.x << ")" << "\n";
		// 
		// 산타 움직임
		for (int i = 0; i < P; i++) {
			if (santa[i].stuned > 0) continue;
			if (santa[i].lived == false) continue;
			santa_move(i);
		}
		
		//cout << "산타 이동 후" << "\n";
		//debug_print(map_santa);
		//print_score();
		//cout << "\n";

		live_bonus(); // 생존 보너스
		stun_check(); // 스턴 턴수 감소
	}
	print_score();

	return 0;
}
