#pragma once

#include "GameAlgorithm.h"

#include <QPixmap>
#include <QWidget>
#include <QPaintEvent>

#include <random>
using namespace ::std::tr1;


class GameField: public QWidget
{
    Q_OBJECT

public:

    explicit GameField(int nDim, bool bGirlsColors = true, int nSpacing = 4, QWidget* parent = 0, Qt::WindowFlags f = 0);
    virtual ~GameField();

public:

	GameAlgorithm* game()
	{
		return m_game.data();
	}

public slots:

	void updateGame();
	void newGame();
	void setBestScore(int nScore);
	void setBestTile(int nTile);
	void setThemeGirls(bool);

signals:

	void fieldChanged(int);

protected:

	void resizeEvent(QResizeEvent* pEvent); // We re-implement from Qt
    void paintEvent(QPaintEvent* pEvent); // We re-implement from Qt
	void keyPressEvent(QKeyEvent* pEvent); // We re-implement from Qt
    void mousePressEvent(QMouseEvent* pQMouseEvent); // We re-implement from Qt
    void mouseReleaseEvent(QMouseEvent* pQMouseEvent); // We re-implement from Qt
    void timerEvent(QTimerEvent* pQTimerEvent); // We re-implement from Qt
	void clearLabels();
    QColor colorForValue(int val) const;
    QColor textColorForValue(int val, bool newp) const;
    QBrush brushForValue(int val, const QRectF& rect);
	void timerAnimateMovePoints();
	void timerAnimateMergedPoints();
	void timerAnimateNewPoints();
	void timerAnimateWinLoss();
	void paintRectAndValue(QPainter& qpainter, const QRectF& rectLabel, const QBrush& qbrush, const Board& board);
	void paintScoreKitty(QPainter& qpainter, int nScore, int nTile, bool bBest);
	int kittyIndexForTile(int nTile);

private:

    int m_nDim;
    Board m_board[SIDE_LEN][SIDE_LEN];
	Board m_board0[SIDE_LEN][SIDE_LEN];
    int m_nSpacing;

	enum GameState
	{
		WIN,
		LOSS,
		GOING
	};

	GameState m_state;
	QScopedPointer<GameAlgorithm> m_game;
	GameState updateBoard(const GameAlgorithm::Point2DArrPair& board);
	GameState checkWinLoss(const GameAlgorithm::Point2DArr& board);
	bool m_bContinueAfterWin;
	bool m_bHasStillAChance;

    QPoint m_qMousePressPos;
    Qt::MouseButton m_nMouseButton;
    mt19937 m_mt19937;
    int m_nTimerCnt;

	QPointF m_topLeft;
	QRectF m_rectLabelBase[SIDE_LEN][SIDE_LEN];
	QRectF m_rectLabelMove[SIDE_LEN][SIDE_LEN];
	QRectF m_rectLabelMerged[SIDE_LEN][SIDE_LEN];
	QRectF m_rectLabelNew[SIDE_LEN][SIDE_LEN];
	QBrush m_brushLabelMerged[SIDE_LEN][SIDE_LEN];

	QList<QPixmap> m_kitties;

	int m_nBestScore;
	int m_nBestTile;
	bool m_bGirlsColors;
};
