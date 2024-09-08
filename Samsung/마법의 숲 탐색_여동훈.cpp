// 주의 해야 했던점 : 데이터를 모두 변경 후 맵에 적용, 새로운 골렘이 추가 되면 길이 열릴 수 도 있기 때문에 탐색을 해야함.

#include<iostream>
#include<queue>
#include<string.h>
using namespace std;

int dy[] = { -1,0,1,0 };
int dx[] = { 0, 1, 0 ,-1 };

// 0, 1, 2, 3 (북 동 남 서) 시계방향
int R, C, K;
int map[73][70]; // 3칸은 입구에서의 움직임
struct loc {
	int r, c;
};
struct info {
	loc center;
	loc exit[4]; // 북 동 남 서
	int d; // 출구의 방향 (0,1,2,3)
	int maxV;
};
info blocks[1000]; // 요정 데이터

void init() {
	cin >> R >> C >> K;
	for (int i = 0; i < K; i++) {
		int c, d;
		cin >> c >> d; // 열, 출구 방향 정보
		blocks[i].center = { 1,c - 1 };
		for (int j = 0; j < 4; j++) {  // 구체적인 북동남서 지점
			int ny = blocks[i].center.r + dy[j];
			int nx = blocks[i].center.c + dx[j];
			blocks[i].exit[j] = { ny,nx };
		}
		blocks[i].d = d;
	}
}
bool move_block(int idx, int dir) {
	for (int i = 0; i < 4; i++) { // 겉에만 잘가도 성공임
		int ny = blocks[idx].exit[i].r + dy[dir];
		int nx = blocks[idx].exit[i].c + dx[dir];
		if (ny >= R + 3 || nx < 0 || nx >= C) {
			return false;
		}
		if (map[ny][nx] != 0 && map[ny][nx] != idx + 1) { // 해당 칸에 이미 있음
			return false;
		}
		if (dir == 1 || dir == 3) { // 아래로 내려갈 수 있는지 확인
			ny += dy[2];
			nx += dx[2];
			if (ny >= R + 3 || nx < 0 || nx >= C) {
				return false;
			}
			if (map[ny][nx] != 0 && map[ny][nx] != idx + 1) { // 해당 칸에 이미 있음
				return false;
			}
		}
	}



	for (int i = 0; i < 4; i++) {
		map[blocks[idx].exit[i].r][blocks[idx].exit[i].c] = 0; // 현재 위치 초기화
	}
	map[blocks[idx].center.r][blocks[idx].center.c] = 0;

	for (int i = 0; i < 4; i++) {
		blocks[idx].exit[i].r += dy[dir]; // 반영
		blocks[idx].exit[i].c += dx[dir];
		if (dir == 1 || dir == 3) {
			blocks[idx].exit[i].r += dy[2]; // 반영
			blocks[idx].exit[i].c += dx[2];
		}
	}
	blocks[idx].center.r += dy[dir];
	blocks[idx].center.c += dx[dir];
	if (dir == 1 || dir == 3) {
		blocks[idx].center.r += dy[2]; // 반영
		blocks[idx].center.c += dx[2];
	}

	for (int i = 0; i < 4; i++) {
		map[blocks[idx].exit[i].r][blocks[idx].exit[i].c] = idx + 1;
	}
	map[blocks[idx].center.r][blocks[idx].center.c] = idx + 1;

	if (dir == 3) { // 서쪽
		blocks[idx].d = (blocks[idx].d + 3) % 4;
	}
	else if (dir == 1) { // 동쪽
		blocks[idx].d = (blocks[idx].d + 1) % 4;
	}
	return true;
}
void infinite_move(int idx) {
	//무한 이동
	// 갈수 없다면 서쪽으로 이동((dir+3)%4)
	// 갈수 없다면 동쪽으로 이동((dir+1)%4)
	// 서쪽 동쪽 아래 이동 불가능 하면 종료.
	while (1) {
		if (move_block(idx, 2)) { // 내려가기 성공
			continue;
		}
		if (blocks[idx].exit[2].r == R + 2) break; // 바닥 도달
		else if (move_block(idx, 3)) { // 왼쪽으로 가기 성공
			continue;
		}
		else if (move_block(idx, 1)) { // 오른쪽으로 가기 성공
			continue;
		}
		else break; // 내려가기 왼쪽가기 오른쪽가기 다 실패
	}

}
bool check() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < C; j++) {
			if (map[i][j] != 0) return true;
		}
	}
	return false;
}
int get_maxR(int idx) {
	queue<int> q;
	q.push(idx);
	// 새로 추가되어서 길이 만들어졌다면 들어가봐야함.
	int nowMaxR = blocks[idx].exit[2].r; // 남쪽
	int visited[1000] = { 0 };
	visited[idx] = 1;
	while (!q.empty()) {
		int now = q.front(); q.pop();
		for (int i = 0; i < 4; i++) {
			int ny = blocks[now].exit[blocks[now].d].r + dy[i]; // 출구방향에서 4가지 방향으로 찾기
			int nx = blocks[now].exit[blocks[now].d].c + dx[i];
			if (ny >= R + 3 || nx < 0 || nx >= C) continue;
			if (map[ny][nx] == 0 || map[ny][nx] == now + 1) continue;
			if (visited[map[ny][nx] - 1]) continue;
			visited[map[ny][nx] - 1] = 1;
			nowMaxR = nowMaxR < blocks[map[ny][nx] - 1].maxV ? blocks[map[ny][nx] - 1].maxV : nowMaxR; // 최댓값 갱신
			q.push(map[ny][nx] - 1);
		}
	}
	blocks[idx].maxV = nowMaxR; // 최신화
	return nowMaxR;
}
int main() {
	init();
	int result = 0;
	for (int tc = 0; tc < K; tc++) { // 정령의 수

		info backup = blocks[tc];
		infinite_move(tc);
		// 만약 0~2 행에 값이 존재한다면 맵을 비우고 작업함.
		if (check()) {
			memset(map, 0, sizeof(map));
			blocks[tc] = backup; // 데이터 초기화
			continue;
		}
		// 아니면 요정이 최대로 내려갈수 있는 값 계산
			// (출구 근처에 4방향으로 검사했을때 다른 번호가 있다면 해당 번호의 최대 R 값과 현재 인덱스의 최대 R값을 비교하여 가장 큰 R값을 저장
			// 결과값 += 현재 R값
		int tmp = get_maxR(tc) - 2;
		result += tmp;
	}

	cout<< result;
}
