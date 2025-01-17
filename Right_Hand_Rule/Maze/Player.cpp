#include "pch.h"
#include "Player.h"
#include "Board.h"
#include <stack>

void Player::Init(Board* board)
{
	_pos = board->GetEnterPos();
	_board = board;

	//RightHand();
	//Bfs();
	AStar();
}

void Player::Update(uint64 deltaTick)
{
	// 길찾기 알고리즘 구현
	if (_pathIndex >= _path.size())
	{
		_board->GenerateMap();
		Init(_board);
		return;
	}
	
	_sumTick += deltaTick;
	if (_sumTick >= MOVE_TICK)
	{
		_sumTick = 0;

		_pos = _path[_pathIndex];
		_pathIndex++;
	}
}

bool Player::CanGo(Pos pos)
{
	TileType tileType = _board->GetTileType(pos);
	return tileType == TileType::EMPTY;
}

void Player::RightHand()
{
	Pos pos = _pos;

	_path.clear();
	_path.push_back(pos);

	// 목적지 도착하기 전에는 계속 실행
	Pos dest = _board->GetExitPos();

	Pos front[4] =
	{
		Pos { -1, 0},	// UP
		Pos { 0, -1},	// LEFT
		Pos { 1, 0},	// DOWN
		Pos { 0, 1},	// RIGHT
	};

	while (pos != dest)
	{
		// 1. 현재 바라보는 방향을 기준으로 오른쪽으로 갈 수 있는지 확인.
		int32 newDir = (_dir - 1 + DIR_COUNT) % DIR_COUNT;
		if (CanGo(pos + front[newDir]))
		{
			// 오른쪽 방향으로 90도 회전.
			_dir = newDir;
			// 앞으로 한 보 전진.
			pos += front[_dir];

			_path.push_back(pos);
		}
		// 2. 현재 바라보는 방향을 기준으로 전진할 수 있는지 확인.
		else if (CanGo(pos + front[_dir]))
		{
			// 앞으로 한 보 전진.
			pos += front[_dir];

			_path.push_back(pos);
		}
		else
		{
			// 왼쪽 방향으로 90도 회전.
			_dir = (_dir + 1) % DIR_COUNT;
		}
	}

	stack<Pos> s;

	for (int i = 0; i < _path.size() - 1; i++)
	{
		if (s.empty() == false && s.top() == _path[i + 1])
			s.pop();
		else
			s.push(_path[i]);
	}

	// 목적지 도착
	if (_path.empty() == false)
		s.push(_path.back());

	vector<Pos> path;
	while (s.empty() == false)
	{
		path.push_back(s.top());
		s.pop();
	}

	std::reverse(path.begin(), path.end());

	_path = path;
}

