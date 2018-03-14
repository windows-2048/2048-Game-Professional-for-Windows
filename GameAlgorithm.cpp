#include "gamealgorithm.h"

#include <QStringList>
#include <QDateTime>

#include <cstdlib>
#include <ctime>


GameAlgorithm::GameAlgorithm(int nDim)
	:	m_nSideLen(nDim)
	,	m_nScore(0)
    ,   m_mt19937((unsigned long)QDateTime::currentMSecsSinceEpoch())
{
	clearBoard();

	Point2DArr freeSlots;

	for (int row = 0; row < m_nSideLen; row++)
	{
		for (int col = 0; col < m_nSideLen; col++)
		{
			m_board[row][col] = NULL;
			Point2D p2d(row, col, NULL);
			freeSlots.push_back(p2d);
		}
	}

    putNextNumbersOnBoard(2 * (m_nSideLen / 4) * (m_nSideLen / 4), freeSlots);
}

GameAlgorithm::~GameAlgorithm(void)
{

}

unsigned int GameAlgorithm::defaultValue()
{
    mt19937::result_type nRnd = m_mt19937();

    if ((nRnd % 10) <= 8)
        return DEFAULT_VALUE;
    else
        return DEFAULT_VALUE4;
}

void GameAlgorithm::putNextNumbersOnBoard(int nHowMany, Point2DArr& freeSlots)
{
    unsigned int next = 0;

    for (int n = 0; n < (nHowMany - 1); ++n)
    {
        next = getNextNumberPoint(freeSlots);
        if (next != -1 && next < freeSlots.size())
        {
            Point2D nextNo = freeSlots[next];
            m_board[nextNo.x][nextNo.y] = Board(defaultValue(), true);
            freeSlots.erase(freeSlots.begin() + next);
        }
    }

    next = getNextNumberPoint(freeSlots);
    if (next != -1 && next < freeSlots.size())
    {
        Point2D nextNo = freeSlots[next];
        m_board[nextNo.x][nextNo.y] = Board(defaultValue(), true);
    }
}

void GameAlgorithm::clearBoard()
{
	for (int row = 0; row < SIDE_LEN; ++row)
	{
		for (int col = 0; col < SIDE_LEN; ++col)
		{
			m_board[row][col] = 0;
			m_board[row][col].m = false;
			m_board[row][col].n = false;
		}
	}
}

QString GameAlgorithm::serializeToString() const
{
	QStringList lstImage;

	lstImage << QString("%1").arg(m_nSideLen);

	for (int row = 0; row < m_nSideLen; row++)
	{
		for (int col = 0; col < m_nSideLen; col++)
		{
			lstImage << QString("%1").arg(m_board[row][col]);
		}
	}

	QString strRes = lstImage.join(" ");
	return strRes;
}

void GameAlgorithm::deserializeFromString(const QString strImage)
{
	QStringList lstImage = strImage.split(" ");

	if (lstImage.size() > 1)
	{
		bool bOk = false;
		int nSideLen = lstImage[0].toInt(&bOk);

		if (bOk && ((nSideLen == 4) || (nSideLen == 8) || (nSideLen == 16)))
		{
			if ((lstImage.size() - 1) == (nSideLen * nSideLen))
			{
				m_nSideLen = nSideLen;
				clearBoard();

				for (int row = 0; row < m_nSideLen; row++)
				{
					for (int col = 0; col < m_nSideLen; col++)
					{
						bool bOk = false;
						int nValue = lstImage[1+ (m_nSideLen * row + col)].toInt(&bOk);

						if (bOk)
						{
							m_board[row][col] = (unsigned int)(nValue);
							m_board[row][col].m = false;
							m_board[row][col].n = false;
							m_board[row][col].ids.clear();
							if (m_board[row][col] != ZERO)
								m_board[row][col].ids.push_back(row * m_nSideLen + col);
						}
					}
				}
			}
		}
	}
}

