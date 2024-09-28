// n이 매우 작아서 매번 맵을 만들어도 됨
#include<iostream>
#include<vector>
#include<queue>
using namespace std;

int N, M;
struct info {
	int target_y, target_x;
	int now_y, now_x;
	bool finished;
	bool start;
};
vector<vector<int>> base_camp_map; // 베이스캠프 맵
vector<vector<int>> wall_map; // 지나가지 못하는 벽
vector<vector<int>> bfs_result; // 경로 저장
vector<info> people; // 사람 데이터
int result_t = 0;
const int dy[] = {-1,0,0,1}; // 위왼오아래
const int dx[] = {0,-1,1,0};
void init() {
	cin >> N >> M;
	base_camp_map = vector<vector<int>>(N, vector<int>(N, 0));
	wall_map = vector<vector<int>>(N, vector<int>(N, 0));
	bfs_result = vector<vector<int>>(N, vector<int>(N, 0));
	result_t = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
		{
			cin >> base_camp_map[i][j];
		}
	}
	for (int i = 0; i < M; i++) {
		int y, x;
		cin >> y >> x;
		y--; x--;
		people.push_back({ y,x, -1, -1, false ,false });
	}
}
void bfs(int y, int x) { // n^2 시간복잡도
	bfs_result = vector<vector<int>>(N, vector<int>(N, 0));
	queue<pair<int, int>> q;
	q.push({ y,x });
	bfs_result[y][x] = 1;
	while (!q.empty()) {
		pair<int, int> now = q.front(); q.pop();
		for (int i = 0; i < 4; i++) {
			int ny = now.first + dy[i];
			int nx = now.second + dx[i];
			if (ny >= N || nx >= N || ny < 0 || nx < 0) continue;
			if (wall_map[ny][nx] == 1) continue;
			if (bfs_result[ny][nx] > 0) continue;
			bfs_result[ny][nx] = bfs_result[now.first][now.second] + 1;
			q.push({ ny,nx });
		}
	}
}

bool check() {
	for (int i = 0; i < M; i++) {
		if (people[i].finished == false) return false;
	}
	return true;
}
void dbg_print(){
	vector<vector<int>> dbg_map = vector<vector<int>>(N, vector<int>(N, 0));; // 경로 저장
	for (int i = 0; i < M; i++) {
		if (people[i].start == false) {
			if (people[i].finished == true) {
				dbg_map[people[i].now_y][people[i].now_x] = 2;
			}
			continue;
		}
		dbg_map[people[i].now_y][people[i].now_x] = 1;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << dbg_map[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
int main() {
	init();
	while (1) {
		result_t++;
		// 사람들이 움직임
		for (int i = 0; i < M; i++) {
			if (people[i].start == false) continue;
			// 편의점 위치에서 bfs로 맵을 채움
			bfs(people[i].target_y, people[i].target_x); 
			int min = 1e9;
			int next_y, next_x;
			for (int j = 0; j < 4; j++) {
				int ny = people[i].now_y + dy[j];
				int nx = people[i].now_x + dx[j];
				if (ny >= N || nx >= N || ny < 0 || nx < 0) continue;
				if (wall_map[ny][nx] == 1) continue;
				if (bfs_result[ny][nx] == 0)continue;
				if (min > bfs_result[ny][nx]) { // 거리가 가까워지는 방향으로 이동
					min = bfs_result[ny][nx];
					next_y = ny;
					next_x = nx;
				}
			}
			people[i].now_y = next_y;
			people[i].now_x = next_x;
		}

		// 편의점 도달 한사람이 있다면 해당칸 벽세우기
		for (int i = 0; i < M; i++) {
			if (people[i].now_y == people[i].target_y && people[i].now_x == people[i].target_x) {
				people[i].start = false;
				people[i].finished = true;
				wall_map[people[i].now_y][people[i].now_x] = 1;
			}
		}
		//dbg_print();
		// 베이스 캠프로 이동
		if (result_t <= M) {
			// 편의점에서 베이스캠프 찾기
			bfs(people[result_t-1].target_y, people[result_t-1].target_x);
			int min = 1e9;
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					if (bfs_result[i][j] == 0)continue; // 방문하지 않는 지점은 제외
					if (base_camp_map[i][j] == 1 && wall_map[i][j] != 1) {
						if (min > bfs_result[i][j]) {
							min = bfs_result[i][j];
							people[result_t-1].now_y = i;
							people[result_t - 1].now_x = j;
						}
					}
				}
			}
			people[result_t - 1].start = true;
			// 해당 위치 벽세우기
			wall_map[people[result_t - 1].now_y][people[result_t - 1].now_x] = 1;
		}
		//dbg_print();
		if (check()) break;
	}
	cout << result_t << endl;

}
