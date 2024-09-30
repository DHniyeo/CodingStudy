#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>

using namespace std;
int n;
struct vc_info {
	int now_num; // 현재 칸의 번호
	int cnt; // 칸의 수
	map<int,int> wall; // 맞닿아 있는 변
};
vector<vector<int>> picture_map;
vector<vector<int>> visited;
int result = 0;

void init() {
	cin >> n;
	picture_map = vector<vector<int>>(n, vector<int>(n,0));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> picture_map[i][j];
		}
	}
}
void block_4_rotate(vector<vector<int>>& rotate_map, int sy, int sx) {
	int size = n / 2;
	vector<vector<int>> block_map(size, vector<int>(size, 0));
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			block_map[j][n/2-1-i] = picture_map[sy + i][sx + j];
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			rotate_map[sy + i][sx + j] = block_map[i][j];
		}
	}
}
vc_info bfs(int y ,int x) {
	vc_info now_vc = { picture_map[y][x], 0, map<int,int>()};
	const int dy[] = { -1,0,1,0 };
	const int dx[] = { 0,-1,0,1 };
	queue<pair<int, int>> q;
	q.push({ y,x });
	visited[y][x] = 1;
	int cnt = 1;
	while (!q.empty()) {
		pair<int, int> now = q.front(); q.pop();
		for (int i = 0; i < 4; i++) {
			int ny = now.first + dy[i];
			int nx = now.second + dx[i];
			if (ny < 0 || nx < 0 || ny >= n || nx >= n) continue;
			if (visited[ny][nx] == 1) continue;
			if (picture_map[y][x] != picture_map[ny][nx]) {
				now_vc.wall[picture_map[ny][nx]]++;
				continue;
			}
			visited[ny][nx] = 1;
			q.push({ ny,nx });
			cnt++;
		}
	}
	now_vc.cnt = cnt;
	return now_vc;
}
void make_vc_find_score() {
	int num = 1;
	visited = vector<vector<int>>(n, vector<int>(n, 0));
	vector<vc_info> vc;
	vc.push_back({ 0, 0,map<int,int>()}); // dummy

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (visited[i][j] == 1) continue;
			visited[i][j] = 1;
			vc_info now_vc = bfs(i, j); 
			// 현재 칸의 번호
			// 칸에 속한 칸의 수
			// 칸의 번호와 맞닿아 있는 변의 수
			vc.push_back(now_vc);
			num++;
		}
	}
	// score 계산
	vector<vector<int>>combi(num,vector<int>(num,0));
	// 그룹 a에 속한 칸의 수 + 그룹 b에 속한 칸의 수 ) x 그룹 a를 이루고 있는 숫자 값 x 그룹 b를 이루고 있는 숫자 값 x 그룹 a와 그룹 b가 서로 맞닿아 있는 변의 수
	for (int idx = 1; idx < num; idx++) {
		for (auto itr = vc[idx].wall.begin(); itr != vc[idx].wall.end(); itr++) {
			int key = itr->first;
			int val = itr->second;
			if (combi[idx][key] == 1) continue;
			combi[idx][key] = 1;
			result += ((vc[idx].cnt + vc[key].cnt) * vc[idx].now_num * vc[key].now_num * val);
		}
	}

}
void rotate() {
	// 십자가 부분은 반시계, 나머지 부분은 시계
	vector<vector<int>> rotate_map(n, vector<int>(n, 0));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i == n / 2 || j == n / 2) {
				rotate_map[n-1-j][i] = picture_map[i][j];
			}
		}
	}
	block_4_rotate(rotate_map,0,0);
	block_4_rotate(rotate_map,0,n/2+1);
	block_4_rotate(rotate_map,n/2+1,0);
	block_4_rotate(rotate_map,n/2+1, n/2+1);

	picture_map = rotate_map;
}
void dbg_print() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << picture_map[i][j] << " ";
		}	
		cout << endl;
	}
	cout << endl;
}
int main() {
	init();
	make_vc_find_score();
	for (int t = 0; t < 3; t++) {
		rotate();
		make_vc_find_score();
	}
	cout << result;
}
