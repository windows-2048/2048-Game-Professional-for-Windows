#include "GameField.h"

#include <QPainter>
#include <QApplication>
#include <QMessageBox>
#include <QMainWindow>
#include <QStatusBar>
#include <QDateTime>

#include <algorithm>
#include <cmath>


const int TIMER_ANIMATION_MOVE_MIN = 1 + 1;
const int TIMER_ANIMATION_MOVE_MAX = TIMER_ANIMATION_MOVE_MIN + 10;
const int TIMER_ANIMATION_MERGE_MIN = TIMER_ANIMATION_MOVE_MAX + 1;
const int TIMER_ANIMATION_MERGE_MAX = TIMER_ANIMATION_MERGE_MIN + 5;
const int TIMER_ANIMATION_HAS_MERGED_MIN = TIMER_ANIMATION_MERGE_MAX + 1;
const int TIMER_ANIMATION_HAS_MERGED_MAX = TIMER_ANIMATION_HAS_MERGED_MIN + 5;
const int TIMER_ANIMATION_NEW_MIN = TIMER_ANIMATION_HAS_MERGED_MAX + 1;
const int TIMER_ANIMATION_NEW_MAX = TIMER_ANIMATION_NEW_MIN + 10;
const int TIMER_ANIMATION_WINLOSS_MIN = TIMER_ANIMATION_NEW_MAX + 1;
const int TIMER_ANIMATION_WINLOSS_MAX = TIMER_ANIMATION_WINLOSS_MIN + 10;

GameField::GameField(int nDim, bool bGirlsColors, int nSpacing, QWidget* parent, Qt::WindowFlags f)
    :   QWidget(parent, f)
    ,   m_nDim(nDim)
	,   m_nSpacing(nSpacing)
    ,   m_qMousePressPos(0, 0)
    ,   m_nMouseButton(Qt::NoButton)
    ,   m_mt19937((unsigned long)QDateTime::currentMSecsSinceEpoch())
    ,   m_nTimerCnt(0)
	,	m_state(GOING)
	,	m_bContinueAfterWin(false)
	,	m_bHasStillAChance(true)
	,	m_nBestScore(0)
	,	m_nBestTile(0)
	,	m_bGirlsColors(bGirlsColors)
{
	m_game.reset(new GameAlgorithm(m_nDim));

    for (int row = 0; row < SIDE_LEN; ++row)
    {
        for (int col = 0; col < SIDE_LEN; ++col)
        {
            m_board[row][col] = 0;
        }
    }

	m_kitties << QPixmap(":/MainWindow/Resources/kitties/1.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/2.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/3.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/4.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/5.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/6.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/7.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/8.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/9.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/10.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/11.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/12.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/13.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/14.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/15.jpg");
	m_kitties << QPixmap(":/MainWindow/Resources/kitties/16.jpg");

    startTimer(10);
}

GameField::~GameField()
{

}

void GameField::clearLabels()
{
    for (int row = 0; row < m_nDim; ++row)
    {
        for (int col = 0; col < m_nDim; ++col)
        {
            m_board[row][col] = Board();
			m_board0[row][col] = Board();
        }
    }
}

