#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;
int n, m, k;
const int dy[] = {-1,0,1,0};
const int dx[] = {0,1,0,-1};

struct player_info {
	int y, x, d, s;
	int gun;
	int score;
};
vector<vector<priority_queue<int,vector<int>, less<int>>>> gun_map;
vector<vector<int>> player_map;
vector<player_info> player;

void init() {
	cin >> n >> m >> k;
	gun_map = vector<vector<priority_queue<int, vector<int>, less<int>>>>(n, vector<priority_queue<int, vector<int>, less<int>>>(n));
	player_map = vector<vector<int>>(n, vector<int>(n, 0));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int gun;
			cin >> gun;
			gun_map[i][j].push(gun);
		}
	}
	for (int i = 0; i < m; i++) {
		int y, x, d, s;
		cin >> y >> x >> d >> s;
		y--; x--;
		player.push_back({ y,x,d,s,0,0});
		player_map[y][x] = i+1;
	}
}
void get_gun(int y, int x, int idx) {
	if (!gun_map[y][x].empty()) {
		int best_gun = gun_map[y][x].top();
		if (best_gun > player[idx].gun) {
			gun_map[y][x].pop();
			if (player[idx].gun != 0) gun_map[y][x].push(player[idx].gun); // 지금 가진 총을 가지고 있다면
			player[idx].gun = best_gun;
			//cout << idx+1 << " getted " << best_gun << endl;
		}
	}
}
void fight(int idx1, int idx2) {
	// 플레이어 마주치면 플레이어의 초기 능력치와 가지고 있는 총의 공격력의 합을 비교하여 싸우고 이긴 플레이어는 그 차이만큼 포인트 획득
	// 진 플레이어는 본인이 가진 총을 격자에 내려놓고 해당 플레이어가 가지고있는 방향대로 한칸 이동. 칸이 있거나 격자범위 밖인경우 오른쪽으로 90도 회전하여 이동
	// 해당칸에 총이 있으면 주움.
	int winner_idx, loser_idx;
	if (player[idx1].gun + player[idx1].s > player[idx2].gun + player[idx2].s) {
		winner_idx = idx1;
		loser_idx = idx2;
	}
	else if (player[idx1].gun + player[idx1].s == player[idx2].gun + player[idx2].s) {
		if (player[idx1].s > player[idx2].s) {
			winner_idx = idx1;
			loser_idx = idx2;
		}
		else {
			winner_idx = idx2;
			loser_idx = idx1;
		}
	}
	else {
		winner_idx = idx2;
		loser_idx = idx1;
	}
	int gap = abs((player[idx1].gun + player[idx1].s) -( player[idx2].gun + player[idx2].s));
	player[winner_idx].score += gap;
	//cout << "winner idx : " << winner_idx + 1 << endl;
	//cout << player[winner_idx].gun << player[winner_idx].s << endl;
	//cout << "loser idx : " << loser_idx + 1 << endl;
	//cout << player[loser_idx].gun << player[loser_idx].s << endl;

	// 진사람은 총버리고 이긴사람은 총을 얻음
	if (player[loser_idx].gun > 0) {
		gun_map[player[loser_idx].y][player[loser_idx].x].push(player[loser_idx].gun);
		player[loser_idx].gun = 0;
	}
	get_gun(player[winner_idx].y, player[winner_idx].x, winner_idx);

	int first_dir = player[loser_idx].d;
	do{
		int ny = player[loser_idx].y + dy[player[loser_idx].d];
		int nx = player[loser_idx].x + dx[player[loser_idx].d];
		if (ny < 0|| nx < 0|| ny >=n || nx >= n || player_map[ny][nx] != 0) {
			player[loser_idx].d = (player[loser_idx].d + 1) % 4; // 오른쪽 방향 전환
		}
		else {
			get_gun(ny, nx, loser_idx);
			// 벡터 갱신
			player[loser_idx].y = ny;
			player[loser_idx].x = nx;
			break;
		}
		

	} while (first_dir != player[loser_idx].d);

	// 맵 갱신
	player_map[player[winner_idx].y][player[winner_idx].x] = winner_idx + 1;
	player_map[player[loser_idx].y][player[loser_idx].x] = loser_idx + 1;

}
void dbg_print() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << player_map[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
int main() {

	init();
	//dbg_print();
	for (int t = 0; t < k; t++) {
		for (int idx = 0; idx < m; idx++) {
			int ny = player[idx].y + dy[player[idx].d];
			int nx = player[idx].x + dx[player[idx].d];

			// 첫째플레이어부터 본인의 방향대로 한칸이동 밖으로 나가면 정반대의 방향으로 1 이동
			if (ny < 0 || nx < 0 || ny >= n || nx >= n) {
				player[idx].d = (player[idx].d + 2) % 4;
				ny = player[idx].y + dy[player[idx].d];
				nx = player[idx].x + dx[player[idx].d];
			}
			// 이동한 방향에 총이 있으면 총얻고, 총을 가지고있으면 공격력이 더 쎈 총을 획득 나머지 총들은 격자에둠
			if (player_map[ny][nx] == 0) { // 다른 플레이어 없음
				// 맵 갱신
				player_map[player[idx].y][player[idx].x] = 0;
				player_map[ny][nx] = idx + 1;
				// 벡터 갱신
				player[idx].y = ny;
				player[idx].x = nx;
				get_gun(ny, nx, idx);
			}
			else {
				// 맵 갱신
				player_map[player[idx].y][player[idx].x] = 0;
				// 벡터 갱신
				player[idx].y = ny;
				player[idx].x = nx;
				fight(idx, player_map[ny][nx]-1);
			}
		}
		 //플레이어들의 점수 출력
		//for (int i = 0; i < m; i++) {
		//	cout << player[i].score << " ";
		//}
		//cout << endl;
		//dbg_print();
	}
	// 플레이어들의 점수 출력
	for (int i = 0; i < m; i++) {
		cout << player[i].score << " ";
	}
	cout << endl;
}