void Player::Bfs()
{
	Pos pos = _pos;

	// 목적지 도착하기 전에는 계속 실행
	Pos dest = _board->GetExitPos();

	Pos front[4] =
	{
		Pos { -1, 0},	// UP
		Pos { 0, -1},	// LEFT
		Pos { 1, 0},	// DOWN
		Pos { 0, 1},	// RIGHT
	};

	const int32 size = _board->GetSize();
	vector<vector<bool>> discovered(size, vector<bool>(size, false));

	//vector<vector<Pos>> parent;
	// parent[A] = B; -> A는 B로 인해 발견함
	map<Pos, Pos> parent;

	queue<Pos> q;
	q.push(pos);
	discovered[pos.y][pos.x] = true;
	parent[pos] = pos;

	while (q.empty() == false)
	{
		pos = q.front();
		q.pop();

		// 방문!
		if (pos == dest) // 목적지에 다 왔으면 break
			break;

		for (int32 dir = 0; dir < 4; dir++)
		{
			Pos nextPos = pos + front[dir];
			// 갈 수 있는 지역은 맞는지 확인.
			if (CanGo(nextPos) == false)
				continue;
			// 이미 발견한 지역인지 확인.
			if (discovered[nextPos.y][nextPos.x])
				continue;

			q.push(nextPos);
			discovered[nextPos.y][nextPos.x] = true;
			parent[nextPos] = pos;
		}
	}

	_path.clear();

	// 거꾸로 거슬러 올라간다
	pos = dest;

	while (true)
	{
		_path.push_back(pos);

		// 시작점은 자신이 곧 부모이다
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(_path.begin(), _path.end()); 

	// 기회가 되면 이걸 안보고 만들어보기
	// 코드를 외우는것이 아니라 공책을 들고 펜으로 알고리즘을 똑같이 어떤 알고리즘
	// 인지 완벽히 이해했는지를 펜으로 그려봐서 흐름을 그려보는것이 중요.
	// 경우에 따라서 알고리즘같은 경우에는 항상 코드로 만드는게 아니라 진짜 동작을 잘 이해했는지
	// 펜으로 써보는게 굉장히 많은 도움이 된다
}

struct PQNode
{
	bool operator<(const PQNode& other) const { return f < other.f; }
	bool operator>(const PQNode& other) const { return f > other.f; }
	// 최종 점수의 대소 비교를 위해 부등호 연산을 재정의해주었다.

	int32	f;	// f = g + h
	int32	g;
	Pos		pos;
};

void Player::AStar()
{
	// 점수 매기기
	// F = G + H
	// F = 최종 점수 (작을수록 좋음. 경로에 따라 달라짐)
	// G = 시작점에서 해당 좌표까지 이동하는데 드는 비용 (작을수록 좋음. 경로에 따라 달라짐)
	// H = 목적지에서 얼마나 가까운지 체크 (작을수록 좋음. 고정), 휴리스틱 추정값의 H . 
	// 목적지에서 얼마나 가까운지 체크하기 위해 사용하는 비용 계산 공식. 
	// 대표적으로 피타고라스 정리 법칙을 이용하지만 경우에 따라 자유롭게 설정할 수 있다.

	// A* 알고리즘은 각 노드 간의 점수를 계산해 해당 점수가 낮은 순으로 순위를 매긴다. 
    // 최종 점수는 시작 노드에서 해당 노드까지 이동하는 데 드는 비용 G와 목적지에서 얼마나 가까운지를 체크하는 H를 합산하여 구한다.
	
	// 시작 노드와 목적지 노드 정하기
	Pos start = _pos;
	Pos dest = _board->GetExitPos();
	// A* 알고리즘을 이용하려면 시작 노드와 목적지 노드를 알아야한다.

	// 탐색 방향 정하기
	enum
	{
		DIR_COUNT = 4
	};

	Pos front[] =
	{
		Pos { -1, 0 },	// UP
		Pos { 0, -1 },	// LEFT
		Pos { 1, 0 },	// DOWN
		Pos { 0, 1 },	// RIGHT
		Pos { -1, -1 },	// UP_LEFT
		Pos { 1, -1 },	// DOWN_LEFT
		Pos { 1, 1 },	// DOWN_RIGHT
		Pos { -1, 1 },	// UP_RIGHT
	};

	int32 cost[] =
	{
		10,	// UP
		10,	// LEFT
		10,	// DOWN
		10,	// RIGHT
		14,	// UP_LEFT
		14,	// DOWN_LEFT
		14,	// DOWN_RIGHT
		14	// UP_RIGHT
	};

	const int32 size = _board->GetSize();

	// 앞서 만들었던 알고리즘에서는 확인 방향을 상하좌우 총 4방향으로만 설정해주었는데 이번엔 대각 방향까지 지정해주었다. 
	// 또한 A* 알고리즘을 사용하려면 각 방향에 위치한 노드까지의 이동하는데 소요되는 비용을 설정해주어야 하기 때문에 
	// cost라는 배열을 따로 만들어 비용을 정하였다. 
	// 기존에 설정해두었던 DIR_COUNT도 따로 빼주어 관리하였다.

	// ClosedList
	// closed[y][x] -> (y, x)에 방문을 했는지 여부
	vector<vector<bool>> closed(size, vector<bool>(size, false));
	// closed라는 이차 배열을 하나 생성해줘서 해당 노드를 방문했는지 여부를 저장한다. 
	// 방문 유무만 저장하기 때문에 이 배열은 클로즈 리스트(Closed List)에 속한다. 
	// 클로즈 리스트는 방문 여부만을 저장하는 목적을 가진다.

	// best[y][x] -> 지금까지 (y, x)에 대한 가장 좋은 비용 (작을수록 좋음)
	vector<vector<int32>> best(size, vector<int32>(size, INT32_MAX));
	// best는 해당 노드까지 이동했을 때 가장 적게 소모된 비용만을 저장하는 배열이다. 
	// 뒤늦게 더 적은 비용을 가지는 경로를 발견했을 때를 대비해 최상의 조건을 유지하기 위해 사용한다.

	// 부모 추적 용도
	map<Pos, Pos> parent;
	// parent는 부모 노드의 정보를 저장하기 위해 사용한다. 해당 맵을 통해 최단 경로를 역으로 추적하여 구할 수 있다.

	// OpenList (발견)
	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq; // 오름차순, 숫자가 작은 순으로 출력
	// priority_queue, 즉 우선순위 큐는 오픈 리스트(Open List)로 클로즈 리스트와는 다르게 발견한 노드들의 정보를 저장한다. 
	// 아직 탐색 전 노드들이라는 것에 주의해야한다. 
	// 해당 큐에는 노드의 좌표 뿐만 아니라 A* 알고리즘에 사용할 최종 점수 F, 거리 비용 G도 함께 저장된다.

	// 1) 예약(발견) 시스템 구현
	// - 이미 더 좋은 경로를 찾았다면 스킵
	// 2) 뒤늦게 더 좋은 경로가 발견될 수 있음 -> 예외 처리 필수
	// - OpenList에서 찾아서 제거하거나
	// - pq에서 pop한 다음에 무시하거나
	// ㄴ> 택 1

	// 초기값
	{
		int32 g = 0;
		// 휴리스틱 공식은 자유롭게 지정할 수 있다.
		int32 h = 10 * (abs(dest.y - start.y) + abs(dest.x - start.x)); // abs는 절댓값을 의미
		pq.push(PQNode{ g + h, g, start });
		best[start.y][start.x] = g + h;
		parent[start] = start;
	}
	// 시작 노드에서 출발하기 때문에 초기 비용은 0이다. 휴리스틱 추정값은 [Y 좌표의 차이 + X 좌표의 차이] 값에 10을 곱해주는 방식으로 구해주었다.
	// 그런 다음 오픈 리스트(우선순위 큐)에 해당 좌표에 대한 정보를 저장해주면서 시작 노드를 발견 리스트에 추가해 관리한다.
	// best에는 방금 계산한 F값을 저장해주고 부모 노드 정보는 자기 자신으로 세팅해준다.

	// 여기서부터 오픈 리스트에 데이터가 없을 때까지 무한 반복하며 실행되는 코드.
	while (!pq.empty())
	{
		// 제일 좋은 후보 찾기
		PQNode node = pq.top();
		pq.pop();
		// 우선순위 큐를 사용했기 때문에 가장 앞에 있는 노드를 꺼내면 
		// 최소 비용을 소모하는 노드가 추출될 것이다.

		// 예외 처리
		// 동일한 좌표를 여러 경로로 찾았을 때,
		// 더 빠른 경로로 인하여 이미 방문(closed)된 경우 스킵
		// 이 때, 위에서 만들었던 배열 중 closed를 사용하거나 best를 사용하는 것은 자유이다.

		// [선택]
		// 1. closed 사용
		if (closed[node.pos.y][node.pos.x])
			continue;
		// 2. best 사용
		if (best[node.pos.y][node.pos.x] < node.f)
			continue;

		// 해당 노드 방문
		closed[node.pos.y][node.pos.x] = true;

		// 목적지에 도착했으면 바로 종료
		if (node.pos == dest)
			break;
		// 클로즈 리스트의 정보를 변경해주어 노드 방문 상태를 변경해주었다. 
		// 해당 노드의 현재 좌표가 목적지와 동일하다면 반복문은 즉시 종료된다.

		// 다음 진행 노드 탐색
		for (int32 dir = 0; dir < DIR_COUNT; dir++)
		{
			Pos nextPos = node.pos + front[dir];
			// 갈 수 있는 지역은 맞는지 확인
			if (!CanGo(nextPos))
				continue;
			// [선택] 이미 방문한 곳이면 스킵
			if (closed[nextPos.y][nextPos.x])
				continue;

			// 비용 계산
			int32 g = node.g + cost[dir];
			int32 h = 10 * (abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x));
			// 다른 경로에서 더 빠른 길을 찾았으면 스킵
			// 동일하면 먼저 세팅된 best 값 사용
			if (best[nextPos.y][nextPos.x] <= g + h)
				continue;

			// 예약 진행
			best[nextPos.y][nextPos.x] = g + h;
			pq.push(PQNode{ g + h, g, nextPos });
			parent[nextPos] = node.pos;
		}
	}
	// 8방향으로 주변 노드를 탐색한다. 먼저 갈 수 있는 노드인지 탐색하고 다음으로 이미 방문한 노드라면 스킵하는 예외 처리까지 해주었다.
	// 해당 구간을 무사히 통과했다는 것은 갈 수 있는 노드 + 아직 방문하지 않은 노드라고 볼 수 있다.
	// 이제 휴리스틱 추정값을 이용하여 비용을 계산해준다. G는 현재 노드의 G값과 다음 노드로 가는 간선의 비용 cost[dir]을 더해준다.
	// 이렇게 구한 비용이 이미 이전에 발견되어 구해진 비용(이 있을 때)보다 낮다면 정보를 갱신해주고 다음 노드로의 설정을 진행한다.
	// 위의 과정을 반복하면 오픈 리스트에 있는 노드 중에서 F값이 가장 적은 노드 순으로 뽑히게 될 것이며 다음 노드로 설정되고, 
	// 최단 경로를 가지는 길찾기가 진행될 것이다.

	Pos pos = dest;

	_path.clear();
	_pathIndex = 0;

	// 경로 계산
	while (true)
	{
		_path.push_back(pos);

		// 시작점은 자신이 곧 부모이다.
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(_path.begin(), _path.end());

	// A* 알고리즘은 서버를 만들때도 많이 사용된다.
	// 코드를 공부할때 이 알고리즘을 이해하고 구현하는 게 중요한 것이지 세부적으로 어떤 컨테이너를 이용해서 구현한다가 핵심이 아니다
	// 코드를 공부할때 너무 암기 과목처럼 접근하면 안되고 이 알고리즘을 이해한 다음, 구현하는 방식은 사용자 마음대로 만들면 된다는 얘기
	// 그래서 a* 코드를 복기해보고 펜으로 그려보고 코드도 안보고 만들어보는 연습도 해보기
}
