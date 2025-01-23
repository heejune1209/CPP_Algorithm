#include "pch.h"
#include "Board.h"
#include "ConsoleHelper.h"
#include "Player.h"
#include "DisjointSet.h"

const char* TILE = "■";

Board::Board()
{

}

Board::~Board()
{

}

void Board::Init(int32 size, Player* player)
{
	_size = size;
	_player = player;

	//GenerateMap();
	//GenerateMap_Kruskal();
	GenerateMap_Prim();
}

void Board::Render()
{
	ConsoleHelper::SetCursorPosition(0, 0);
	ConsoleHelper::ShowConsoleCursor(false);

	for (int32 y = 0; y < 25; y++)
	{
		for (int32 x = 0; x < 25; x++)
		{
			ConsoleColor color = GetTileColor(Pos{ y, x });
			ConsoleHelper::SetCursorColor(color);
			cout << TILE;
		}

		cout << endl;
	}
}

// Binary Tree 미로 생성 알고리즘 -> 2025.01.23 Kruskal로 교체
// - Mazes For Programmers
void Board::GenerateMap_Kruskal()
{
	struct CostEdge
	{
		int cost;
		Pos u;
		Pos v;

		bool operator<(CostEdge& other)
		{
			return cost < other.cost;
		}
	};

	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			if (x % 2 == 0 || y % 2 == 0)
				_tile[y][x] = TileType::WALL;
			else
				_tile[y][x] = TileType::EMPTY;
		}
	}

	vector<CostEdge> edges;

	// edges 후보를 랜덤 cost로 등록
	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			if (x % 2 == 0 || y % 2 == 0)
				continue;

			// 우측 연결하는 간선 후보
			if (x < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				edges.push_back(CostEdge{ randValue, Pos{y, x}, Pos{y, x + 2} });
			}

			// 아래쪽 연결하는 간선 후보
			if (y < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				edges.push_back(CostEdge{ randValue, Pos{y, x}, Pos{y + 2, x} });
			}
		}
	}

	// Kruskal
	std::sort(edges.begin(), edges.end());

	DisjointSet sets(_size * _size);

	for (CostEdge& edge : edges)
	{
		// 2차원 배열을 1차원 배열처럼 사용하고 싶을 때 쓰는 방식
		int u = edge.u.y * _size + edge.u.x;
		int v = edge.v.y * _size + edge.v.x;
		// 같은 그룹이면 스킵 (안 그러면 사이클 발생)
		if (sets.Find(u) == sets.Find(v))
			continue;

		// 두 그룹을 병합
		sets.Merge(u, v);

		// 맵에 적용
		int y = (edge.u.y + edge.v.y) / 2;
		int x = (edge.u.x + edge.v.x) / 2;
		_tile[y][x] = TileType::EMPTY;
	}
}

// 프림 알고리즘
// 프림 알고리즘(Prim Algorithm)은 하나의 시작점으로 구성된 트리에 간선을 하나씩 수집하며 탐색을 진행하는 방식의 알고리즘이다. 
// 기준 노드가 존재하기 때문에 각 노드와 연결된 간선만을 비교하여 트리를 확장해 나가는 방식이며 
// 크루스칼 알고리즘과 마찬가지로 최소 스패닝 트리를 만드는 데 사용되는 알고리즘이다. 기준 노드가 있다는 점에서 다익스트라 알고리즘과 비슷하나 
// 프림 알고리즘의 경우 기준 노드가 탐색할 때마다 변경된다는 차이점이 있다.

// 프림은 트리(정점 집합)을 기준으로 최단 cost를 탐색하고, 
// 다익스트라는 변경되지 않는 시작점을 기준으로 최단 cost를 탐색한다.