GameAlgorithm::Point2DArr GameAlgorithm::getInitialBoard( void )
{
	Point2DArr board;

	for (int row = 0; row < m_nSideLen; row++)
	{
		for (int col = 0; col < m_nSideLen; col++)
		{
			Point2D p2d(row, col, m_board[row][col]);
			board.push_back(p2d);
		}
	}

	return board;
}

GameAlgorithm::Point2DArrPair GameAlgorithm::updateBoard(GameAlgorithm::Move move)
{
	GameAlgorithm::Point2DArrPair result;

    for (int row = 0; row < m_nSideLen; row++)
    {
        for (int col = 0; col < m_nSideLen; col++)
        {
            m_board[row][col].m = false;
			m_board[row][col].n = false;
			m_board[row][col].ids.clear();
			if (m_board[row][col] != ZERO)
				m_board[row][col].ids.push_back(row * m_nSideLen + col);
        }
    }

	for (int row = 0; row < m_nSideLen; row++)
	{
		for (int col = 0; col < m_nSideLen; col++)
		{
			Point2D p2d(row, col, m_board[row][col]);
			result.first.push_back(p2d);
		}
	}

	switch (move)
	{
	case UP:
		mergeUp();
		moveUp();
		break;
	case DOWN:
		mergeDown();
		moveDown();
		break;
	case LEFT:
		mergerLeft();
		moveLeft();
		break;
	case RIGHT:
		mergeRight();
		moveRight();
		break;
	}

	Point2DArr freeSlots;

	for (int row = 0; row < m_nSideLen; row++)
	{
		for (int col = 0; col < m_nSideLen; col++)
		{
			if (m_board[row][col] == NULL)
			{
				Point2D p2d(row, col, NULL);
				freeSlots.push_back(p2d);
			}
		}
	}

    putNextNumbersOnBoard(1 * (m_nSideLen / 4) * (m_nSideLen / 4), freeSlots);

	for (int row = 0; row < m_nSideLen; row++)
	{
		for (int col = 0; col < m_nSideLen; col++)
		{
			Point2D p2d(row, col, m_board[row][col]);
			result.second.push_back(p2d);
		}
	}

	return result;
}

const int GameAlgorithm::getNextNumberPoint(const Point2DArr& freeSlots)
{
	size_t range = freeSlots.size();

	if (range == 0)
		return -1;

    mt19937::result_type nRnd = m_mt19937();
	return nRnd % range;
}

