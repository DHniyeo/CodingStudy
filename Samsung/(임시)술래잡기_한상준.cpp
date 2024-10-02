#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };
// 상우하좌

struct Player
{
	int y;
	int x;
	//int d; // 1 좌우 2 상하
	int dir;// = 0;
	bool survive = true;
};

struct Tagger
{
	int y;
	int x;
	int dir = 0;
	int move_count = 2;
	int move_dist = 1;
	int move_d = 0;
	bool d = true;
};

struct Tree
{
	int y;
	int x;
};

int n, m, h, k;
int score = 0;

vector<Player> players;
vector<Tree> trees;
Tagger tagger;

// 술래 움직이기
void TMove() {
	// 중앙에서 끝으로
	if (tagger.d) {
		tagger.y = tagger.y + dy[tagger.dir];
		tagger.x = tagger.x + dx[tagger.dir];
		tagger.move_d++;
		if (tagger.move_d == tagger.move_dist) {
			tagger.move_d = 0;
			tagger.move_count--;
			tagger.dir = (tagger.dir + 1) % 4;
		}

		if (tagger.move_count == 0) {
			tagger.move_count = 2;
			tagger.move_dist++;
			
		}

		if (tagger.y == 1 && tagger.x == 1) {
			tagger.d = false;
			tagger.dir = 2;
			tagger.move_dist--;
			tagger.move_d = 0;
			tagger.move_count = 3;
		}
	}
	else {
		tagger.y = tagger.y + dy[tagger.dir];
		tagger.x = tagger.x + dx[tagger.dir];
		tagger.move_d++;
		if (tagger.move_d == tagger.move_dist) {
			tagger.move_d = 0;
			tagger.move_count--;
			tagger.dir = (tagger.dir - 1+4) % 4;
		}

		if (tagger.move_count == 0) {
			tagger.move_count = 2;
			tagger.move_dist--;

		}

		if (tagger.y == (n / 2 + 1) && tagger.x == (n / 2 + 1)) {
			tagger.d = true;
			tagger.dir = 0;
			tagger.move_dist++;
			tagger.move_d = 0;
		}
	}
}
void PMove(Player& player) {
	int nxtY = player.y + dy[player.dir];
	int nxtX = player.x + dx[player.dir];

	if (nxtY<1 || nxtY>n || nxtX<1 || nxtX>n) {
		player.dir = (player.dir + 2) % 4;
		nxtY = player.y + dy[player.dir];
		nxtX = player.x + dx[player.dir];

		if (nxtY == tagger.y && nxtX == tagger.x) {
			return;
		}
		else {
			player.y = nxtY;
			player.x = nxtX;
		}
	}
	else {
		if (nxtY == tagger.y && nxtX == tagger.x) {
			return;
		}
		else {
			player.y = nxtY;
			player.x = nxtX;
		}
	}
}

int CatchPlayer() {
	int cnt = 0;

	int y = tagger.y;
	int x = tagger.x;
	int flag = false;
	int dist = 0;
	while (dist<3 &&!(y <1 || x<1 || y>n || x>n)) {
		for (Tree& tree : trees) {
			if (y == tree.y && x == tree.x) {
				flag = true;
				break;
			}

		}
		if (flag) {
			y = y + dy[tagger.dir];
			x = x + dx[tagger.dir];
			dist++;
			flag = false;
			continue;
		}
			

		for (Player& player : players) {
			if (player.survive == false)
				continue;
			if (y == player.y && x == player.x) {
				cnt++;
				player.survive = false;
			}
		}


		y = y + dy[tagger.dir];
		x = x + dx[tagger.dir];
		dist++;
	}
	return cnt;
}



int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	
	cin >> n >> m >> h >> k;

	tagger.y = n / 2 + 1;
	tagger.x = n / 2 + 1;


	for (int i = 1; i <= m; i++)
	{
		int t1, t2, t3;
		cin >> t1 >> t2 >> t3;
		players.push_back({ t1,t2});
		if (t3 == 1)
		{
			players.back().dir = 1;
		}
		else
		{
			players.back().dir = 2;
		}
	}

	for (int i = 1; i <= h; i++) {
		int t1, t2;
		cin >> t1 >> t2;
		trees.push_back({ t1,t2 });
	}

	for (int i = 1; i <= k; i++) {
		// 플레이어 이동
		for (Player& player : players) {
			int dist = std::abs(player.y - tagger.y) + std::abs(player.x - tagger.x);
			if (dist <= 3) {
				PMove(player);
			}
		}

		//술래이동
		TMove();

		// 플레이어 잡기
		// 점수
		score += (CatchPlayer() * i);

	}

	cout << score;
}
