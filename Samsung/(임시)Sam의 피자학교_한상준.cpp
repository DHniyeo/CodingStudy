#include <iostream>
#include <vector>	
#include <queue>

using namespace std;

int n, k;

int dx[4] = {0,1,0,-1};
int dy[4] = {-1,0,1,0};

vector<int> arr;
vector<vector<int>> board;

//void DebugBoard() {
//	cout << '\n';
//	for (int i = 0; i < n; i++) {
//		for (int j = 0; j < n; j++) {
//			cout << board[i][j] << " ";
//
//		}
//		cout << '\n';
//	}
//}

void MinPlusOne() {
	int min = *std::min_element(arr.begin(), arr.end());

	for (int& n : arr) {
		if (n==min)
			n++;
	}
}

void Bfs(pair<int, int> pos) {
	vector<vector<bool>> visited(n, vector<bool>(n, false));
	queue<pair<int, int>> q;

	vector<vector<int>> delta(n, vector<int>(n, 0));
	visited[pos.first][pos.second] = true;
	q.push(pos);

	for (int dir = 0; dir < 4; dir++) {
		int nxtY = pos.first + dy[dir];
		int nxtX = pos.second + dx[dir];

		if (nxtY < 0 || nxtX < 0 || nxtY >= n || nxtX >= n)
			continue;

		if (board[nxtY][nxtX] == 0)
			continue;

		if (board[pos.first][pos.second] > board[nxtY][nxtX]) {
			int d = (board[pos.first][pos.second] - board[nxtY][nxtX]) / 5;
			delta[pos.first][pos.second] -= d;
			delta[nxtY][nxtX] += d;
		}
		else {
			int d = (board[nxtY][nxtX] - board[pos.first][pos.second]) / 5;
			delta[pos.first][pos.second] += d;
			delta[nxtY][nxtX] -= d;
		}
	}

	while (q.empty() == false) {
		auto temp = q.front();
		q.pop();
		int nowY = temp.first;
		int nowX = temp.second;

		for (int dir = 0; dir < 4; dir++) {
			int nxtY = nowY + dy[dir];
			int nxtX = nowX + dx[dir];

			if (nxtY < 0 || nxtX < 0 || nxtY >= n || nxtX >= n)
				continue;

			if (board[nxtY][nxtX] == 0)
				continue;

			if (visited[nxtY][nxtX])
				continue;

			q.push({ nxtY,nxtX });
			visited[nxtY][nxtX] = true;


			int ny = nxtY;
			int nx = nxtX;

			for (int dd = 0; dd < 4; dd++) {
				int nxy = ny + dy[dd];
				int nxx = nx + dx[dd];

				if (nxy < 0 || nxx < 0 || nxy >= n || nxx >= n)
					continue;
				if (board[nxy][nxx] == 0)
					continue;
				if (visited[nxy][nxx])
					continue;

				if (board[ny][nx] > board[nxy][nxx]) {
					int d = (board[ny][nx] - board[nxy][nxx]) / 5;
					delta[ny][nx] -= d;
					delta[nxy][nxx] += d;
				}
				else {
					int d = (board[nxy][nxx] - board[ny][nx]) / 5;
					delta[ny][nx] += d;
					delta[nxy][nxx] -= d;
				}

			}

		}

	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			board[i][j] += delta[i][j];
		}
	}

}

void Push() {
	int flag = false;
	for (int y = 0; y < n; y++) {
		for (int x = 0; x < n; x++) {
			if (board[y][x] != 0) {
				Bfs({ y,x });
				flag = true;
				break;
			}
		}
		if (flag)
			break;
	}
}

void Rotate(vector<vector<int>>& square, vector<vector<int>>& square2) {
	vector<vector<int>> temp(square[0].size(),vector<int>(square.size()));
	int y = temp.size();
	int x = temp[0].size();
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			temp[i][j] = square[x-j][i];
		}
	}
	square2 = temp;
}

