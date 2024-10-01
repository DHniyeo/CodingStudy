#include<iostream>
#include<deque>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

typedef pair<int, int> loc;
int N, M, K;
vector<vector<int>> field;
vector<vector<int>> human_map;
vector<deque<pair<int, int>>> humans;
const int dy[] = {-1,0,1,0};
const int dx[] = {0,-1,0,1};
int result = 0;
void make_vector() {
	humans.clear();
	humans.push_back(deque<pair<int,int>>()); // dummy
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (field[i][j] == 1) { // 헤드 찾음
				deque<pair<int, int>> tmp;
				queue<pair<int, int>> q;
				vector<vector<int>> visited(N,vector<int>(N,0));
				q.push({ i,j });
				visited[i][j] = 1;
				tmp.push_back({ i,j });
				int last_py, last_px;
				while (!q.empty()) {
					pair<int, int> now = q.front(); q.pop();
					for (int k = 0; k < 4; k++) {
						int ny = now.first + dy[k];
						int nx = now.second + dx[k];
						if (ny < 0 || nx < 0 || ny >= N || nx >= N) continue;
						if (visited[ny][nx] == 1)continue;
						if (field[ny][nx] == 2) {
							visited[ny][nx] = 1;
							tmp.push_back({ ny,nx });
							q.push({ ny,nx });
							last_py = ny;
							last_px = nx;
						}
					}
				}
				for (int i = 0; i < 4; i++) {
					int ny = last_py + dy[i];
					int nx = last_px + dx[i];
					if (ny < 0 || nx < 0 || ny >= N || nx >= N) continue;
					if (field[ny][nx] == 3) {
						tmp.push_back({ ny,nx });
					}
				}
				humans.push_back(tmp);
			}
		}
	}


}

void init() {
	cin >> N >> M >> K;
	field = vector<vector<int>>(N, vector<int>(N, 0));
	human_map = vector<vector<int>>(N, vector<int>(N, 0));
	result = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> field[i][j];
		}
	}
	// 링 판과 사람들(벡터) 분리.
	make_vector();
	for (int i = 0; i < N; i++) { // field 비움
		for (int j = 0; j < N; j++) {
			if (field[i][j] == 3 || field[i][j] == 2 || field[i][j] == 1) {
				field[i][j] = 4;
			}
		}
	}
}
void move() {
	for (int i = 1; i <= M; i++) {
		loc head = humans[i].front();
		loc next_head;
		for (int j = 0; j < 4; j++) {
			int ny = head.first + dy[j];
			int nx = head.second + dx[j];
			if (ny < 0 || nx < 0 || ny >= N || nx >= N) continue;
			if (field[ny][nx] != 4) continue;
			if (ny == humans[i][1].first && nx == humans[i][1].second) continue; // 뒤로 돌아가는 방향 안감
			next_head = { ny,nx };
		}
		humans[i].push_front(next_head);
		humans[i].pop_back();
	}
}
void make_human_map() {
	human_map = vector<vector<int>>(N, vector<int>(N, 0));
	for (int i = 1; i <= M; i++) {
		for (int j = 0; j < humans[i].size(); j++) {
			int y = humans[i][j].first;
			int x = humans[i][j].second;
			human_map[y][x] = 1;
		}
	}
}
void hit_ball(int y,int x) {
	for (int i = 1; i <= M; i++) {
		for (int j = 0; j < humans[i].size(); j++) {
			int nowy = humans[i][j].first;
			int nowx = humans[i][j].second;
			if (nowy == y && nowx == x) { // 공과 맞은 사람 찾음
				result += (j+1)*(j+1);
				reverse(humans[i].begin(), humans[i].end());
				return;
			}
		}
	}

}
void get_score(int round) {
	int tmp = (round % (4 * N));
	int option = tmp / N;
	if (option == 0) {
		int start = tmp;
		int i = start;
		for (int j = 0; j < N; j++){
			if (human_map[i][j] == 1) {
				hit_ball(i, j);
				break;
			}
		}
	}
	else if (option == 1) {
		int start = tmp % (option *N);
		int j = start;
		for (int i = N - 1; i >= 0; i--) {
			if (human_map[i][j] == 1) {
				hit_ball(i, j);
				break;
			}
		}
	}
	else if (option == 2) {
		int start = tmp % (option *N);
		int i = N - 1 - start;
		for (int j = N - 1; j >= 0; j--) {
			if (human_map[i][j] == 1) {
				hit_ball(i, j);
				break;
			}
		}
	}
	else if (option == 3) {
		int start = tmp % (option *N);
		int j = N - 1 - start;
		for (int i = 0; i < N; i++) {
			if (human_map[i][j] == 1) {
				hit_ball(i, j);
				break;
			}
		}

	}


}
int main() {
	init();
	for (int t = 0; t < K; t++) {
		move();
		make_human_map();
		get_score(t);
	}
	cout << result;
}

//링 판과 사람들(벡터) 분리.
//
//move() :
//	1.머리 사람을 따라 이동
//	좌표로 할때는 머리사람 앞에 추가하고 맨뒤에 꼬리 삭제함.
//
//
//	get_score() :
//	2. 좌표를 이동하면서 머리사람을 시작으로 점수 제곱 얻음.
//	좌표 탐색하면서 해당 좌표 찾으면 팀별 위치 찾기
//	팀별로 위치 찾았으면 위치 변환
//
//	3. 공을 얻은 팀은 방향을 바꿈.
//	reverse();
