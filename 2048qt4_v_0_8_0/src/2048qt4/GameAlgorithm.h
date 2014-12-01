#ifndef GAMEALGORITHM_H
#define GAMEALGORITHM_H

#include <QString>

#include <vector>
#include <random>
using namespace ::std::tr1;


const int SIDE_LEN	= 64;
const unsigned int DEFAULT_VALUE = 2;
const unsigned int DEFAULT_VALUE4 = 4;
const unsigned int ZERO = 0;
const unsigned int TARGET = 2048; // 16 for test purposes


struct Board
{
    unsigned int v;
    bool n;
    bool m;
	std::vector<int> ids;

    Board(unsigned int val = NULL, bool newp = false, bool merged = false)
        :   v(val)
        ,   n(newp)
        ,   m(merged)
		,	ids()			
    {

    }

    Board(const Board& b)
        :   v(b.v)
        ,   n(b.n)
        ,   m(b.m)
		,	ids(b.ids)
    {

    }

	const Board& operator = (unsigned int val)
	{
		v = val;
		if (val == ZERO)
			ids.clear();
		return *this;
	}

    const Board& operator = (const Board& b)
    {
        v = b.v;
        n = b.n;
        m = b.m;
		ids = b.ids;
        return *this;
    }

    operator unsigned int () const
    {
        return v;
    }
};

class GameAlgorithm
{
public:

	explicit GameAlgorithm(int nDim);
	~GameAlgorithm();

	QString serializeToString() const;
	void deserializeFromString(const QString strImage);
	int score() const;
	void setScore(int nScore);

	struct Point2D
	{
		unsigned int x;
		unsigned int y;
		Board b;

		Point2D()
			:	x(0)
			,	y(0)
			,	b(NULL, false, false)
		{
		}

		Point2D(unsigned int cx, unsigned int cy, const Board& cb)
			:	x(cx)
			,	y(cy)
			,	b(cb)
		{
		}
	};

	typedef std::vector<Point2D> Point2DArr;
	typedef std::pair<Point2DArr, Point2DArr> Point2DArrPair;

	enum Move
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	Point2DArr getInitialBoard(void);
	Point2DArrPair updateBoard(Move move);
	bool hasStillAChance(void);

protected:

    void putNextNumbersOnBoard(int nHowMany, Point2DArr& freeSlots);

private:

	const int getNextNumberPoint(const Point2DArr& freeSlots);
	void moveUp(void);
	void moveDown(void);
	void moveLeft(void);
	void moveRight(void);
	void mergeUp(void);
	void mergeDown(void);
	void mergerLeft(void);
	void mergeRight(void);
	void clearBoard();
	const unsigned int mergePoints(const unsigned int v1, const unsigned int v2) const;
    unsigned int defaultValue();

	Board m_board[SIDE_LEN][SIDE_LEN];
	int m_nSideLen;
	unsigned int m_nScore;
    mt19937 m_mt19937;
};

#endif // GAMEALGORITHM_H