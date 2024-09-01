#include <iostream>
#include <string.h>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

struct knight_info {
	int r, c;
	int h, w;
	int k; // 기사의 체력  
};
struct order_info {
	int i, d; // index, 위,오,아,왼
};
struct queue_info {
	int y, x, idx;
};

int L, N, Q; // 맵의 크기, 기사의 수, 명령의 수
int map[40][40] = {0,}; // 0 : 빈칸, 1 : 함정, 2 : 벽
int knight_map[40][40] = { 0, }; // 나이트의 위치(기사 위치 찾기 위함)

vector<int> to_move;
knight_info knight_start[30]; // 초기의 기사들의 정보(백업)
knight_info knight[30];
order_info order[100];

const int dy[] = {-1,0,1,0};
const int dx[] = {0,1,0,-1};

void make_knight_map() {
	memset(knight_map, 0, sizeof(knight_map));
	for (int i = 0; i < N; i++) {
		if (knight[i].k <= 0) continue; // 죽은 기사는 표시 x
		for (int y = 0; y < knight[i].h; y++) {
			for (int x = 0; x < knight[i].w; x++) {
				int ny = knight[i].r + y;
				int nx = knight[i].c + x;
				if (ny >= L || nx >= L || ny < 0 || nx < 0) continue;
				knight_map[knight[i].r + y][knight[i].c + x] = i + 1;
			}
		}
	}
}
void init() {
	cin >> L >> N >> Q;
	// 맵 정보 입력
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			cin >> map[i][j];
		}
	}
	// 기사 정보 입력
	for (int i = 0; i < N; i++) {
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		knight[i] = { r-1,c-1,h,w,k };
	}
	memcpy(knight_start, knight, sizeof(knight)); // 백업

	// 명령 입력
	for (int i = 0; i < Q; i++) {
		int idx, d;
		cin >> idx >> d;
		order[i] = { idx, d };
	}
	// 나이트 맵 만들기
	make_knight_map();
}
bool knight_move(int idx, int dir) {  // 기사의 이동
	int tmp_knight_map[40][40] = {0,};
	to_move.clear();

	to_move.push_back(idx);
	queue<queue_info> q; // 위치, 인덱스
	for (int y = 0; y < knight[idx - 1].h; y++) {
		for (int x = 0; x < knight[idx - 1].w; x++) {
			int ny = knight[idx - 1].r + y;
			int nx = knight[idx - 1].c + x;
			q.push({ ny,nx,idx });
		}
	}

	while (!q.empty()) {
		queue_info now = q.front(); q.pop();
		int ny = now.y + dy[dir];
		int nx = now.x + dx[dir];
		if (ny >= L || nx >= L || ny < 0 || nx < 0 || map[ny][nx] == 2) { // 벽을 발견하면 이동안함.
			return false;
		}
		tmp_knight_map[ny][nx] = now.idx; // 이동한 위치 반영
		if (knight_map[ny][nx] > 0 && knight_map[ny][nx] != now.idx) { // 밀려나는 기사 포착
			int next_knight_idx = knight_map[ny][nx];
			if (find(to_move.begin(), to_move.end(), next_knight_idx) == to_move.end()) {
				to_move.push_back(next_knight_idx); // 밀리는 기사
			}
			for (int y = 0; y < knight[next_knight_idx - 1].h; y++) { // 해당하는 인덱스 q에 추가
				for (int x = 0; x < knight[next_knight_idx - 1].w; x++) {
					int nexty = knight[next_knight_idx - 1].r + y;
					int nextx = knight[next_knight_idx - 1].c + x;
					if (nexty >= L || nextx >= L || nexty < 0 || nextx < 0) { // 벽을 발견하면 이동안함
						return false;
					}
					q.push({ nexty,nextx,next_knight_idx});
				}
			}
		}
	}
	memcpy(knight_map, tmp_knight_map, sizeof(knight_map)); // 기사 맵 최신화(이동 된 기사들만)
	return true;
}

void cal_damage(int start_idx, int dir) {
	
	for (int idx : to_move) { // 위치 최신화
		knight[idx - 1].r += dy[dir];
		knight[idx - 1].c += dx[dir];
	}
	// 해당 칸에 함정있다면 -1
	for (int idx : to_move) {
		if (start_idx == idx) continue; // 공격한 기사는 함정 상관 x
		for (int y = 0; y < knight[idx - 1].h; y++) { // 해당하는 인덱스 q에 추가
			for (int x = 0; x < knight[idx - 1].w; x++) {
				int nexty = knight[idx - 1].r + y;
				int nextx = knight[idx - 1].c + x;
				if (map[nexty][nextx] == 1) { // 해당칸이 함정이라면
					knight[idx - 1].k--;
				}
			}
		}
	}
}

int get_result() { // 최종으로 남은 기사 체력 구하기
	int sum = 0;
	for (int i = 0; i < N; i++) {
		if (knight[i].k <= 0) continue; // 이미 죽은 기사는 스킵
		sum += knight_start[i].k - knight[i].k;
	}
	return sum;
}

// debug
void print_map(int arr[40][40]) {
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < L; j++) {
			cout << arr[i][j] << " ";
		}
		cout << "\n";
	}
}

int main() {
	init();

	//print_map(knight_map);
	for (int i = 0; i < Q; i++) {
		// 기사 이동
		int knight_idx = order[i].i;
		int knight_dir = order[i].d;
		if (knight[knight_idx - 1].k <= 0) continue; // 이미 죽은 기사라면 스킵
		int flag = knight_move(knight_idx, knight_dir); // 맵으로 기사 이동(밀린 기사 맵 최신화)
		if (flag) { // 벽을 안 만났다면
			// 데미지 반영 및 데이터 최신화
			cal_damage(knight_idx, knight_dir);
			make_knight_map(); // 기사 맵 전체 최신화(데이터 기반 최신화)
		}
		//cout << i << "번째"<< endl;
		//print_map(knight_map);
	}
	cout << get_result() << endl;

	return 0;
}
