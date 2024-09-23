#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

const int dy[] = {-1,0,0,1};
const int dx[] = {0,-1,1,0};

int N, M;
vector<vector<int>> map;
vector<vector<int>> visited;
vector<pair<int, int>> people;

void init() {
	cin >> N >> M;
	map = vector<vector<int>>(N, vector<int>(N, 0));
	visited = vector<vector<int>>(N, vector<int>(N, 0));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}
	for (int i = 0; i < M; i++) {
		int y, x;
		cin >> y >> x;
		y--, x--;
		map[y][x] = 2;
	}
}
int main()
{
	init();
	for (int t = 0; t < M; t++) {
		// 가고싶은 편의점 방향 찾아서 움직이기

		// 편의점 찾으면 멈춤

		// 편의점과 가까운 베이스캠프 진입


	}


}