void GameAlgorithm::moveUp(void)
{
	for (int row = 1; row < m_nSideLen; row++)
	{
		for (int col = 0; col < m_nSideLen; col++)
		{
			if (m_board[row][col] != ZERO)
			{
				for (int i = row; i > 0; i--)
				{
					if (m_board[i-1][col] == ZERO)
					{
						m_board[i-1][col] = m_board[i][col];
						m_board[i][col] = ZERO;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
}

void GameAlgorithm::moveDown(void)
{
	for (int row = m_nSideLen-2; row >= 0; row--)
	{
		for (int col = 0; col < m_nSideLen; col++)
		{
			if (m_board[row][col] != ZERO)
			{
				for (int i = row; i < m_nSideLen-1; i++)
				{
					if (m_board[i+1][col] == ZERO)
					{
						m_board[i+1][col] = m_board[i][col];
						m_board[i][col] = ZERO;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
}

void GameAlgorithm::moveLeft(void)
{
	for (int row = 0; row < m_nSideLen; row++)
	{
		for (int col = 1; col < m_nSideLen; col++)
		{
			if (m_board[row][col] != ZERO)
			{
				for (int i = col; i > 0; i--)
				{
					if (m_board[row][i-1] == ZERO)
					{
						m_board[row][i-1] = m_board[row][i];
						m_board[row][i] = ZERO;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
}

void GameAlgorithm::moveRight(void)
{
	for (int row = 0; row < m_nSideLen; row++)
	{
		for (int col = m_nSideLen-2; col >= 0; col--)
		{
			if (m_board[row][col] != ZERO)
			{
				for (int i = col; i < m_nSideLen-1; i++)
				{
					if (m_board[row][i+1] == ZERO)
					{
						m_board[row][i+1] = m_board[row][i];
						m_board[row][i] = ZERO;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
}

void GameAlgorithm::mergeUp()
{
	for (int row = 1; row < m_nSideLen; row++)
	{
		for (int col = 0; col < m_nSideLen; col++)
		{
			int val = mergePoints(m_board[row][col], m_board[row-1][col]);
			if (val != ZERO)
			{
				m_board[row-1][col] = val;
                m_board[row-1][col].m = true;
				m_board[row-1][col].ids.push_back(m_board[row][col].ids.back());
				m_board[row][col] = ZERO;
				m_nScore+= val;
			}
		}
	}
}

void GameAlgorithm::mergeDown()
{
	for (int row = m_nSideLen-2; row >= 0; row--)
	{
		for (int col = 0; col < m_nSideLen; col++)
		{
			int val = mergePoints(m_board[row][col], m_board[row+1][col]);
			if (val != ZERO)
			{
				m_board[row+1][col] = val;
                m_board[row+1][col].m = true;
				m_board[row+1][col].ids.push_back(m_board[row][col].ids.back());
				m_board[row][col] = ZERO;
				m_nScore+= val;
			}
		}
	}
}

void GameAlgorithm::mergerLeft()
{
	for (int row = 0; row < m_nSideLen; row++)
	{
		for (int col = 1; col < m_nSideLen; col++)
		{
			int val = mergePoints(m_board[row][col], m_board[row][col-1]);
			if (val != ZERO)
			{
				m_board[row][col-1] = val;
                m_board[row][col-1].m = true;
				m_board[row][col-1].ids.push_back(m_board[row][col].ids.back());
				m_board[row][col] = ZERO;
				m_nScore+= val;
			}
		}
	}
}

void GameAlgorithm::mergeRight()
{
	for (int row = 0; row < m_nSideLen; row++)
	{
		for (int col = m_nSideLen-2; col >= 0; col--)
		{
			int val = mergePoints(m_board[row][col], m_board[row][col+1]);
			if (val != ZERO)
			{
				m_board[row][col+1] = val;
                m_board[row][col+1].m = true;
				m_board[row][col+1].ids.push_back(m_board[row][col].ids.back());
				m_board[row][col] = ZERO;
				m_nScore+= val;
			}
		}
	}
}

const unsigned int GameAlgorithm::mergePoints(const unsigned int v1, const unsigned int v2) const
{
	if (v1 != v2)
	{
		return ZERO;
	}
	else
	{
		if (v1 != ZERO)
		{
			unsigned int nV = v1 * 2;
			return nV;
		}
		else
		{
			return ZERO;
		}
	}
}

int GameAlgorithm::score() const
{
	return int(m_nScore);
}

int GameAlgorithm::tile() const
{
	unsigned int nTile = 0;

	for (int row = 0; row < m_nSideLen; row++)
	{
		for (int col = 0; col < m_nSideLen; col++)
		{
			if (m_board[row][col] > nTile)
				nTile = (unsigned int)m_board[row][col];
		}
	}

	return int(nTile);
}

void GameAlgorithm::setScore(int nScore)
{
	m_nScore = (unsigned int)nScore;
}

bool GameAlgorithm::hasStillAChance( void )
{
	for (int row = 0; row < m_nSideLen; row++)
	{
		for (int col = 0; col < m_nSideLen-1; col++)
		{
			if (mergePoints(m_board[row][col], m_board[row][col+1]) != ZERO)
			{
				return true;
			}
		}
	}

	for (int col = 0; col < m_nSideLen; col++)
	{
		for (int row = 0; row < m_nSideLen-1; row++)
		{
			if (mergePoints(m_board[row][col], m_board[row+1][col]) != ZERO)
			{
				return true;
			}
		}
	}

	return false;
}
