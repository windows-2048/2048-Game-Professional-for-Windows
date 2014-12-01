#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QLineEdit>


class ScoreBar: public QWidget
{
	Q_OBJECT

public:

	ScoreBar(QWidget* parent = 0);
	virtual ~ScoreBar();

	QLineEdit* scoreEdit()
	{
		return m_pScoreEdit;
	}

	QLineEdit* bestEdit()
	{
		return m_pBestEdit;
	}

private:

	QHBoxLayout* m_pLayout;
	QSpacerItem* m_pSpacer;
	QLabel* m_pScoreLabel;
	QLineEdit* m_pScoreEdit;
	QLabel* m_pBestLabel;
	QLineEdit* m_pBestEdit;
	QSpacerItem* m_pSpacerEnd;
};