void GameField::timerAnimateMovePoints()
{
	if (m_nTimerCnt < TIMER_ANIMATION_MOVE_MIN)
	{
		for (int y = 0; y < m_nDim; ++y)
		{
			for (int x = 0; x < m_nDim; ++x)
			{
				m_rectLabelMove[x][y] = QRectF();

				if (m_board0[x][y].ids.size() == 1)
				{
					m_rectLabelMove[x][y] = m_rectLabelBase[x][y];
				}
			}
		}
	}
	else
	{
		if ((m_nTimerCnt >= TIMER_ANIMATION_MOVE_MIN) && (m_nTimerCnt <= TIMER_ANIMATION_MOVE_MAX))
		{
			qreal frac01 = qreal(m_nTimerCnt - TIMER_ANIMATION_MOVE_MIN) / qreal(TIMER_ANIMATION_MOVE_MAX - TIMER_ANIMATION_MOVE_MIN);

			for (int y = 0; y < m_nDim; ++y)
			{
				for (int x = 0; x < m_nDim; ++x)
				{
					m_rectLabelMove[x][y] = QRectF();

					if (m_board0[x][y].ids.size() == 1)
					{
						qreal x0 = m_rectLabelBase[x][y].center().x();
						qreal y0 = m_rectLabelBase[x][y].center().y();
						int id = m_board0[x][y].ids[0];

						for (int y2 = 0; y2 < m_nDim; ++y2)
						{
							for (int x2 = 0; x2 < m_nDim; ++x2)
							{
								if (m_board[x2][y2].ids.size() >= 1)
								{
									if (std::find(m_board[x2][y2].ids.begin(), m_board[x2][y2].ids.end(), id) != m_board[x2][y2].ids.end())
									{
										qreal x1 = m_rectLabelBase[x2][y2].center().x();
										qreal y1 = m_rectLabelBase[x2][y2].center().y();

										qreal x01 = (1.0 - frac01) * x0 + frac01 * x1;
										qreal y01 = (1.0 - frac01) * y0 + frac01 * y1;

										m_rectLabelMove[x][y] = m_rectLabelBase[x][y];
										m_rectLabelMove[x][y].moveCenter(QPointF(x01, y01));
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			for (int y = 0; y < m_nDim; ++y)
			{
				for (int x = 0; x < m_nDim; ++x)
				{
					m_rectLabelMove[x][y] = QRectF();
					/*
					if (m_board0[x][y].ids.size() == 1)
					{
						qreal x0 = m_rectLabelBase[x][y].center().x();
						qreal y0 = m_rectLabelBase[x][y].center().y();
						int id = m_board0[x][y].ids[0];

						for (int y2 = 0; y2 < m_nDim; ++y2)
						{
							for (int x2 = 0; x2 < m_nDim; ++x2)
							{
								if (m_board[x2][y2].ids.size() >= 1)
								{
									if (std::find(m_board[x2][y2].ids.begin(), m_board[x2][y2].ids.end(), id) != m_board[x2][y2].ids.end())
									{
										m_rectLabelMove[x][y] = m_rectLabelBase[x2][y2];
									}
								}
							}
						}
					}
					*/
				}
			}
		}
	}
}

void GameField::timerAnimateMergedPoints()
{
	if (m_nTimerCnt < TIMER_ANIMATION_MERGE_MIN)
	{
		for (int y = 0; y < m_nDim; ++y)
		{
			for (int x = 0; x < m_nDim; ++x)
			{
				m_rectLabelMerged[x][y] = QRectF();
			}
		}
	}
	else
	{
		if ((m_nTimerCnt >= TIMER_ANIMATION_MERGE_MIN) && (m_nTimerCnt <= TIMER_ANIMATION_MERGE_MAX))
		{
			qreal frac01 = qreal(m_nTimerCnt - TIMER_ANIMATION_MERGE_MIN) / qreal(TIMER_ANIMATION_MERGE_MAX - TIMER_ANIMATION_MERGE_MIN);

			for (int y = 0; y < m_nDim; ++y)
			{
				for (int x = 0; x < m_nDim; ++x)
				{
					if (!m_board[x][y].n)
					{
						m_rectLabelMerged[x][y] = m_rectLabelBase[x][y];

						if (m_board[x][y].m)
						{
							m_rectLabelMerged[x][y] = m_rectLabelBase[x][y];

							QColor color = colorForValue(m_board[x][y]);
							QColor color2 = colorForValue(m_board[x][y] / 2);
							QColor color3 = (m_mt19937() % 2 == 0) ? colorForValue(m_board[x][y] * 2) : QColor("White");

							QRadialGradient qGrad(m_rectLabelMerged[x][y].center(), m_rectLabelMerged[x][y].width() / 2);

							qGrad.setCoordinateMode(QGradient::LogicalMode);
							qGrad.setSpread(QGradient::PadSpread);

							for (int nAngle = 0; nAngle < 360; nAngle+= 36)
							{
                                QColor color23 = (m_mt19937() % 2 == 0) ? color2 : color3;
								QColor curColor =  (m_mt19937() % (m_nTimerCnt - TIMER_ANIMATION_MERGE_MIN + 1) == 0) ? color23 : color;
								qGrad.setColorAt(qreal(nAngle) / qreal(360), curColor);
							}

							QBrush qBrush(qGrad);

							m_brushLabelMerged[x][y] = qBrush;
						}
						else
						{
							m_brushLabelMerged[x][y] = brushForValue(m_board[x][y], m_rectLabelMerged[x][y]);
						}
					}
				}
			}
		}
		else
		{
			if ((m_nTimerCnt >= TIMER_ANIMATION_HAS_MERGED_MIN) && (m_nTimerCnt <= TIMER_ANIMATION_HAS_MERGED_MAX))
			{
				for (int y = 0; y < m_nDim; ++y)
				{
					for (int x = 0; x < m_nDim; ++x)
					{
						if (!m_board[x][y].n)
						{
							m_rectLabelMerged[x][y] = m_rectLabelBase[x][y];

							if (m_board[x][y].m)
							{
								m_rectLabelMerged[x][y].setWidth(m_rectLabelBase[x][y].width() * 1.25);
								m_rectLabelMerged[x][y].setHeight(m_rectLabelBase[x][y].height() * 1.25);
								m_rectLabelMerged[x][y].moveCenter(m_rectLabelBase[x][y].center());
							}

							m_brushLabelMerged[x][y] = brushForValue(m_board[x][y], m_rectLabelMerged[x][y]);
						}
					}
				}
			}
			else
			{
				for (int y = 0; y < m_nDim; ++y)
				{
					for (int x = 0; x < m_nDim; ++x)
					{
						if (!m_board[x][y].n)
						{
							m_rectLabelMerged[x][y] = m_rectLabelBase[x][y];
							m_brushLabelMerged[x][y] = brushForValue(m_board[x][y], m_rectLabelMerged[x][y]);
						}
					}
				}
			}
		}
	}
}

void GameField::timerAnimateNewPoints()
{
	if (m_nTimerCnt < TIMER_ANIMATION_NEW_MIN)
	{
		for (int y = 0; y < m_nDim; ++y)
		{
			for (int x = 0; x < m_nDim; ++x)
			{
				m_rectLabelNew[x][y] = QRectF();

				if (m_board[x][y].n)
				{
					m_rectLabelNew[x][y] = m_rectLabelBase[x][y];
					m_rectLabelNew[x][y].setWidth(0);
					m_rectLabelNew[x][y].setHeight(0);
					m_rectLabelNew[x][y].moveCenter(m_rectLabelBase[x][y].center());
				}
			}
		}
	}
	else
		if ((m_nTimerCnt >= TIMER_ANIMATION_NEW_MIN) && (m_nTimerCnt <= TIMER_ANIMATION_NEW_MAX))
		{
			qreal frac01 = qreal(m_nTimerCnt - TIMER_ANIMATION_NEW_MIN) / qreal(TIMER_ANIMATION_NEW_MAX - TIMER_ANIMATION_NEW_MIN);

			for (int y = 0; y < m_nDim; ++y)
			{
				for (int x = 0; x < m_nDim; ++x)
				{
					m_rectLabelNew[x][y] = QRectF();

					if (m_board[x][y].n)
					{
						m_rectLabelNew[x][y] = m_rectLabelBase[x][y];
						m_rectLabelNew[x][y].setWidth(m_rectLabelBase[x][y].width() * frac01);
						m_rectLabelNew[x][y].setHeight(m_rectLabelBase[x][y].height() * frac01);
						m_rectLabelNew[x][y].moveCenter(m_rectLabelBase[x][y].center());
					}
				}
			}
		}
		else
		{
			for (int y = 0; y < m_nDim; ++y)
			{
				for (int x = 0; x < m_nDim; ++x)
				{
					m_rectLabelNew[x][y] = QRectF();

					if (m_board[x][y].n)
					{
						m_rectLabelNew[x][y] = m_rectLabelBase[x][y];
					}
				}
			}
		}
}

void GameField::timerAnimateWinLoss()
{
	if (m_nTimerCnt < TIMER_ANIMATION_WINLOSS_MIN)
	{
		// Do nothing
	}
	else
		if ((m_nTimerCnt >= TIMER_ANIMATION_WINLOSS_MIN) && (m_nTimerCnt < TIMER_ANIMATION_WINLOSS_MAX))
		{
			// TODO: do nothing for a while
		}
		else
			if (m_nTimerCnt == TIMER_ANIMATION_WINLOSS_MAX)
			{
                if ((m_state == WIN) && (m_bContinueAfterWin == false))
				{
					QMessageBox msgBox(this);
					msgBox.setModal(true);
					msgBox.addButton(QMessageBox::Close);
					msgBox.setWindowTitle("You win - 2048 Game Professional");
                    msgBox.setTextFormat(Qt::RichText);
					msgBox.setText(tr("<font color='HotPink'><b>2048 Game Professional</b></font><br/><font color='Lime'>congratulates! You win!</font><br/><br/><img src=':/MainWindow/Resources/win_300.png'/>"));

					QIcon icon;
					icon.addFile(":/MainWindow/Resources/favicon_16.png", QSize(16, 16), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/favicon_24.png", QSize(24, 24), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/favicon_32.png", QSize(32, 32), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(48, 48), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(64, 64), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(96, 96), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(128, 128), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(256, 256), QIcon::Normal, QIcon::Off);
					msgBox.setWindowIcon(icon);

					msgBox.exec();

					m_bContinueAfterWin = true;
				}
				else
				if ((m_state == LOSS))
				{
					QMessageBox msgBox(this);
					msgBox.setModal(true);
					msgBox.addButton(QMessageBox::Close);
					msgBox.setWindowTitle("You lose - 2048 Game Professional");
					msgBox.setText(tr("<font color='Gainsboro'>2048 Game Professional<br/>apologizes. You lose. Try Again...</font><br/><br/><img src=':/MainWindow/Resources/lose_300.png'/>"));

					QIcon icon;
					icon.addFile(":/MainWindow/Resources/favicon_16.png", QSize(16, 16), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/favicon_24.png", QSize(24, 24), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/favicon_32.png", QSize(32, 32), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(48, 48), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(64, 64), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(96, 96), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(128, 128), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(256, 256), QIcon::Normal, QIcon::Off);
					msgBox.setWindowIcon(icon);

					msgBox.exec();
				}
				else
				if ((m_state == WIN) && (m_bHasStillAChance == false))
				{
					QMessageBox msgBox(this);
					msgBox.setModal(true);
					msgBox.addButton(QMessageBox::Close);
					msgBox.setWindowTitle("You win - 2048 Game Professional");
					msgBox.setText(tr("<font color='HotPink'><b>2048 Game Professional</b></font><br/><font color='Lime'>congratulates! <big>You win</font> <font color='#fd97a2'><big><b>%1</b></big></font> <font color='Lime'>!</font></big><br/><br/><img src=':/MainWindow/Resources/win_300.png'/>").arg(game()->score()));

					QIcon icon;
					icon.addFile(":/MainWindow/Resources/favicon_16.png", QSize(16, 16), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/favicon_24.png", QSize(24, 24), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/favicon_32.png", QSize(32, 32), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(48, 48), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(64, 64), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(96, 96), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(128, 128), QIcon::Normal, QIcon::Off);
					icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(256, 256), QIcon::Normal, QIcon::Off);
					msgBox.setWindowIcon(icon);

					msgBox.exec();
				}

				if ((m_state == LOSS) || ((m_state == WIN) && (m_bHasStillAChance == false)))
				{
					newGame();
				}
			}
}

void GameField::timerEvent(QTimerEvent* pQTimerEvent)
{
    ++m_nTimerCnt;

	timerAnimateMovePoints();
	timerAnimateMergedPoints();
	timerAnimateNewPoints();
	timerAnimateWinLoss();

	if (m_nTimerCnt <= (TIMER_ANIMATION_WINLOSS_MAX + 1))
		repaint();
}

QBrush GameField::brushForValue(int val, const QRectF& rect)
{
	QColor color = colorForValue(val);

	QRadialGradient qGrad(rect.center(), (rect.width() / 2.0) * sqrt(2.0));

	qGrad.setCoordinateMode(QGradient::LogicalMode);
	qGrad.setSpread(QGradient::PadSpread);

	qGrad.setColorAt(0.00, color.darker(110));
	qGrad.setColorAt(0.40, color);
	qGrad.setColorAt(0.90, color);
	qGrad.setColorAt(1.00, color.darker(125));

	QBrush qBrush(qGrad);

    return qBrush;
}

void GameField::resizeEvent(QResizeEvent* pEvent)
{
	m_nTimerCnt = 0; // Restart animation

	qreal dw = (pEvent->size().width() - pEvent->size().height()) / 2.0;

	m_topLeft = QPointF();

	if (dw >= 0)
	{
		m_topLeft.setX(dw);
	}
	else
	{
		m_topLeft.setY(-dw);
	}

	qreal wTotal = size().width() < size().height() ? size().width() : size().height();
	qreal wLabel = (wTotal - (m_nDim + 1) * m_nSpacing) / m_nDim;
	qreal wLabelwSpacing = wLabel + m_nSpacing;

	for (int y = 0; y < m_nDim; ++y)
	{
		for (int x = 0; x < m_nDim; ++x)
		{
			m_rectLabelBase[x][y] = QRectF();
			m_rectLabelBase[x][y].setTopLeft(QPointF(x * wLabelwSpacing, y * wLabelwSpacing));
			m_rectLabelBase[x][y].setWidth(wLabel);
			m_rectLabelBase[x][y].setHeight(wLabel);

			m_rectLabelMove[x][y] = QRectF();

			if (m_board0[x][y].ids.size() == 1)
			{
				m_rectLabelMove[x][y] = m_rectLabelBase[x][y];
			}

			m_rectLabelNew[x][y] = QRectF();

			if (m_board[x][y].n)
			{
				m_rectLabelNew[x][y] = m_rectLabelBase[x][y];
				m_rectLabelNew[x][y].setWidth(0);
				m_rectLabelNew[x][y].setHeight(0);
				m_rectLabelNew[x][y].moveCenter(m_rectLabelBase[x][y].center());
			}

			m_rectLabelMerged[x][y] = QRectF();

			if (m_board[x][y].m)
			{
				m_brushLabelMerged[x][y] = brushForValue(m_board[x][y] / 2, m_rectLabelBase[x][y]);
			}
		}
	}

	repaint();
}

void GameField::paintRectAndValue(QPainter& qpainter, const QRectF& rectLabel, const QBrush& qbrush, const Board& board)
{
	if ((rectLabel != QRectF()) && (board.v != 0))
	{
		QPen qPen(Qt::NoPen);
		qpainter.setPen(qPen);

		qpainter.setBrush(qbrush);

		qpainter.drawRoundedRect(rectLabel, m_nSpacing, m_nSpacing);

        if (rectLabel.width() < 4)
            return;

		QPen qPenT;
		qPenT.setColor(textColorForValue(board.v, board.n));
		qPenT.setWidthF(2.0);
		qpainter.setPen(qPenT);

		QFont qFnt = QApplication::font();

		qFnt.setPixelSize(int(rectLabel.width() / 2.0));

		if (board.v >= (1 << 7))
		{
			qFnt.setPixelSize(int(rectLabel.width() / 2.5));
		}

		if (board.v >= (1 << 10))
		{
			qFnt.setPixelSize(int(rectLabel.width() / 3.0));
		}

		if (board.v >= (1 << 14))
		{
			qFnt.setPixelSize(int(rectLabel.width() / 3.5));
		}

		if (board.v >= (1 << 17))
		{
			qFnt.setPixelSize(int(rectLabel.width() / 4.0));
		}

		qpainter.setFont(qFnt);

		qpainter.drawText(rectLabel, Qt::AlignCenter, QString("%1").arg(board.v));
	}
}

void GameField::paintEvent(QPaintEvent* pEvent)
{
    int nFntPixSz = int(m_rectLabelBase[0][0].width() / 2.0);
    if (nFntPixSz < 6)
        return;

    QPainter qpainter(this);
    qpainter.setRenderHint(QPainter::Antialiasing);

	QRadialGradient qGradBg(size().width() / 2, size().height() / 2, size().height() < size().width() ? size().height() : size().width());

	qGradBg.setCoordinateMode(QGradient::LogicalMode);
	qGradBg.setSpread(QGradient::PadSpread);

	qGradBg.setColorAt(0.0, QColor("#555"));
	qGradBg.setColorAt(0.5, QColor("#444"));
	qGradBg.setColorAt(0.75, QColor("#333"));
	qGradBg.setColorAt(1.0, QColor("#111"));

	QBrush qBrushBg(qGradBg);
	qpainter.fillRect(0, 0, size().width(), size().height(), qBrushBg);

	QFont qFnt = QApplication::font();
    //qFnt.setFamily("Arial");
    qFnt.setPixelSize(nFntPixSz);
    qFnt.setStyleStrategy(QFont::PreferAntialias);
    qpainter.setFont(qFnt);

	QPen qPen(Qt::NoPen);
	qpainter.setPen(qPen);

	QBrush qbrush(Qt::SolidPattern);
	qbrush.setColor(colorForValue(0));
	qpainter.setBrush(qbrush);

	qpainter.save();
	qpainter.translate(m_topLeft);
	qpainter.translate(QPointF(m_nSpacing, m_nSpacing));

	for (int y = 0; y < m_nDim; ++y)
	{
		for (int x = 0; x < m_nDim; ++x)
		{
			qpainter.drawRoundedRect(m_rectLabelBase[x][y], m_nSpacing, m_nSpacing);
		}
	}

    for (int y = 0; y < m_nDim; ++y)
    {
        for (int x = 0; x < m_nDim; ++x)
        {//m_board[x][y] = (1 << (y * m_nDim + x + 1)); // Test colors
			if (m_rectLabelMove[x][y] != QRectF())
			{
				paintRectAndValue(qpainter, m_rectLabelMove[x][y], brushForValue(m_board0[x][y], m_rectLabelMove[x][y]), m_board0[x][y]);
			}

			if (m_rectLabelMerged[x][y] != QRectF())
			{
				paintRectAndValue(qpainter, m_rectLabelMerged[x][y], m_brushLabelMerged[x][y], m_board[x][y]);
			}

			if (m_rectLabelNew[x][y] != QRectF())
			{
				paintRectAndValue(qpainter, m_rectLabelNew[x][y], brushForValue(m_board[x][y], m_rectLabelNew[x][y]), m_board[x][y]);
			}
        }
    }

	qpainter.restore();

	paintScoreKitty(qpainter, game()->score(), game()->tile(), false);
	paintScoreKitty(qpainter, m_nBestScore, m_nBestTile, true);
}

void GameField::setBestScore(int nScore)
{
	m_nBestScore = nScore;
}

void GameField::setBestTile(int nTile)
{
	m_nBestTile = nTile;
}

void GameField::setThemeGirls(bool bGirls)
{
	m_bGirlsColors = bGirls;
	update();
}

int GameField::kittyIndexForTile(int nTile)
{
	if (nTile < 128 * (m_nDim * m_nDim / 16))
		return 0;

	if (nTile < 256 * (m_nDim * m_nDim / 16))
		return 1;

	if (nTile < 512 * (m_nDim * m_nDim / 16))
		return 2;

	if (nTile < 1024 * (m_nDim * m_nDim / 16))
		return 3;

	if (nTile < 2048 * (m_nDim * m_nDim / 16))
		return 4;

	if (nTile < 4096 * (m_nDim * m_nDim / 16))
		return 5;

	if (nTile < 8192 * (m_nDim * m_nDim / 16))
		return 6;

	if (nTile < 16384 * (m_nDim * m_nDim / 16))
		return 7;

	if (nTile < 32768 * (m_nDim * m_nDim / 16))
		return 8;

	if (nTile < 65536 * (m_nDim * m_nDim / 16))
		return 9;

	if (nTile < 131072 * (m_nDim * m_nDim / 16))
		return 10;

	if (nTile < 262144 * (m_nDim * m_nDim / 16))
		return 11;

	if (nTile < 524288 * (m_nDim * m_nDim / 16))
		return 12;

	if (nTile < 1048576 * (m_nDim * m_nDim / 16))
		return 13;

	if (nTile < 2097152 * (m_nDim * m_nDim / 16))
		return 14;

	if (nTile < 4194304 * (m_nDim * m_nDim / 16))
		return 15;

	return 15;
}

void GameField::paintScoreKitty(QPainter& qpainter, int nScore, int nTile, bool bBest)
{
	qreal fMinPixmapWidth = 64;
	qreal fMaxPixmapWidth = m_topLeft.x()- m_nSpacing * 2;

	if (fMaxPixmapWidth < 150)
		return; // Won't paint at all

	if (fMaxPixmapWidth > 256)
		fMaxPixmapWidth = 256;

	qreal fPixmapWidth = fMinPixmapWidth + (fMaxPixmapWidth - fMinPixmapWidth) * tanh(qreal(nScore) / qreal(10000) / (qreal(m_nDim * m_nDim * m_nDim *m_nDim / 256)));

	qpainter.setPen(Qt::NoPen);

	QRectF scoreRect;
	scoreRect.setWidth(fPixmapWidth + m_nSpacing * 2);
	scoreRect.setHeight(fPixmapWidth + m_nSpacing * 2);
	QPointF lcenter;
	lcenter.setX(bBest ? (size().width() - m_topLeft.x() / 2) : m_topLeft.x() / 2);
	lcenter.setY(size().height() / 2);
	scoreRect.moveCenter(lcenter);

	qpainter.drawRoundedRect(scoreRect, m_nSpacing, m_nSpacing);
	QRectF scoreRect2; //fPixmapWidth = 256;
	scoreRect2.setWidth(fPixmapWidth);
	scoreRect2.setHeight(fPixmapWidth);
	scoreRect2.moveCenter(scoreRect.center()); //nTile = 2097152 * (m_nDim * m_nDim / 16);
	int nAtIdx = kittyIndexForTile(nTile);
	qpainter.drawPixmap(scoreRect2, m_kitties.at(nAtIdx), QRectF(QPoint(0, 0), m_kitties.at(nAtIdx).size()));

	QFont qFnt = QApplication::font();
	//qFnt.setFamily("Arial");
	qFnt.setPixelSize(30);
	qFnt.setStyleStrategy(QFont::PreferAntialias);
	qpainter.setFont(qFnt);

	QPen qPenT;
	qPenT.setColor(bBest ? "DeepPink" : "Lime");
	qPenT.setWidthF(2.0);
	qpainter.setPen(qPenT);

	QRectF scoreTextRect = scoreRect;
	scoreTextRect.setWidth(256);
	scoreTextRect.setHeight(32);
	scoreTextRect.moveCenter(scoreRect.center());
	scoreTextRect.moveBottom(scoreRect.top());

	qpainter.drawText(scoreTextRect, Qt::AlignCenter, QString("%1").arg(nScore));

	qFnt.setPixelSize(20);
	qpainter.setFont(qFnt);

	qPenT.setColor("White");
	qpainter.setPen(qPenT);

	QRectF tileTextRect = scoreRect;
	tileTextRect.setWidth(256);
	tileTextRect.setHeight(32);
	tileTextRect.moveCenter(scoreRect.center());
	tileTextRect.moveTop(scoreRect.bottom());

	qpainter.drawText(tileTextRect, Qt::AlignCenter, QString("%1").arg(nTile));
}

GameField::GameState GameField::updateBoard(const GameAlgorithm::Point2DArrPair& board)
{
	m_nTimerCnt = 0;

	if ((board.first.size() == m_nDim * m_nDim) && (board.second.size() == m_nDim * m_nDim))
	{
		clearLabels();

		for (size_t i = 0; i < board.second.size(); ++i)
		{
			m_board[board.second[i].y][board.second[i].x] = board.second[i].b;
			m_board0[board.first[i].y][board.first[i].x] = board.first[i].b;
		}

		QResizeEvent qRE(size(), size());
		QCoreApplication::sendEvent(this, &qRE);
	}

	emit fieldChanged(game()->score());

	GameField::GameState state = checkWinLoss(board.second);

	return state;
}

GameField::GameState GameField::checkWinLoss(const GameAlgorithm::Point2DArr& board)
{
	GameField::GameState result = GOING;

	unsigned int noFreeSlots = 0;

	for (size_t i = 0; i < board.size(); i++)
	{
		if (board[i].b.v == ZERO)
		{
			++noFreeSlots;
		}

		if (board[i].b.v >= (TARGET * (m_nDim * m_nDim / 16))) // Significant >= in case user continue gaming after win TARGET
		{
			result = WIN;
		}
	}

	if (noFreeSlots == 0 && !(game()->hasStillAChance()))
	{
		if (result != WIN)
		{
			result = LOSS;
		}

		m_bHasStillAChance = false;
	}

	return result;
}

void GameField::keyPressEvent(QKeyEvent* pEvent)
{
	m_state = GOING;

	switch (pEvent->key())
	{
	case Qt::Key_Up:
		m_state = updateBoard(game()->updateBoard(GameAlgorithm::UP));
		break;
	case Qt::Key_Down:
		m_state = updateBoard(game()->updateBoard(GameAlgorithm::DOWN));
		break;
	case Qt::Key_Left:
		m_state = updateBoard(game()->updateBoard(GameAlgorithm::LEFT));
		break;
	case Qt::Key_Right:
		m_state = updateBoard(game()->updateBoard(GameAlgorithm::RIGHT));
		break;
    case Qt::Key_Escape:
        parentWidget()->parentWidget()->close();
        break;
	}
}

void GameField::mousePressEvent(QMouseEvent* pQMouseEvent)
{
    m_qMousePressPos = pQMouseEvent->pos();
    m_nMouseButton = pQMouseEvent->button();
}

void GameField::mouseReleaseEvent(QMouseEvent* pQMouseEvent)
{
    m_state = GOING;

    QPoint qMousePos = pQMouseEvent->pos();
    Qt::MouseButton nMouseButton = pQMouseEvent->button();

    if ((m_nMouseButton == Qt::LeftButton) && (nMouseButton == m_nMouseButton) && (m_qMousePressPos != qMousePos))
    {
        QPoint qDeltaPos = qMousePos - m_qMousePressPos;

        if (qDeltaPos.x() > 0)
        {
            qreal fTg = qreal(qDeltaPos.y()) / qreal(qDeltaPos.x());

            if (abs(fTg) < 0.5)
            {
                m_state = updateBoard(game()->updateBoard(GameAlgorithm::RIGHT));
            }
        }

        if (qDeltaPos.x() < 0)
        {
            qreal fTg = qreal(qDeltaPos.y()) / qreal(-qDeltaPos.x());

            if (abs(fTg) < 0.5)
            {
                m_state = updateBoard(game()->updateBoard(GameAlgorithm::LEFT));
            }
        }

        if (qDeltaPos.y() > 0)
        {
            qreal fTg = qreal(qDeltaPos.x()) / qreal(qDeltaPos.y());

            if (abs(fTg) < 0.5)
            {
                m_state = updateBoard(game()->updateBoard(GameAlgorithm::DOWN));
            }
        }

        if (qDeltaPos.y() < 0)
        {
            qreal fTg = qreal(qDeltaPos.x()) / qreal(-qDeltaPos.y());

            if (abs(fTg) < 0.5)
            {
                m_state = updateBoard(game()->updateBoard(GameAlgorithm::UP));
            }
        }
    }
}

void GameField::updateGame()
{
	GameAlgorithm::Point2DArrPair pair;
	pair.second = m_game->getInitialBoard();
	pair.first = pair.second;
	m_state = updateBoard(pair);
}

void GameField::newGame()
{
	m_game.reset(new GameAlgorithm(m_nDim));
	m_bHasStillAChance = true; // Only in new game we can set this
	updateGame();
	m_bContinueAfterWin = false;
	qobject_cast<QMainWindow*>(parentWidget()->parentWidget())->statusBar()->showMessage(tr("Start new game"), 3000);
}

QColor GameField::colorForValue(int val) const
{
	if (m_bGirlsColors)
	{
		switch (val)
		{
		case 0: return QColor("#999");
		case (1 << 1): return QColor("Ivory");
		case (1 << 2): return QColor("LemonChiffon");
		case (1 << 3): return QColor("PapayaWhip");
		case (1 << 4): return QColor("Moccasin");
		case (1 << 5): return QColor("LightPink");
		case (1 << 6): return QColor("SandyBrown");
		case (1 << 7): return QColor("Goldenrod");
		case (1 << 8): return QColor("Chocolate");
		case (1 << 9): return QColor("Sienna");
		case (1 << 10): return QColor("Brown");
		case (1 << 11): return QColor("DeepPink");
		case (1 << 12): return QColor("LimeGreen");
		case (1 << 13): return QColor("MediumSeaGreen");
		case (1 << 14): return QColor("Teal");
		case (1 << 15): return QColor("SteelBlue");
		case (1 << 16): return QColor("RoyalBlue");
		}

		int drk = int(0 + log(double(val - (1 << 16))) * 10.0);
		return QColor("BlueViolet").darker(drk);
	}
	else
	{
		switch (val)
		{
		case 0: return QColor("#bbada0").darker(115);
		case (1 << 1): return QColor("#eee4da");
		case (1 << 2): return QColor("#ede0c8");
		case (1 << 3): return QColor("#f2b179");
		case (1 << 4): return QColor("#f59563");
		case (1 << 5): return QColor("#f67c5f");
		case (1 << 6): return QColor("#f65e3b");
		case (1 << 7): return QColor("#edcf72");
		case (1 << 8): return QColor("#edcc61");
		case (1 << 9): return QColor("#edc850");
		case (1 << 10): return QColor("#edc53f");
		case (1 << 11): return QColor("#edc22e");
		case (1 << 12): return QColor("RoyalBlue");
		case (1 << 13): return QColor("SteelBlue");
		case (1 << 14): return QColor("Teal");
		case (1 << 15): return QColor("MediumSeaGreen");
		case (1 << 16): return QColor("LimeGreen");
		}

		int drk = int(0 + log(double(val - (1 << 16))) * 10.0);
		return QColor("Magenta").darker(drk);
	}
}

QColor GameField::textColorForValue(int val, bool newp) const
{
	if (m_bGirlsColors)
	{
		if (newp)
		{
			return val < (1 << 6) ? QColor("ForestGreen") : (val < (1 << 11) ? QColor("SpringGreen") : QColor("LightCyan"));
		}

		return val < (1 << 6) ? QColor("Black") : QColor("White");
	}
	else
	{
		if (newp)
		{
			return (val < (1 << 3) || val > (1 << 6)) ? QColor("#060") : QColor("#cfc");
		}

		return (val < (1 << 3) || val > (1 << 6)) ? QColor("#000") : QColor("#fff");
	}
}
