#pragma once

typedef unsigned char Score;

struct PlayerScore
{
	Score firstPlayerScore = 0;
	Score secondPlayerScore = 0;

	void initScore() {
		firstPlayerScore = 0;
		secondPlayerScore = 0;
	}
};
