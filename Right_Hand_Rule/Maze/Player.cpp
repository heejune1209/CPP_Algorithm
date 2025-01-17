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
	// ��ã�� �˰��� ����
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

	// ������ �����ϱ� ������ ��� ����
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
		// 1. ���� �ٶ󺸴� ������ �������� ���������� �� �� �ִ��� Ȯ��.
		int32 newDir = (_dir - 1 + DIR_COUNT) % DIR_COUNT;
		if (CanGo(pos + front[newDir]))
		{
			// ������ �������� 90�� ȸ��.
			_dir = newDir;
			// ������ �� �� ����.
			pos += front[_dir];

			_path.push_back(pos);
		}
		// 2. ���� �ٶ󺸴� ������ �������� ������ �� �ִ��� Ȯ��.
		else if (CanGo(pos + front[_dir]))
		{
			// ������ �� �� ����.
			pos += front[_dir];

			_path.push_back(pos);
		}
		else
		{
			// ���� �������� 90�� ȸ��.
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

	// ������ ����
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

	// ������ �����ϱ� ������ ��� ����
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
	// parent[A] = B; -> A�� B�� ���� �߰���
	map<Pos, Pos> parent;

	queue<Pos> q;
	q.push(pos);
	discovered[pos.y][pos.x] = true;
	parent[pos] = pos;

	while (q.empty() == false)
	{
		pos = q.front();
		q.pop();

		// �湮!
		if (pos == dest) // �������� �� ������ break
			break;

		for (int32 dir = 0; dir < 4; dir++)
		{
			Pos nextPos = pos + front[dir];
			// �� �� �ִ� ������ �´��� Ȯ��.
			if (CanGo(nextPos) == false)
				continue;
			// �̹� �߰��� �������� Ȯ��.
			if (discovered[nextPos.y][nextPos.x])
				continue;

			q.push(nextPos);
			discovered[nextPos.y][nextPos.x] = true;
			parent[nextPos] = pos;
		}
	}

	_path.clear();

	// �Ųٷ� �Ž��� �ö󰣴�
	pos = dest;

	while (true)
	{
		_path.push_back(pos);

		// �������� �ڽ��� �� �θ��̴�
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(_path.begin(), _path.end()); 

	// ��ȸ�� �Ǹ� �̰� �Ⱥ��� ������
	// �ڵ带 �ܿ�°��� �ƴ϶� ��å�� ��� ������ �˰����� �Ȱ��� � �˰���
	// ���� �Ϻ��� �����ߴ����� ������ �׷����� �帧�� �׷����°��� �߿�.
	// ��쿡 ���� �˰����� ��쿡�� �׻� �ڵ�� ����°� �ƴ϶� ��¥ ������ �� �����ߴ���
	// ������ �Ẹ�°� ������ ���� ������ �ȴ�
}

struct PQNode
{
	bool operator<(const PQNode& other) const { return f < other.f; }
	bool operator>(const PQNode& other) const { return f > other.f; }
	// ���� ������ ��� �񱳸� ���� �ε�ȣ ������ ���������־���.

	int32	f;	// f = g + h
	int32	g;
	Pos		pos;
};

void Player::AStar()
{
	// ���� �ű��
	// F = G + H
	// F = ���� ���� (�������� ����. ��ο� ���� �޶���)
	// G = ���������� �ش� ��ǥ���� �̵��ϴµ� ��� ��� (�������� ����. ��ο� ���� �޶���)
	// H = ���������� �󸶳� ������� üũ (�������� ����. ����), �޸���ƽ �������� H . 
	// ���������� �󸶳� ������� üũ�ϱ� ���� ����ϴ� ��� ��� ����. 
	// ��ǥ������ ��Ÿ��� ���� ��Ģ�� �̿������� ��쿡 ���� �����Ӱ� ������ �� �ִ�.

	// A* �˰����� �� ��� ���� ������ ����� �ش� ������ ���� ������ ������ �ű��. 
    // ���� ������ ���� ��忡�� �ش� ������ �̵��ϴ� �� ��� ��� G�� ���������� �󸶳� ��������� üũ�ϴ� H�� �ջ��Ͽ� ���Ѵ�.
	
	// ���� ���� ������ ��� ���ϱ�
	Pos start = _pos;
	Pos dest = _board->GetExitPos();
	// A* �˰����� �̿��Ϸ��� ���� ���� ������ ��带 �˾ƾ��Ѵ�.

	// Ž�� ���� ���ϱ�
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

	// �ռ� ������� �˰��򿡼��� Ȯ�� ������ �����¿� �� 4�������θ� �������־��µ� �̹��� �밢 ������� �������־���. 
	// ���� A* �˰����� ����Ϸ��� �� ���⿡ ��ġ�� �������� �̵��ϴµ� �ҿ�Ǵ� ����� �������־�� �ϱ� ������ 
	// cost��� �迭�� ���� ����� ����� ���Ͽ���. 
	// ������ �����صξ��� DIR_COUNT�� ���� ���־� �����Ͽ���.

	// ClosedList
	// closed[y][x] -> (y, x)�� �湮�� �ߴ��� ����
	vector<vector<bool>> closed(size, vector<bool>(size, false));
	// closed��� ���� �迭�� �ϳ� �������༭ �ش� ��带 �湮�ߴ��� ���θ� �����Ѵ�. 
	// �湮 ������ �����ϱ� ������ �� �迭�� Ŭ���� ����Ʈ(Closed List)�� ���Ѵ�. 
	// Ŭ���� ����Ʈ�� �湮 ���θ��� �����ϴ� ������ ������.

	// best[y][x] -> ���ݱ��� (y, x)�� ���� ���� ���� ��� (�������� ����)
	vector<vector<int32>> best(size, vector<int32>(size, INT32_MAX));
	// best�� �ش� ������ �̵����� �� ���� ���� �Ҹ�� ��븸�� �����ϴ� �迭�̴�. 
	// �ڴʰ� �� ���� ����� ������ ��θ� �߰����� ���� ����� �ֻ��� ������ �����ϱ� ���� ����Ѵ�.

	// �θ� ���� �뵵
	map<Pos, Pos> parent;
	// parent�� �θ� ����� ������ �����ϱ� ���� ����Ѵ�. �ش� ���� ���� �ִ� ��θ� ������ �����Ͽ� ���� �� �ִ�.

	// OpenList (�߰�)
	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq; // ��������, ���ڰ� ���� ������ ���
	// priority_queue, �� �켱���� ť�� ���� ����Ʈ(Open List)�� Ŭ���� ����Ʈ�ʹ� �ٸ��� �߰��� ������ ������ �����Ѵ�. 
	// ���� Ž�� �� �����̶�� �Ϳ� �����ؾ��Ѵ�. 
	// �ش� ť���� ����� ��ǥ �Ӹ� �ƴ϶� A* �˰��� ����� ���� ���� F, �Ÿ� ��� G�� �Բ� ����ȴ�.

	// 1) ����(�߰�) �ý��� ����
	// - �̹� �� ���� ��θ� ã�Ҵٸ� ��ŵ
	// 2) �ڴʰ� �� ���� ��ΰ� �߰ߵ� �� ���� -> ���� ó�� �ʼ�
	// - OpenList���� ã�Ƽ� �����ϰų�
	// - pq���� pop�� ������ �����ϰų�
	// ��> �� 1

	// �ʱⰪ
	{
		int32 g = 0;
		// �޸���ƽ ������ �����Ӱ� ������ �� �ִ�.
		int32 h = 10 * (abs(dest.y - start.y) + abs(dest.x - start.x)); // abs�� ������ �ǹ�
		pq.push(PQNode{ g + h, g, start });
		best[start.y][start.x] = g + h;
		parent[start] = start;
	}
	// ���� ��忡�� ����ϱ� ������ �ʱ� ����� 0�̴�. �޸���ƽ �������� [Y ��ǥ�� ���� + X ��ǥ�� ����] ���� 10�� �����ִ� ������� �����־���.
	// �׷� ���� ���� ����Ʈ(�켱���� ť)�� �ش� ��ǥ�� ���� ������ �������ָ鼭 ���� ��带 �߰� ����Ʈ�� �߰��� �����Ѵ�.
	// best���� ��� ����� F���� �������ְ� �θ� ��� ������ �ڱ� �ڽ����� �������ش�.

	// ���⼭���� ���� ����Ʈ�� �����Ͱ� ���� ������ ���� �ݺ��ϸ� ����Ǵ� �ڵ�.
	while (!pq.empty())
	{
		// ���� ���� �ĺ� ã��
		PQNode node = pq.top();
		pq.pop();
		// �켱���� ť�� ����߱� ������ ���� �տ� �ִ� ��带 ������ 
		// �ּ� ����� �Ҹ��ϴ� ��尡 ����� ���̴�.

		// ���� ó��
		// ������ ��ǥ�� ���� ��η� ã���� ��,
		// �� ���� ��η� ���Ͽ� �̹� �湮(closed)�� ��� ��ŵ
		// �� ��, ������ ������� �迭 �� closed�� ����ϰų� best�� ����ϴ� ���� �����̴�.

		// [����]
		// 1. closed ���
		if (closed[node.pos.y][node.pos.x])
			continue;
		// 2. best ���
		if (best[node.pos.y][node.pos.x] < node.f)
			continue;

		// �ش� ��� �湮
		closed[node.pos.y][node.pos.x] = true;

		// �������� ���������� �ٷ� ����
		if (node.pos == dest)
			break;
		// Ŭ���� ����Ʈ�� ������ �������־� ��� �湮 ���¸� �������־���. 
		// �ش� ����� ���� ��ǥ�� �������� �����ϴٸ� �ݺ����� ��� ����ȴ�.

		// ���� ���� ��� Ž��
		for (int32 dir = 0; dir < DIR_COUNT; dir++)
		{
			Pos nextPos = node.pos + front[dir];
			// �� �� �ִ� ������ �´��� Ȯ��
			if (!CanGo(nextPos))
				continue;
			// [����] �̹� �湮�� ���̸� ��ŵ
			if (closed[nextPos.y][nextPos.x])
				continue;

			// ��� ���
			int32 g = node.g + cost[dir];
			int32 h = 10 * (abs(dest.y - nextPos.y) + abs(dest.x - nextPos.x));
			// �ٸ� ��ο��� �� ���� ���� ã������ ��ŵ
			// �����ϸ� ���� ���õ� best �� ���
			if (best[nextPos.y][nextPos.x] <= g + h)
				continue;

			// ���� ����
			best[nextPos.y][nextPos.x] = g + h;
			pq.push(PQNode{ g + h, g, nextPos });
			parent[nextPos] = node.pos;
		}
	}
	// 8�������� �ֺ� ��带 Ž���Ѵ�. ���� �� �� �ִ� ������� Ž���ϰ� �������� �̹� �湮�� ����� ��ŵ�ϴ� ���� ó������ ���־���.
	// �ش� ������ ������ ����ߴٴ� ���� �� �� �ִ� ��� + ���� �湮���� ���� ����� �� �� �ִ�.
	// ���� �޸���ƽ �������� �̿��Ͽ� ����� ������ش�. G�� ���� ����� G���� ���� ���� ���� ������ ��� cost[dir]�� �����ش�.
	// �̷��� ���� ����� �̹� ������ �߰ߵǾ� ������ ���(�� ���� ��)���� ���ٸ� ������ �������ְ� ���� ������ ������ �����Ѵ�.
	// ���� ������ �ݺ��ϸ� ���� ����Ʈ�� �ִ� ��� �߿��� F���� ���� ���� ��� ������ ������ �� ���̸� ���� ���� �����ǰ�, 
	// �ִ� ��θ� ������ ��ã�Ⱑ ����� ���̴�.

	Pos pos = dest;

	_path.clear();
	_pathIndex = 0;

	// ��� ���
	while (true)
	{
		_path.push_back(pos);

		// �������� �ڽ��� �� �θ��̴�.
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}

	std::reverse(_path.begin(), _path.end());

	// A* �˰����� ������ ���鶧�� ���� ���ȴ�.
	// �ڵ带 �����Ҷ� �� �˰����� �����ϰ� �����ϴ� �� �߿��� ������ ���������� � �����̳ʸ� �̿��ؼ� �����Ѵٰ� �ٽ��� �ƴϴ�
	// �ڵ带 �����Ҷ� �ʹ� �ϱ� ����ó�� �����ϸ� �ȵǰ� �� �˰����� ������ ����, �����ϴ� ����� ����� ������� ����� �ȴٴ� ���
	// �׷��� a* �ڵ带 �����غ��� ������ �׷����� �ڵ嵵 �Ⱥ��� ������ ������ �غ���
}