void Roll() {
	board[n - 2][1] = board[n - 1][0];
	board[n - 1][0] = 0;

	while (true) {
		// 돌릴부분찾기
		int startX; // 돌릴 사각형 시작 x좌표
		int endX;
		int endY; // 돌릴 사각형 최고 위쪽 Y좌표

		for (int i = 0; i < n; i++) {
			if (board[n - 1][i] != 0) {
				startX = i;
				break;
			}
		}

		for (int i = n - 1; i >= 0; i--) {
			if (board[i][startX] == 0) {
				endY = i + 1;
				break;
			}
		}

		for (int i = startX; i < n; i++) {
			if (board[endY][i] == 0) {
				endX = i - 1;
				break;
			}
		}
		// 보드에 적용가능한지 못하면 break;
		if (n - endY > n - 1 - endX)
			break;

		// 사각형돌리기
		vector<vector<int>> temp(n - endY, vector<int>(endX - startX + 1, 0));
		for (int i = 0; i < n - endY; i++) {
			for (int j = 0; j < endX - startX + 1; j++) {
				temp[i][j] = board[i + endY][j + startX];
			}
		}

		vector<vector<int>> temp2(temp[0].size(), vector<int>(temp.size()));
		int y = temp2.size();
		int x = temp2[0].size();
		for (int i = 0; i < y; i++) {
			for (int j = 0; j < x; j++) {
				temp2[i][j] = temp[x - j-1][i];
			}
		}
		// 보드에 적용
		for (int i = 0; i < y; i++) {
			for (int j = 0; j < x; j++) {
				board[i+n-y-1][j+endX+1] = temp2[i][j];
			}
		}
		// 원래 자리 0으로
		for (int i = 0; i < n - endY; i++) {
			for (int j = 0; j < endX - startX + 1; j++) {
				board[i + endY][j + startX] = 0;
			}
		}
	}

	
}

void Move() {
	int startX; // 돌릴 사각형 시작 x좌표
	int endX;
	int endY; // 돌릴 사각형 최고 위쪽 Y좌표

	for (int i = 0; i < n; i++) {
		if (board[n - 1][i] != 0) {
			startX = i;
			break;
		}
	}

	for (int i = n - 1; i >= 0; i--) {
		if (board[i][startX] == 0) {
			endY = i + 1;
			break;
		}
	}

	for (int i = startX; i < n; i++) {
		if (board[endY][i] == 0) {
			endX = i - 1;
			break;
		}
	}
	int cnt = 0;
	int sizeY = n - endY;
	int sizeX = endX - startX + 1;
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			 board[n-1][cnt] = board[n-1-j][startX+i];
			 cnt++;
			 board[n - 1 - j][startX + i] = 0;
		}
	}
}
void DoubleFold(){
	vector<int> temp(n / 2, 0);
	for (int i = 0; i < n / 2; i++) {
		temp[i] = board[n - 1][i];
		board[n - 1][i] = 0;
	}
	std::reverse(temp.begin(), temp.end());
	for (int i = 0; i < n / 2; i++) {
		board[n - 2][i + n / 2] = temp[i];
	}
	
	vector<vector<int>> temp2(2, vector<int>(n/4,0));
	//vector<vector<int>> temp2(2, vector<int>(n/4,0));

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < n / 4; j++) {
			temp2[i][j] = board[n - 2 + i][n / 2 + j];
			board[n - 2 + i][n / 2 + j] = 0;
		}
	}
	

	vector<vector<int>> temp3(2, vector<int>(n/4, 0));

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < n/4; j++) {
			temp3[i][j] = temp2[n/4-i-1][2-j-1];
		}
	}
	
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < n/4; j++) {
			board[n-4+i][n/2+n/4+j] = temp3[i][j];
		}
	}
	
}

void Move2() {
	int idx = 0;

	for (int i = 0; i < n / 4; i++) {
		for (int j = 0; j < 4; j++) {
			board[n - 1][idx] = board[n-1-j][n/2+n/4+i];
			idx++;
			board[n - 1 - j][n / 2 + n / 4 + i] = 0;
		}
	}
}


int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> k;

	arr = vector<int>(n);
	

	for (int& num : arr) {
		cin >> num;
	}

	int r = 1;
	while (true) {
		board = vector<vector<int>>(n, vector<int>(n, 0));
		// 제일작은 밀가루양 + 1
		MinPlusOne();
		// 보드에 적용
		board[n - 1] = arr;
		// 말기
		Roll();
		//DebugBoard();
		// 누르기
		Push();
		//DebugBoard();
		// 옮기기
		Move();
		//DebugBoard();
		// 2번 반으로 접기
		DoubleFold();

		Push();

		//DebugBoard();
		
		Move2();


		int min = *std::min_element(board[n - 1].begin(), board[n - 1].end());
		int max = *std::max_element(board[n - 1].begin(), board[n - 1].end());

		if (max - min <= k)
			break;

		r++;
	}
	cout << r;
	
}
