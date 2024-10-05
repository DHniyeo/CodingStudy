#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

// 우상좌하
int dy[4] = {0,-1,0,1};
int dx[4] = {1,0,-1,0};

int n, m, k;
int idx = 1;
int score = 0;
int ball_dir = 0;
int ball_idx = 1;

struct Team
{
	int head = 0;
	int tail;

	int cnt = 0;
	int d = 1;

	vector<pair<int, int>> path;
};

vector<vector<int>> board;
vector<vector<int>> board2;
vector<Team> teams;
vector<vector<bool>> visited;

void DebugBoard(int r)
{
	cout << endl;
	for (int y = 1; y <= n; y++)
	{
		for (int x = 1; x <= n; x++)
		{
			cout << board[y][x] << " ";
		}
		cout << endl;
	}
	cout << endl;
	//cout << "ball_dir : " << ball_dir << endl;
	//cout << "ball_idx : " << ball_idx << endl;
	//for (int i = 1;i<=teams.size()-1;i++)
	//{
	//	cout <<i<<" : "<< teams[i].d << endl;
	//}
	cout << ball_idx << endl;
	cout <<r<<" "<< score << endl;
}

void FindPath(pair<int,int> pos,int i)
{
	if (board[pos.first][pos.second] != 4)
		teams[i].cnt++;

	visited[pos.first][pos.second] = true;
	teams[i].path.push_back(pos);

	for (int dir = 0;dir<4;dir++)
	{
		int nxtY = pos.first + dy[dir];
		int nxtX = pos.second + dx[dir];
		if (nxtY<1 || nxtX<1 || nxtY>n || nxtX>n)
			continue;
		if (visited[nxtY][nxtX])
			continue;
		if (board[nxtY][nxtX] == 0)
			continue;


		if (board[pos.first][pos.second] == 1 && board[nxtY][nxtX] == 2)
			continue;

		FindPath({ nxtY,nxtX }, i);

	}

	
}

void FindTeam()
{
	for (int y = 1; y <= n; y++)
	{
		for (int x = 1; x <= n; x++)
		{
			if (board[y][x] == 1)
			{
				FindPath({y,x},idx);
				idx++;
			}
		}
	}

	for (int i = 1;i<=teams.size()-1;i++)
	{
		teams[i].tail = teams[i].path.size() - teams[i].cnt + 1;
	}

}

void Move(int i)
{
	if (teams[i].d== 1)
	{
		board[teams[i].path[teams[i].head].first][teams[i].path[teams[i].head].second] = 2;

		teams[i].head++;
		if (teams[i].head == teams[i].path.size())
			teams[i].head = 0;

		board[teams[i].path[teams[i].head].first][teams[i].path[teams[i].head].second] = 1;

		if (board[teams[i].path[teams[i].tail].first][teams[i].path[teams[i].tail].second] != 1)
			board[teams[i].path[teams[i].tail].first][teams[i].path[teams[i].tail].second] = 4;
		teams[i].tail++;
		if (teams[i].tail == teams[i].path.size())
			teams[i].tail = 0;

		board[teams[i].path[teams[i].tail].first][teams[i].path[teams[i].tail].second] = 3;

	}
	else
	{
		//DebugBoard();
		board[teams[i].path[teams[i].head].first][teams[i].path[teams[i].head].second] = 2;

		teams[i].head--;
		if (teams[i].head == -1)
			teams[i].head = teams[i].path.size() - 1;

		board[teams[i].path[teams[i].head].first][teams[i].path[teams[i].head].second] = 1;
		if (board[teams[i].path[teams[i].tail].first][teams[i].path[teams[i].tail].second]!= 1)
			board[teams[i].path[teams[i].tail].first][teams[i].path[teams[i].tail].second] = 4;

		teams[i].tail--;
		if (teams[i].tail == -1)
			teams[i].tail = teams[i].path.size() - 1;

		board[teams[i].path[teams[i].tail].first][teams[i].path[teams[i].tail].second] = 3;
		//DebugBoard();
	}
}
void GetScore(pair<int,int> pos)
{
	int team_idx = board2[pos.first][pos.second];

	int score_idx = 1;

	if (teams[team_idx].d == 1)
	{
		int player_idx = teams[team_idx].head;
		while (true)
		{
			if (teams[team_idx].path[player_idx] == pos)
				break;
			score_idx++;
			player_idx--;
			if (player_idx == -1)
				player_idx = teams[team_idx].path.size() - 1;
		}
	}
	else
	{
		int player_idx = teams[team_idx].head;
		while (true)
		{
			if (teams[team_idx].path[player_idx] == pos)
				break;
			score_idx++;
			player_idx++;
			if (player_idx == teams[team_idx].path.size())
				player_idx = 0;
		}
	}

	score += (score_idx * score_idx);
	if (teams[team_idx].d == 1)
		teams[team_idx].d = -1;
	else
		teams[team_idx].d = 1;

	std::swap(teams[team_idx].head, teams[team_idx].tail);
	//std::swap(teams[team_idx].head, teams[team_idx].tail);

	std::swap(board[teams[team_idx].path[teams[team_idx].head].first][teams[team_idx].path[teams[team_idx].head].second], board[teams[team_idx].path[teams[team_idx].tail].first][teams[team_idx].path[teams[team_idx].tail].second]);
}

void Ball()
{
	int nowY;
	int nowX;
	if (ball_dir == 0)
	{
		nowY = ball_idx;
		nowX = 1;
	}
	else if (ball_dir == 1)
	{
		nowY = n;
		nowX = ball_idx;
	}
	else if (ball_dir == 2)
	{
		nowY = ball_idx;
		nowX = n;
	}
	else if (ball_dir == 3)
	{
		nowY = 1;
		nowX = ball_idx;
	}

	while (!(nowY>n||nowX>n || nowY<1||nowX<1))
	{
		if (board[nowY][nowX] != 0 && board[nowY][nowX] !=4)
		{
			GetScore({nowY,nowX});
			break;
		}
		nowY = nowY + dy[ball_dir];
		nowX = nowX + dx[ball_dir];
	}

	if (ball_dir == 0 || ball_dir == 1)
		ball_idx++;
	else
	{
		ball_idx--;
	}
	if (ball_idx == n+1 || ball_idx == 0)
	{
		ball_dir++;

		if (ball_dir == 4)
			ball_dir = 0;
		if (ball_dir == 0 || ball_dir == 1)
			ball_idx = 1;
		else
			ball_idx = n;
			
		

	}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> m >> k;

	board = vector<vector<int>>(n + 1, vector<int>(n + 1, 0));
	board2 = vector<vector<int>>(n + 1, vector<int>(n + 1, 0));
	teams = vector<Team>(m + 1);

	for (int y = 1;y<=n;y++)
	{
		for (int x = 1;x<=n;x++)
		{
			cin >> board[y][x];
		}
	}
	visited = vector<vector<bool>>(n + 1, vector<bool>(n + 1, false));
	FindTeam();

	for (int i = 1;i<=teams.size()-1;i++)
	{
		for (auto& pos : teams[i].path)
		{
			board2[pos.first][pos.second] = i;
		}
	}

	for (int r = 1;r<=k;r++)
	{
		// 이동
		for (int i = 1;i<=teams.size()-1;i++)
		{
			Move(i);
		}
		//DebugBoard(r);
		// 공날리기, 점수계산
		Ball();


		int a = 0;
		
	}
	cout << score;
}
