#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int dy[4] = { -1,0,1,0 };
int dx[4] = { 0,1,0,-1 };

vector<vector<int>> board;


int n, k;

//void DebugBoard()
//{
//	cout << "\n";
//	for (int i = 0;i<n;i++)
//	{
//		for (int j = 0;j<n;j++)
//		{
//			cout << board[i][j] << " ";
//		}
//		cout << "\n";
//	}
//	cout << "\n";
//}

bool Check()
{
	int max = *std::max_element(board[n - 1].begin(), board[n - 1].end());
	int min = *std::min_element(board[n - 1].begin(), board[n - 1].end());

	if (max - min <= k)
		return true;

	return false;
}

void MinPlusOne()
{
	int min = *std::min_element(board[n - 1].begin(), board[n - 1].end());

	for (int& num : board[n-1])
	{
		if (num == min)
			num+=1;
	}
}

void Roll()
{
	int startX = 0;
	int sizeX = 1;
	int xCnt = 2;
	int sizeY = 1;
	int yCnt = 1;

	while (true)
	{
		if (n-1-(startX+sizeX)+1 < sizeY)
			break;

		vector<vector<int>> temp(sizeX, vector<int>(sizeY, 0));

		for (int i =0;i<sizeX;i++)
		{
			for (int j =0;j<sizeY;j++)
			{
				temp[i][j] = board[sizeY - j - 1+n-sizeY][i+startX];
				board[sizeY - j - 1 + n - sizeY][i + startX] = 0;
			}
		}

		for (int i =0;i<sizeX;i++)
		{
			for (int j = 0;j<sizeY;j++)
			{
				board[n-1-sizeX+i][startX+sizeX+j] = temp[i][j];
			}
		}
		//DebugBoard();
		startX = startX + sizeX;
		xCnt--;
		yCnt--;
		if (xCnt == 0)
		{
			xCnt = 2;
			sizeX++;
		}
			
		if (yCnt == 0)
		{
			yCnt = 2;
			sizeY++;
		}
	}
}

void PushDow(pair<int,int> pos)
{
	vector<vector<int>> temp(n, vector<int>(n, 0));
	vector<vector<bool>> visited(n, vector<bool>(n, false));
	queue<pair<int, int>> q;
	visited[pos.first][pos.second] = true;
	q.push(pos);
	{
		int nowY = pos.first;
		int nowX = pos.second;
		for (int dir = 0;dir<4;dir++)
		{
			int nxtY = nowY + dy[dir];
			int nxtX = nowX + dx[dir];

			if (nxtY < 0 || nxtX < 0 || nxtY >= n || nxtX >= n)
				continue;
			if (board[nxtY][nxtX] == 0)
				continue;
			if (visited[nxtY][nxtX])
				continue;

			int d = board[nowY][nowX] - board[nxtY][nxtX];

			if (d>=0)
			{
				d /= 5;
				temp[nowY][nowX] -= d;
				temp[nxtY][nxtX] += d;
			}
			else
			{
				d= std::abs(d);
				d /= 5;
				temp[nowY][nowX] += d;
				temp[nxtY][nxtX] -= d;
			}
		}
	}

	while(q.empty()== false)
	{
		auto ppos = q.front();
		q.pop();
		int nowY = ppos.first;
		int nowX = ppos.second;
		for (int dir = 0; dir < 4; dir++)
		{
			int nxtY = nowY + dy[dir];
			int nxtX = nowX + dx[dir];

			if (nxtY < 0 || nxtX < 0 || nxtY >= n || nxtX >= n)
				continue;
			if (board[nxtY][nxtX] == 0)
				continue;
			if (visited[nxtY][nxtX])
				continue;

			q.push({ nxtY, nxtX });
			visited[nxtY][nxtX] = true;
			{
				int nowY = nxtY;
				int nowX = nxtX;
				for (int dir = 0; dir < 4; dir++)
				{
					int nxtY = nowY + dy[dir];
					int nxtX = nowX + dx[dir];

					if (nxtY < 0 || nxtX < 0 || nxtY >= n || nxtX >= n)
						continue;
					if (board[nxtY][nxtX] == 0)
						continue;
					if (visited[nxtY][nxtX])
						continue;

					int d = board[nowY][nowX] - board[nxtY][nxtX];

					if (d >= 0)
					{
						d /= 5;
						temp[nowY][nowX] -= d;
						temp[nxtY][nxtX] += d;
					}
					else
					{
						d = std::abs(d);
						d /= 5;
						temp[nowY][nowX] += d;
						temp[nxtY][nxtX] -= d;
					}
				}
			}
		}
	}

	for (int i =0;i<n;i++)
	{
		for (int j = 0;j<n;j++)
		{
			board[i][j] += temp[i][j];
		}
	}

	//DebugBoard();
}

void Move()
{
	int startX = 0;
	int startY = n - 1;

	for (int i=0;i<n;i++)
	{
		if (i >= n)
			break;

		if (board[n - 1][i] != 0)
			break;
		startX++;
	}
	int x = 0;
	while(true)
	{
		board[n - 1][x] = board[startY][startX];
		board[startY][startX] = 0;

		startY--;
		x++;

		if (startY<0 || board[startY][startX] == 0)
		{
			startY = n - 1;
			startX++;
		}
		if (startX == n)
			break;
		if (x == n)
			break;
		if (board[n - 1][x] != 0)
			break;
	}
	//DebugBoard();
	int a = 0;
}

void DoubleFold()
{
	{
		vector<int> temp(n / 2, 0);
		for(int i =0;i<n/2;i++)
		{
			temp[i] = board[n - 1][i];
			board[n - 1][i] = 0;
		}
		std::reverse(temp.begin(), temp.end());

		for(int i =0;i<n/2;i++)
		{
			board[n - 2][i + n / 2] = temp[i];
		}
	}
	//DebugBoard();
	int a = 0;
	{
		vector<vector<int>> temp(2, vector<int>(n / 4, 0));
		for (int i = 0;i<2;i++)
		{
			for (int j = 0;j<n/4;j++)
			{
				temp[i][j] = board[i + n - 2][j + n / 2];
				board[i + n - 2][j + n / 2] = 0;
			}
		}
		vector<vector<int>> temp2(2, vector<int>(n / 4, 0));
		for (int i =0;i<2;i++)
		{
			for (int j = 0;j<n/4;j++)
			{
				temp2[i][j] = temp[1-i][n/4-j-1];
			}
		}
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < n / 4; j++)
			{
				board[n-4+i][j+n/2+n/4] = temp2[i][j];
			}
		}

		//DebugBoard();
		int a = 0;
	}
	//DebugBoard();
	int b = 0;
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> k;

	board = vector<vector<int>>(n,vector<int>(n,0));

	for (int i =0;i<n;i++)
	{
		cin >> board[n - 1][i];
	}

	int r = 1;
	while(true)
	{
		// 최솟값 +1
		MinPlusOne();

		Roll();

		//DebugBoard();

		bool flag1 = false;
		for (int i = 0;i<n;i++)
		{
			for (int j = 0;j<n;j++)
			{
				if (board[i][j]!=0) {
					PushDow({ i,j });
					flag1 = true;
					break;
				}
			}
			if (flag1)
				break;
		}

		Move();

		DoubleFold();

		bool flag2 = false;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (board[i][j] != 0) {
					PushDow({ i,j });
					flag2 = true;
					break;
				}
			}
			if (flag2)
				break;
		}

		Move();
		//DebugBoard();




		//최솟값 체크
		if (Check())
			break;
		// 라운드 증가
		r++;
	}

	cout << r;

    return 0;
}
