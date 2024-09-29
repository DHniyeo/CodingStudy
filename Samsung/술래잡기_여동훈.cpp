// 실수한 부분 : 문제 제대로 안봄. 현재 칸 ~ 3칸, 155 줄 가야할 칸에 술래가 있는 지 체크하는 부분 오류.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef pair<int, int> loc;
// 이동방법  1 : 좌우, 2 : 상하 // 처음은 오른쪽, 처음은 아래쪽

struct info {
	int y, x;
	int dir;
	bool death;
};

int n, m, h, k; // 맵 크기 , 도망자 수 , 나무 수, 턴 수
int score;
vector<info> runner_info; // 0 : 술래  1 ~ : 도망자
vector<vector<vector<int>>> runner_map; // 인덱스 저장 맵
vector<vector<int>> tree_map;
vector<vector<int>> corner_map;
int in_out_corner = 1; // 1 : out, 3 : in

const int dy[] = { -1,0, 1,0 }; // up, right, down, left
const int dx[] = { 0,1,0,-1 };

void make_map() {
	// 벡터 기반으로 맵 만들기
	runner_map = vector<vector<vector<int>>>(n, vector<vector<int>>(n, vector<int>()));
	for (int i = 1; i <= m; i++) {
		if (runner_info[i].death) continue;
		runner_map[runner_info[i].y][runner_info[i].x].push_back(i);
	}
}
void debug_map() {
	// 술래의 이동용
	vector<vector<int>> tmp(n, vector<int>(n, 0));
	tmp[runner_info[0].y][runner_info[0].x] = -1;
	for (int i = 1; i <= m; i++) {
		if (runner_info[i].death) continue;
		tmp[runner_info[i].y][runner_info[i].x] += 1;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << tmp[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
int get_dist(loc a, loc b) {
	return abs(a.first - b.first) + abs(a.second - b.second);
}

void init() {
	cin >> n >> m >> h >> k;
	score = 0;
	runner_info = vector<info>();
	tree_map = vector<vector<int>>(n, vector<int>(n, 0));
	corner_map = vector<vector<int>>(n, vector<int>(n, 0));
	runner_map = vector<vector<vector<int>>>(n, vector<vector<int>>(n, vector<int>()));
	runner_info.push_back({ n / 2,n / 2,0 ,false });
	in_out_corner = 1;
	for (int i = 0; i < m; i++) {
		int y, x, dir;
		cin >> y >> x >> dir;
		y--; x--;
		runner_info.push_back({ y,x,dir,false }); // 방향 그대로 담아줘도 됨.(1 : 오른쪽, 2 : 아래)
	}
	for (int i = 0; i < h; i++) { // 트리 맵 만들기
		int y, x;
		cin >> y >> x;
		y--; x--;
		tree_map[y][x] = 1;
	}
	make_map(); // 벡터 기반 맵 만들기

	int nowy = n / 2;
	int nowx = n / 2;
	int cnt = 0;
	int dir = 0;
	int limit = 1;
	int limit_cnt = 0;
	while (1) {
		cnt++;
		if (nowy == 0 && nowx == 0) break;
		int ny = nowy + dy[dir];
		int nx = nowx + dx[dir];
		if (cnt == limit) { // 코너임
			dir = (dir + 1) % 4;
			limit_cnt++;
			cnt = 0;
			corner_map[ny][nx] = 1;
		}
		if (limit_cnt == 2) { // 리미트 한계 늘리기
			limit_cnt = 0;
			limit++;
		}
		nowy = ny;
		nowx = nx;
	}
	/*for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << corner_map[i][j] << " ";
		}
		cout << endl;
	}*/
}
void move() {
	int ny = runner_info[0].y + dy[runner_info[0].dir];
	int nx = runner_info[0].x + dx[runner_info[0].dir];

	// 방향 바로 제어
	if ((ny == n / 2 && nx == n / 2) || (ny == 0 && nx == 0)) { // 중앙 또는 0,0 이면 방향 조절
		runner_info[0].dir = (runner_info[0].dir + 2) % 4;
		if (in_out_corner == 1) in_out_corner = 3;
		else in_out_corner = 1;
	}
	if (corner_map[ny][nx] == 1) { // 코너
		runner_info[0].dir = (runner_info[0].dir + in_out_corner) % 4;
	}
	runner_info[0].y = ny;
	runner_info[0].x = nx;
}
void get_score(int turn) {
	for (int i = 0; i <= 2; i++) {
		int ny = runner_info[0].y + i * dy[runner_info[0].dir];
		int nx = runner_info[0].x + i * dx[runner_info[0].dir];
		if (ny < 0 || nx < 0 || ny >= n || nx >= n) continue;
		if (tree_map[ny][nx] == 1) continue;
		if (runner_map[ny][nx].size() == 0) continue;
		int human = runner_map[ny][nx].size(); // 사람의 수
		score += (turn * human);
		for (int i = 0; i < human; i++) {
			int idx = runner_map[ny][nx][i];
			runner_info[idx].death = true;
		}
	}
}
int main() {
	init();
	//debug_map();
	for (int t = 0; t < k; t++) {
		// 도망자 움직이기
		for (int idx = 1; idx <= m; idx++) {
			if (runner_info[idx].death) continue;
			int dist = get_dist({ runner_info[0].y, runner_info[0].x }, { runner_info[idx].y, runner_info[idx].x }); // 술래와의 거리
			if (dist <= 3) { // 도망자가 움직임
				int ny = runner_info[idx].y + dy[runner_info[idx].dir];
				int nx = runner_info[idx].x + dx[runner_info[idx].dir];
				if (ny < 0 || nx < 0 || ny >= n || nx >= n) { // 범위 초과
					runner_info[idx].dir = (runner_info[idx].dir + 2) % 4;
					ny = runner_info[idx].y + dy[runner_info[idx].dir];
					nx = runner_info[idx].x + dx[runner_info[idx].dir];
				}
				if (runner_info[0].y == ny && runner_info[0].x == nx) continue;
				runner_info[idx].y = ny;
				runner_info[idx].x = nx;
			}
		}
		make_map(); // 맵 최신화
		//debug_map();

		// 술래잡기
		move();
		get_score(t + 1);
		make_map(); // 맵 최신화
		//debug_map();
	}
	cout << score;
}
