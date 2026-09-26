#include <iostream>
#include <deque>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <bit>
#include <algorithm>

using namespace std;

struct Data {
	unsigned int s;
	unsigned int cursor, x;
};

struct DataHash {
	size_t operator()(const Data& d) const {
		size_t h = 0;

		h = d.s * 2 + 1;

		h = h * 13 + 1000003 + d.cursor;

		h = h * 13 + 1000003 + d.x;

		return h;
	}
};

bool operator==(const Data& a, const Data& b) {
	return a.s == b.s &&
		a.cursor == b.cursor &&
		a.x == b.x;
}

int n;
int arr[32];
unordered_map<Data, int, DataHash> dist;
int ans = 1e9;
int kth[65536][16]; // 전처리됨.

unsigned int insert(unsigned int N, int K) {
	return N | (1 << K);
}

unsigned int find_queue(unsigned int N, int X) {
	return ((1u << N) - 1) ^ X;
}

void bfs() {
	deque<Data> q; // 이름은 q인데 사실 dq.
	q.push_front({ 0u, 0u, 0u });
	dist[{ 0u, 0u, 0u }] = 0;
	while (!q.empty()) {
		Data d = q.front();
		q.pop_front();
		unsigned int s = d.s;
		unsigned int c = d.cursor;
		unsigned int x = d.x;
		unsigned int que = find_queue(x, s);
		/*
		auto it = dist.find({ s, c, x });
		cout << s << ":" << c << ":" << x << ":"
			<< (it == dist.end() ? -1 : it->second) << "\n";
			*/

		if (popcount(s) == n) { // s.size() == n과 같음
			ans = min(ans, dist[{s, c, x}]);
			continue;
		}
		// in x 처리.
		if (x < n && (s == 0 || (c == 0 || kth[s][c-1] < arr[x]) && 
			(c == popcount(s) || kth[s][c] > arr[x]))) {
			unsigned s2 = s;
			s2 = s | (1u << x);
			if (dist.find({ s2, c + 1, x + 1 }) == dist.end() ||
				dist[{ s2, c + 1, x + 1 }] > dist[{s, c, x}]) {
				dist[{s2, c + 1, x + 1}] = dist[{s, c, x}];
				q.push_front({ s2, c + 1, x + 1 });
			}
		}

		// 큐 처리. (push)
		unsigned int que2 = que;
		que2 = insert(que2, x); // que2.push_back(arr[x]);
		if (x < n && (dist.find({ s, c, x + 1 }) == dist.end() ||
			dist[{ s, c, x + 1 }] > dist[{s, c, x}] + 1)) {
			dist[{ s, c, x + 1 }] = dist[{s, c, x}] + 1;
			q.push_back({ s, c, x + 1 });
		}
		// pop 처리.
		if (que!=0 && (c == 0 || s==0 || kth[s][c-1] < arr[countr_zero(que)]) &&
			(c == popcount(s) || kth[s][c] > arr[countr_zero(que)])) {
			unsigned int s2 = s; 
			s2 = insert(s2, countr_zero(que)); // 값이 어느 위치에 있는지 찾기위해 cnt...(이였던것)
			unsigned int que2 = que;
			que2 &= ~(que & -(int)que); // que2.erase(que2.front())
			if (dist.find({ s2, c + 1, x }) == dist.end() ||
				dist[{ s2, c + 1, x}] > dist[{s, c, x}] + 1) {
				// 참고로 x가 증가하지 않는 이유는 
				// 이 행동에서 pop만 하기 때문에 새로 처리한 문자가 없음.
				dist[{ s2, c + 1, x}] = dist[{s, c, x}] + 1;
				q.push_back({ s2, c + 1, x });
			}
		}
		// ptr++ 처리.
		if (c < popcount(s) && (dist.find({s, c + 1, x}) == dist.end() ||
			dist[{ s, c + 1, x }] > dist[{s, c, x}] + 1)) {
			dist[{ s, c + 1, x }] = dist[{s, c, x}] + 1;
			q.push_back({ s, c + 1, x });
		}
		// ptr-- 처리. 
		if (c > 0 && (dist.find({ s, c - 1, x }) == dist.end() ||
			dist[{ s, c - 1, x }] > dist[{s, c, x}] + 1)) {
			dist[{ s, c - 1, x }] = dist[{s, c, x}] + 1;
			q.push_back({ s, c - 1, x });
		}
	}
}
  
int main(void) {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
	}
	// 전처리
	for (unsigned int i = 0; i < (1 << n); i++) { // 모든 mask 생성.
		vector<int> temp;
		for (int j = 0; j < n; j++) {
			if (i & (1 << j)) temp.push_back(arr[j]);
		}
		sort(temp.begin(), temp.end());
		for (int j = 0; j < temp.size(); j++) {
			kth[i][j] = temp[j];
		}
	}
	bfs();
	cout << ans;
	return 0;
}
