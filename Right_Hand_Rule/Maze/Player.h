#pragma once

class Board;

class Player
{
	enum 
	{
		// 0.1초 단위로 이동시켜주기 위해 MOVE_TICK을 100으로 설정
		MOVE_TICK = 100,
	};

public:
	void		Init(Board* board);
	void		Update(uint64 deltaTick);

	void		SetPos(Pos pos) { _pos = pos; }
	Pos			GetPos() { return _pos; }

	bool CanGo(Pos pos);

private:
	void		RightHand();
	void		Bfs();
	void	    AStar();
private:
	Pos			_pos = {};
	int32		_dir = DIR_UP;
	Board* _board = nullptr;

	vector<Pos> _path;
	// 경로를 기준으로 어디까지 이동했는지 추적하는 용도
	uint32 _pathIndex = 0;
	uint64 _sumTick = 0;
};

