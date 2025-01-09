#include "pch.h"
#include "Player.h"
#include "Board.h"
#include <stack>

void Player::Init(Board* board)
{
	_pos = board->GetEnterPos();
	_board = board;

	Pos pos = _pos;

	_path.clear();
	_path.push_back(pos);

	// 목적지 도착하기 전에는 계속 실행
	Pos dest = board->GetExitPos();

	// 다음 방향 포지션 찾기
	// Pos는 (y, x)의 값을 가지는 구조체
	Pos front[4] =
	{
		Pos {-1,0}, // UP
		Pos {0,-1}, // LEFT
		Pos {1,0}, // Down
		Pos {0,1}, // Right
	};

	while (pos != dest)
	{
		// 1. 현재 바라보는 방향을 기준으로 오른쪽으로 갈수있는지 확인.
		int32 newDir = (_dir - 1 + DIR_COUNT) % DIR_COUNT;
		if (CanGo(pos + front[newDir]))
		{
			// 오른쪽 방향으로 90도 회전.
			_dir = newDir;
			// 앞으로 한 보 전진
			pos += front[_dir];
			// 이동한 좌표를 저장
			_path.push_back(pos);
		}
		// 2. 현재 바라보는 방향을 기준으로 전진할 수 있는지 확인.
		else if (CanGo(pos +front[_dir]))
		{
			// 앞으로 한 보 전진
			pos += front[_dir];
			_path.push_back(pos);
		}
		else
		{
			// 왼쪽 방향으로 90도 회전.
			// 아래에 스위치 문으로 만들수 있는것을 이렇게 한줄로 만들어버릴수 있음
			_dir = (_dir + 1) % DIR_COUNT;
				
			/*switch (_dir)
			{
			case DIR_UP:
				_dir = DIR_LEFT;
				break;
			case DIR_LEFT:
				_dir = DIR_DOWN;
				break;
			case DIR_DOWN:
				_dir = DIR_RIGHT;
				break;
			case DIR_RIGHT:
				_dir = DIR_UP;
				break;
			}*/
		}
	}
	// 오른손 법칙 개선
	// 앞서 배운 스택을 이용해 오른손 법칙을 적용한 미로 길찾기 알고리즘을 개선해보자.
	// 길을 찾을 때 막다른 길이면 돌아 나오던 부분을 스택을 이용하면 알 수 있다.
	// 돌아가는 길이 스택의 가장 위에 존재하는 데이터와 같으면 돌아간다는 의미이기 때문이다.이러한 성격을 활용하여 경로 배열을 다듬으면 다음과 같이 개선이 된다.
	// 스택으로 오른쪽으로만 꺾어서 가던 길에서 막힌 길을 되돌아오는 부분을 없앴다.
	// 스택으로 걸어 왔던 길을 스택으로 추적을 해준다. 그리고 가야될 길을 현재 스택 최상위에 있는 원소와 같은지 비교
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
		s.push(_path.back()); // 마지막 좌표를 따로 넣어줌

	vector<Pos> path;
	while (s.empty() == false)
	{
		path.push_back(s.top());
		s.pop();
	}

	std::reverse(path.begin(), path.end());

	_path = path;
}

void Player::Update(uint64 deltaTick)
{
	// 일정 시간마다 경로 업데이트
	if (_pathIndex >= _path.size())
		return;
	
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
