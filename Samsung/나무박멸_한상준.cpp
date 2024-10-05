#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

int n,m,k,c;
int score = 0;

int dy[8] = { -1,0, 1, 0,-1,-1,1,1 };
int dx[8] = { 0, 1, 0, -1,-1,1,-1,1 };

pair<int, int> last_pos;

vector<vector<int>> board;
vector<vector<int>> herbi;

int AdjTree(pair<int,int> pos)
{
	int cnt = 0;

	for (int dir =0;dir<4;dir++)
	{
		int adjY = pos.first + dy[dir];
		int adjX = pos.second + dx[dir];
		if (adjY<1 || adjX<1 || adjX>n || adjY>n)
			continue;

		if (board[adjY][adjX] > 0)
			cnt++;
	}
	return cnt;
}

void Grow()
{
	for (int y = 1;y<=n;y++)
	{
		for (int x = 1;x<=n;x++)
		{
			if (board[y][x] > 0)
				board[y][x] += AdjTree({y,x});
		}
	}
}

void Breed()
{
	vector<vector<int>> tree_board(n + 1, vector<int>(n + 1, 0));
	for (int y = 1; y <= n; y++)
	{
		for (int x = 1; x <= n; x++)
		{
			if (board[y][x] > 0)
			{
				// 빈칸개수 구하기
				int cnt = 0;

				for (int dir = 0; dir < 4; dir++)
				{
					int adjY = y + dy[dir];
					int adjX = x + dx[dir];
					if (adjY<1 || adjX<1 || adjX>n || adjY>n)
						continue;

					if (herbi[adjY][adjX] > 0)
						continue;

					if (board[adjY][adjX] == 0)
						cnt++;
				}
				if (cnt == 0)
					continue;
				// 나누기
				int sum = board[y][x] / cnt;
				for (int dir = 0; dir < 4; dir++)
				{
					int adjY = y + dy[dir];
					int adjX = x + dx[dir];
					if (adjY<1 || adjX<1 || adjX>n || adjY>n)
						continue;

					if (herbi[adjY][adjX] > 0)
						continue;

					if (board[adjY][adjX] == 0)
						tree_board[adjY][adjX] += sum;
				}
			}
				
		}
	}

	for (int y = 1;y<=n;y++)
	{
		for (int x = 1;x<=n;x++)
		{
			if (tree_board[y][x] == 0)
				continue;
			board[y][x] += tree_board[y][x];
		}
	}
}

void DebugBoard()
{
	cout << endl;
	for (int y = 1;y<=n;y++)
	{
		for (int x =1;x<=n;x++)
		{
			cout << board[y][x] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void RemoveHerbi(int year)
{
	for (int y = 1; y <= n; y++)
	{
		for (int x = 1; x <= n; x++)
		{
			if (herbi[y][x] < year)
				herbi[y][x] = 0;
		}
	}
}

int InsertTest(pair<int,int> pos)
{
	int cnt = 0;

	cnt += board[pos.first][pos.second];
	for (int dir = 4;dir<8;dir++)
	{
		int nxtY = pos.first;
		int nxtX = pos.second;
		for (int i = 0; i < k; i++)
		{
			nxtY = nxtY + dy[dir];
			nxtX = nxtX + dx[dir];

			if (nxtY<1 || nxtX<1 || nxtY>n || nxtX>n)
				break;
			if (board[nxtY][nxtX] == -1)
				break;

			if (board[nxtY][nxtX] == 0)
				break;

			cnt += board[nxtY][nxtX];
		}
	}
	return cnt;
}

void Insert(pair<int,int> pos,int year)
{
	herbi[pos.first][pos.second] = year + c;
	board[pos.first][pos.second] = 0;

	for (int dir = 4; dir < 8; dir++)
	{
		int nxtY = pos.first;
		int nxtX = pos.second;
		for (int i = 0; i < k; i++)
		{
			nxtY = nxtY + dy[dir];
			nxtX = nxtX + dx[dir];

			if (nxtY<1 || nxtX<1 || nxtY>n || nxtX>n)
				break;
			if (board[nxtY][nxtX] == -1)
				break;

			if (board[nxtY][nxtX] == 0)
			{
				herbi[nxtY][nxtX] = year + c;
				board[nxtY][nxtX] = 0;
				break;
			}
				

			herbi[nxtY][nxtX] = year + c;
			board[nxtY][nxtX] = 0;
		}
	}
}

int InsertHerbi(int year)
{
	//DebugBoard();
	pair<int, int> max_pos;
	int max_tree = -1;

	for (int y = 1;y<=n;y++)
	{
		for (int x = 1;x<=n;x++)
		{
			if (board[y][x] <= 0)
				continue;
			int dead_tree = InsertTest({ y,x });
			if (dead_tree >max_tree)
			{
				max_pos = { y,x };
				max_tree = dead_tree;
			}
		}
	}
	if (max_tree == -1)
	{
		Insert(last_pos,year);
		return 0;
	}
	Insert(max_pos,year);
	last_pos = max_pos;

	return max_tree;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> m >> k >> c;
	board = vector<vector<int>>(n + 1, vector<int>(n + 1, 0));
	herbi = vector<vector<int>>(n + 1, vector<int>(n + 1, 0));

	for (int y = 1;y<=n;y++)
	{
		for (int x =1;x<=n;x++)
		{
			cin >> board[y][x];
		}
	}

	for (int year = 1;year<=m;year++)
	{
		// 제초제 삭제
		RemoveHerbi(year);
		// 성장
		Grow();
		// 번식
		Breed();
		

		// 제초제 투입
		score += InsertHerbi(year);

		//cout << score << endl;
		//DebugBoard();
		
	}

	cout << score;
}