// 가지를 모아서 둥지를 짓는 느낌
void Board::GenerateMap_Prim()
{
	// 간선 정보를 담은 배열과 정점을 포함한 맵 생성하기
	struct CostEdge
	{
		int cost;
		Pos vtx;

		bool operator<(const CostEdge& other) const
		{
			return cost < other.cost;
		}
	};

	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			if (x % 2 == 0 || y % 2 == 0)
				_tile[y][x] = TileType::WALL;
			else
				_tile[y][x] = TileType::EMPTY;
		}
	}

	// edges[u] : u 정점과 연결된 간선 목록
	map<Pos, vector<CostEdge>> edges;

	// 프림 알고리즘은 기준이 되는 노드가 트리 단위로 진행되기 때문에 하나의 정점과 가중치 정보만 담는다. 
	// 그런 다음, 정점을 포함한 첫 단계의 맵을 생성해주고 간선의 정보를 포함하여 기준이 되는 정점과 
	// 연결된 간선 목록을 맵 형식으로 생성해주었다.

	// 맵으로 생성하면 기준 정점을 키로 두고, 해당 정점과 연결된 간선 목록을 값으로 관리할 수 있게 된다.

	// edges 후보를 랜덤 cost로 등록
	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			if (x % 2 == 0 || y % 2 == 0)
				continue;

			// 우측 연결하는 간선 후보
			if (x < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				Pos u = Pos{ y, x };
				Pos v = Pos{ y, x + 2 };
				edges[u].push_back(CostEdge{ randValue, v });
				edges[v].push_back(CostEdge{ randValue, u });
			}

			// 아래쪽 연결하는 간선 후보
			if (y < _size - 2)
			{
				const int32 randValue = ::rand() % 100;
				Pos u = Pos{ y, x };
				Pos v = Pos{ y + 2, x };
				edges[u].push_back(CostEdge{ randValue, v });
				edges[v].push_back(CostEdge{ randValue, u });
			}
		}
	}
	// 오른쪽과 아래쪽으로의 간선 후보들을 골라 랜덤으로 가중치를 지정해주었다. 
	// 양방향 연결을 위해 u와 v 모두 연결된 간선을 등록해주었다.

	// 프림 알고리즘 구현
	// Prim
	// 해당 정점이 트리에 포함되어 있나?
	map<Pos, bool> added;
	// 어떤 정점이 누구에 의해 연결 되었는지
	map<Pos, Pos> parent;
	// 만들고 있는 트리에 인접한 간선 중, 해당 정점에 닿는 최소 간선의 정보
	map<Pos, int32> best;

	// 다익스트라랑 거의 유사
	// 단, 다익스트라에서는 best가 [시작점]을 기준으로 한 cost
	// 프림에서는 best가 [현재 트리]를 기준으로 한 간선 cost

	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			best[Pos{ y, x }] = INT32_MAX;
			added[Pos{ y, x }] = false; // 옵션
		}
	}

	priority_queue<CostEdge> pq;
	const Pos startPos = Pos{ 1, 1 };   // 랜덤으로 정해도 됨
	pq.push(CostEdge{ 0, startPos });
	best[startPos] = 0;
	parent[startPos] = startPos;

	while (!pq.empty())
	{
		CostEdge bestEdge = pq.top();
		pq.pop();

		// 새로 연결된 정점
		Pos v = bestEdge.vtx;
		// 이미 추가되었다면 스킵
		if (added[v])
			continue;

		added[v] = true;

		// 맵에 적용
		{
			int y = (parent[v].y + v.y) / 2;
			int x = (parent[v].x + v.x) / 2;
			_tile[y][x] = TileType::EMPTY;
		}

		// 방금 추가한 정점에 인접한 간선들을 검사한다
		for (CostEdge& edge : edges[v])
		{
			// 이미 추가 되었으면 스킵
			if (added[edge.vtx])
				continue;

			// 다른 경로로 더 좋은 후보가 발견 되었으면 스킵
			if (edge.cost > best[edge.vtx])
				continue;

			best[edge.vtx] = edge.cost;
			parent[edge.vtx] = v;
			pq.push(edge);
		}
	}
	// 우선순위 큐를 이용하여 구현해주었다. 다익스트라와 비슷하지만 기준이 되는 정점이 다르기 때문에 약간은 다른 계산 방식을 가진다. 
	// 아무래도 여러 간선을 비교해야 하다보니 맵을 이용하여 필요한 모든 정보를 저장하였다. 
	// 간선이라 지칭하는 칸은 정점을 사이에 두고 있기 때문에 두 정점의 좌표를 합친 수를 2로 나눈 값이 간선의 좌표값이 된다.
}

// Binary Tree 미로 생성 알고리즘
// - Mazes For Programmers
void Board::GenerateMap()
{
	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			if (x % 2 == 0 || y % 2 == 0)
				_tile[y][x] = TileType::WALL;
			else
				_tile[y][x] = TileType::EMPTY;
		}
	}

	// 랜덤으로 우측 혹은 아래로 길을 뚫는 작업
	for (int32 y = 0; y < _size; y++)
	{
		for (int32 x = 0; x < _size; x++)
		{
			if (x % 2 == 0 || y % 2 == 0)
				continue;
			if (y == _size - 2 && x == _size - 2)
				continue;

			if (y == _size - 2)
			{
				_tile[y][x + 1] = TileType::EMPTY;
				continue;
			}

			if (x == _size - 2)
			{
				_tile[y + 1][x] = TileType::EMPTY;
				continue;
			}

			const int32 randValue = ::rand() % 2;
			if (randValue == 0)
			{
				_tile[y][x + 1] = TileType::EMPTY;
			}
			else
			{
				_tile[y + 1][x] = TileType::EMPTY;
			}
		}
	}
}

TileType Board::GetTileType(Pos pos)
{
	if (pos.x < 0 || pos.x >= _size)
		return TileType::NONE;

	if (pos.y < 0 || pos.y >= _size)
		return TileType::NONE;

	return _tile[pos.y][pos.x];
}

ConsoleColor Board::GetTileColor(Pos pos)
{
	if (_player && _player->GetPos() == pos)
		return ConsoleColor::YELLOW;

	if (GetExitPos() == pos)
		return ConsoleColor::BLUE;

	TileType tileType = GetTileType(pos);

	switch (tileType)
	{
	case TileType::EMPTY:
		return ConsoleColor::GREEN;
	case TileType::WALL:
		return ConsoleColor::RED;
	}

	return ConsoleColor::WHITE;
}
