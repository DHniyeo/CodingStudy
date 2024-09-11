#include<iostream>
#include<vector>
#include<queue>
#include<cstring>
#include<algorithm>
using namespace std;

int N, M, K;
struct vc_info {
	int damage;
	int last_attack;
	int r, c;
	bool not_repair;
};
struct q_info {
	int r, c;
	vector<vector<int>> visited; // 경로 저장
};

int ad_map[10][10];
vector<vc_info> vc;

bool cmp(vc_info a, vc_info b) {
	if (a.damage < b.damage) { // 공격력이 낮고
		return true;
	}
	else if (a.damage == b.damage) {
		if (a.last_attack > b.last_attack) { // 최근에 공격한
			return true;
		}
		else if (a.last_attack == b.last_attack) {
			if (a.r + a.c > b.r + b.c) { // 행과 열의 합이 큰
				return true;
			}
			else if (a.r + a.c == b.r + b.c) {
				return a.c > b.c; // 열의 값이 더큰
			}
		}
	}
	return false;
}

void init() {
	cin >> N >> M >> K;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> ad_map[i][j];
			if (ad_map[i][j] == 0) continue;
			vc.push_back({ ad_map[i][j], 0, i,j, false });
		}
	}

}

int damage_cal(int a, int b) {
	if (a - b < 0) return 0;
	else return a - b;
}

bool find_dir(int start_idx, int end_idx, vector<vector<int>> &visited) {
	bool flag = false;
	// 최단경로로 검색(맵은 벽이 없이 이어짐, 우하좌상 우선순위)
	int dy[] = { 0,1,0,-1 }; // 우 하 좌 상
	int dx[] = { 1,0,-1,0 };
	queue<q_info> q;
	visited[vc[start_idx].r][vc[start_idx].c] = 1;
	q.push({vc[start_idx].r, vc[start_idx].c, visited });
	
	while (!q.empty()) {
		q_info now = q.front(); q.pop();
		if (now.r == vc[end_idx].r && now.c == vc[end_idx].c) {
			visited = now.visited;
			flag = true;
			break;
		}
		for (int i = 0; i < 4; i++) {
			int ny = now.r + dy[i];
			int nx = now.c + dx[i];
			if (ny < 0) ny = N - 1;
			if (ny >= N) ny = 0;
			if (nx < 0) nx = M - 1;
			if (nx >= M) nx = 0;
			if (ad_map[ny][nx] == 0) continue;
			if (now.visited[ny][nx] == 1) continue;
			now.visited[ny][nx] = 1;
			q.push({ ny,nx,now.visited });
		}
	}
	return flag;
}
void rayzer_attack(int start_idx, int end_idx, vector<vector<int>>& visited) {
	// 공격자는 공격력만큼 피해입히고 경로에 위치한 포탑들은 공격자의 절반 공격력 만큼 피해를 입음
	int damage = vc[start_idx].damage;
	vc[end_idx].damage = damage_cal(vc[end_idx].damage, damage); // 중앙 공격
	for (int i = 0; i < vc.size(); i++) { // 경로 공격
		if (start_idx == i) continue;
		if (end_idx == i) continue;
		if (visited[vc[i].r][vc[i].c] == 1) {
			vc[i].damage = damage_cal(vc[i].damage, damage / 2);
			vc[i].not_repair = true;
		}
	}
}

void bomb_attack(int start_idx, int end_idx) {
	// 공격 대상에 포탄을 던짐. 공격자는 피해 입지 않음. 8방향도 추가적으로 절반 공격력 만큼 피해를 입음
	int damage = vc[start_idx].damage;
	int dy[] = { -1,-1,-1,0,0,1,1,1 };
	int dx[] = { -1,0, 1,-1,1,-1,0,1 };

	vc[end_idx].damage = damage_cal(vc[end_idx].damage, damage); // 중앙 공격
	for (int i = 0; i < 8; i++) { // 주변 공격
		int ny = vc[end_idx].r + dy[i];
		int nx = vc[end_idx].c + dx[i];
		if (ny < 0) ny = N - 1;
		if (ny >= N) ny = 0;
		if (nx < 0) nx = M - 1;
		if (nx >= M) nx = 0;
		if (ad_map[ny][nx] == 0) continue;
		if (ny == vc[start_idx].r && nx == vc[start_idx].c) continue; // 공격자는 공격 피해 안입음
		for (int i = 0; i < vc.size(); i++) { // 벡터 데이터 최신화
			vc_info now = vc[i];
			if (now.r == ny && now.c == nx) {
				vc[i].damage = damage_cal(vc[i].damage, damage / 2);
				vc[i].not_repair = true;
				break;
			}
		}
	}
}
void repair() {
	// 공격 끝나면 부서지지않은 포탑중 공격자, 피해자 포탑이 아닌 포탑은 정비해서 공격력이 + 1 이됨 
	for (int i = 0; i < vc.size(); i++) {
		if (vc[i].damage == 0) continue;
		if (vc[i].not_repair) {
			vc[i].not_repair = false;
			continue;
		}
		vc[i].damage += 1;
	}
}
void change_map() {
	for (int i = 0; i < vc.size(); i++) {
		vc_info now = vc[i];
		ad_map[now.r][now.c] = now.damage;
	}
}
void print_map() {
	cout << endl;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cout << ad_map[i][j] << " ";
		}
		cout << endl;
	}
}
bool check_alive() { // 살아 있는 포탑 찾기
	int cnt = 0;
	for (int i = 0; i < vc.size(); i++) {
		if (vc[i].damage > 0) {
			cnt++;
		}
	}
	if (cnt == 1) return true;
	return false;
}
int find_reuslt() {
	int max = 0;
	for (int i = 0; i < vc.size(); i++) {
		max = max < vc[i].damage ? vc[i].damage : max;
	}
	return max;
}
int main() {
	init();
	for (int t = 1; t <= K; t++) {
		sort(vc.begin(), vc.end(), cmp); // 정렬

		int attacker_idx = 0;
		for (int i = 0; i < vc.size(); i++) { // 공격자 선정
			if (vc[i].damage == 0) continue;
			attacker_idx = i;
			break;
		}
		vc[attacker_idx].damage += N + M; // 공격력 증가
		vc[attacker_idx].last_attack = t;
		vc[attacker_idx].not_repair = true;

		cout << "공격령 증가" << endl;
		change_map(); // 벡터를 기반으로 맵 수정
		print_map();
		
		int defender_idx = 0;
		for (int i = vc.size()-1; i >= 0; i--) { // 피격자 선정
			if (vc[i].damage == 0) continue;
			defender_idx = i;
			break;
		}
		vc[defender_idx].not_repair = true;

		vector<vector<int>> visit = vector<vector<int>>(N, vector<int>(M, 0));

		if (find_dir(attacker_idx, defender_idx,visit)) { // 레이저 공격
			rayzer_attack(attacker_idx, defender_idx, visit);
		}
		else { // 포탄 공격
			bomb_attack(attacker_idx, defender_idx);
		}
		cout << "공격 완료" << endl;
		change_map(); // 벡터를 기반으로 맵 수정
		print_map();

		repair(); // 공격 끝나면 부서지지않은 포탑중 공격자, 피해자 포탑이 아닌 포탑은 정비해서 공격력이 + 1 이됨 
		cout << "정비 완료" << endl;
		change_map(); // 벡터를 기반으로 맵 수정
		print_map();

		// 만약 부서지지 않은 포탑이 1개가 된다면 그 즉시 중지됩니다.
		if (check_alive()) break;


	}
	// 남아있는 포탑중 강한것
	int result = find_reuslt();
	cout << result << endl;
}
