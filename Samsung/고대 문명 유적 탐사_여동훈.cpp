#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
using namespace std;

int K, M; // 반복횟수, 조각갯수
struct loc {
	int r, c;
};
struct vc_info {
	int getto; // 유물 획득값
	int degree; // 각도
	loc location; // 중심 좌표값
	int map[5][5];
	int visited[5][5]; // 사라지는 칸
};
vector<vc_info> vc; // 최대 36개
int map[5][5];
queue<int> peice;
bool cmp(vc_info a, vc_info b) {
	if (a.getto > b.getto) {
		return true;
	}
	else if (a.getto == b.getto) {
		if (a.degree < b.degree) {
			return true;
		}
		else if (a.degree == b.degree) {
			if (a.location.c < b.location.c) {
				return true;
			}
			else if (a.location.c == b.location.c) {
				return a.location.r < b.location.r;
			}
		}
	}
	return false;
}

void init() {
	cin >> K >> M;

	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) {
			cin >> map[y][x];
		}
	}
	for (int i = 0; i < M; i++) {
		int n;
		cin >> n;
		peice.push(n);
	}
}
void rotate_map(int y, int x) {
	vc_info tmp;
	tmp.getto = 0;
	tmp.location = { y,x };
	memcpy(tmp.map, map, sizeof(map));
	int tmp_map[3][3] = { 0 };
	int rotate_map_90[3][3] = { 0 };
	int rotate_map_180[3][3] = { 0 };
	int rotate_map_270[3][3] = { 0 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			tmp_map[i][j] = map[y + i][x + j];
		}
	}
	for (int i = 0; i < 3; i++) { // 90도
		for (int j = 0; j < 3; j++) {
			rotate_map_90[j][2 - i] = tmp_map[i][j];
		}
	}
	for (int i = 0; i < 3; i++) { // 180도
		for (int j = 0; j < 3; j++) {
			rotate_map_180[j][2 - i] = rotate_map_90[i][j];
		}
	}
	for (int i = 0; i < 3; i++) { // 270도
		for (int j = 0; j < 3; j++) {
			rotate_map_270[j][2 - i] = rotate_map_180[i][j];
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			tmp.map[y + i][x + j] = rotate_map_90[i][j];
		}
	}
	tmp.degree = 1;
	vc.push_back(tmp); // 90 도 변경한 맵
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			tmp.map[y + i][x + j] = rotate_map_180[i][j];
		}
	}
	tmp.degree = 2;
	vc.push_back(tmp); // 180 도 변경한 맵
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			tmp.map[y + i][x + j] = rotate_map_270[i][j];
		}
	}
	tmp.degree = 3;
	vc.push_back(tmp); // 270 도 변경한 맵
}
bool find_getto(int idx) {
	int total_visited[5][5] = { 0 }; // 최종적으로 유물 생성되는곳
	int dy[] = { -1,0,1,0 };
	int dx[] = { 0,1,0,-1 };
	int result = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (total_visited[i][j] == 1) continue;
			queue<loc> q;
			int visited[5][5] = { 0 };
			int cnt = 1;
			int num = vc[idx].map[i][j]; // 현재 위치의 값
			q.push({ i,j });
			visited[i][j] = 1;
			while (!q.empty()) {
				loc n = q.front(); q.pop();

				for (int i = 0; i < 4; i++) {
					int ny = n.r + dy[i];
					int nx = n.c + dx[i];
					if (ny < 0 || ny >= 5 || nx < 0 || nx >= 5) continue; // 범위초과
					if (visited[ny][nx] == 1) continue;
					if (vc[idx].map[ny][nx] != num) continue;
					visited[ny][nx] = 1;
					cnt++;
					q.push({ ny,nx });
				}
			}
			if (cnt >= 3) {
				for (int i = 0; i < 5; i++) {
					for (int j = 0; j < 5; j++) {
						if (visited[i][j] == 1) {
							total_visited[i][j] = 1;
						}

					}
				}
				result += cnt;
			}
		}
	}
	if (result == 0) return false;
	vc[idx].getto = result;
	memcpy(vc[idx].visited, total_visited, sizeof(total_visited));
	return true;
}


void make_vc() {
	vc.clear();
	for (int y = 1; y <= 3; y++) {
		for (int x = 1; x <= 3; x++) {
			rotate_map(y - 1, x - 1); // map_vc를 만듬 90, 180, 270도로 회전한
		}
	}
	for (int i = 0; i < vc.size(); i++) { // 유물 획득값 조사
		find_getto(i);
	}
}
void remove_map() { // 공간 제거하기
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (vc[0].visited[i][j] == 1) {
				map[i][j] = 0;
			}
		}
	}
}
void fill_empty() { // 빈공간 채우기
	for (int x = 0; x < 5; x++) {
		for (int y = 4; y >= 0; y--) {
			if (map[y][x] == 0) {
				int now = peice.front(); peice.pop();
				map[y][x] = now;
			}
		}
	}
}
int main() {
	init();
	for (int t = 0; t < K; t++) {
		make_vc();
		// 정렬
		sort(vc.begin(), vc.end(), cmp);
		if (vc[0].getto == 0) break; // 얻는 값이 없으면 종료
		// 첫번째값을 맵에 적용.
		memcpy(map, vc[0].map, sizeof(map));
		// 맵 적용후 연쇄작용
		int result = vc[0].getto;
		while (1) {
			
			remove_map();
			fill_empty();
			memcpy(vc[0].map, map, sizeof(map));
			if (find_getto(0)) { // 찾기
				memcpy(map, vc[0].map, sizeof(map));
				result += vc[0].getto;
			}
			else break;
		}
		cout << result << " ";
		// >> 턴마다 유물 가치의 총합을 공백 사이를 두고 출력...

	}
}
