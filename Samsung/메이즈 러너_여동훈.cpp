#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int N, M, K;
vector<vector<int>> map;

struct loc {
	int y, x;
};
struct vec_info {
	loc l;
	bool flag; // 탈출 성공 여부
};
loc exit_loc;
vector<vec_info> vc;
int move_dist_sum;
const int dy[] = { -1,1,0,0 }; // 상하 좌우
const int dx[] = { 0,0,-1,1 };

int getdist(loc a, loc b) {
	return abs(a.y - b.y) + abs(a.x - b.x);
}
void init() {
	cin >> N >> M >> K;
	map = vector<vector<int>>(N, vector<int>(N, 0));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}
	for (int i = 0; i < M; i++) {
		int y, x;
		cin >> y >> x;
		vc.push_back({ y - 1,x - 1,false });
	}
	cin >> exit_loc.y >> exit_loc.x;
	exit_loc.y--; exit_loc.x--;
	move_dist_sum = 0;
}
loc find_squre(int minL, vector<int> (*Human)[10], vector<int>& idxs) {
	// 사각형 찾기
	for (int i = 0; i < N - minL + 1; i++) {
		for (int j = 0; j < N - minL + 1; j++) {
			// 현재의 사각형이 출입구와
			int flag1 = false;
			int flag2 = false;
			idxs = vector<int>();
			for (int y = 0; y < minL; y++) {
				for (int x = 0; x < minL; x++) {
					int human_num = Human[i + y][j + x].size();
					if (human_num != 0) { // 사람 존재
						for (int z = 0; z < human_num; z++) {
							idxs.push_back(Human[i + y][j + x][z]);
						}
						flag1 = true;
					}
					if (i + y == exit_loc.y && j + x == exit_loc.x) { // 출구 존재
						flag2 = true;
					}
				}
			}
			if (flag1 && flag2) {
				return { i,j };
			}
		}
	}
}
void rotate_map(loc start_p, int length, vector<int> idxs) {
	// 맵 변환
	vector<vector<int>> rotated_map(length, vector<int>(length, 0));
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			rotated_map[j][length - 1 - i] = map[i + start_p.y][j + start_p.x];
		}
	}
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			if (rotated_map[i][j] > 0) rotated_map[i][j]--; // 회전시 내구도 감소
			map[i + start_p.y][j + start_p.x] = rotated_map[i][j];
		}
	}
	// (i,j) => (j,length - 1 - i) + start_p
	for (int i = 0; i < idxs.size(); i++) {
		int idx = idxs[i];
		int ii = vc[idx].l.y - start_p.y;
		int jj = vc[idx].l.x - start_p.x;
		int nexty = jj + start_p.y;
		int nextx = length - 1 - ii + start_p.x;
		//int dist = getdist({nexty,nextx}, {vc[idx].l.y, vc[idx].l.x});
		//move_dist_sum += dist;
		vc[idx].l = { nexty, nextx };
	}
	// 탈출구 위치 이동
	int ii = exit_loc.y - start_p.y;
	int jj = exit_loc.x - start_p.x;
	int nexty = jj + start_p.y;
	int nextx = length - 1 - ii + start_p.x;
	exit_loc = { nexty, nextx };
}
bool check() {
	for (int i = 0; i < M; i++) {
		if (vc[i].flag == false) return false;
	}
	return true;
}
void debug_print() {
	vector<vector<int>> Human(N,vector<int>(N,0));
	for (int i = 0; i < M; i++) {
		if (vc[i].flag == true) continue;
		Human[vc[i].l.y][vc[i].l.x] = 1; // 중복 노상관
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << Human[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;

}
int main() {
	init();
	for (int t = 1; t <= K; t++) {
		// 참가자 이동

		for (int i = 0; i < M; i++) {
			if (vc[i].flag) continue;
			int dist = getdist(vc[i].l, exit_loc);
			for (int j = 0; j < 4; j++) {
				loc next = { vc[i].l.y + dy[j], vc[i].l.x + dx[j] };
				if (next.y >= N || next.x >= N || next.y < 0 || next.x < 0) continue;
				if (map[next.y][next.x] != 0) continue;
				int now_dist = getdist(next, exit_loc);
				if (dist > now_dist) { // 더 짧은거리 찾음
					vc[i].l = next;
					move_dist_sum++;
					break;
				}
			}
			if (vc[i].l.y == exit_loc.y && vc[i].l.x == exit_loc.x) {
				vc[i].flag = true;
			}
		}

		if (check()) break;
		//debug_print();
		// 사각형 길이 찾기
		int minL = 1e9;
		for (int i = 0; i < M; i++) {
			if (vc[i].flag) continue;
			int ydist = abs(vc[i].l.y - exit_loc.y);
			int xdist = abs(vc[i].l.x - exit_loc.x);
			int maxL = max(ydist, xdist);
			if (minL > maxL) {
				minL = maxL;
			}
		}
		minL++;
		// 사람 맵 만들기
		vector<int> Human[10][10];
		vector<int> idxs; // 회전 해야하는 인덱스
		for (int i = 0; i < M; i++) {
			if (vc[i].flag) continue;
			Human[vc[i].l.y][vc[i].l.x].push_back(i);
		}
		loc start_p = find_squre(minL, Human, idxs);
		// 회전
		rotate_map(start_p, minL ,idxs);

		//debug_print();
	}
	cout << move_dist_sum << endl;
	cout << exit_loc.y + 1 << " " << exit_loc.x + 1 << endl;

}
