#include "ScoreBar.h"


ScoreBar::ScoreBar(QWidget* parent)
:	QWidget(parent)
,	m_pLayout(0)
,	m_pSpacer(0)
,	m_pScoreLabel(0)
,	m_pScoreEdit(0)
,	m_pBestLabel(0)
,	m_pBestEdit(0)
,	m_pSpacerEnd(0)
{
	m_pLayout = new QHBoxLayout(this);
	m_pLayout->setContentsMargins(QMargins(0, 0, 0, 0));
	m_pLayout->setSpacing(0);
	m_pScoreLabel = new QLabel(this);
	m_pScoreLabel->setObjectName("score");
	m_pScoreLabel->setText(tr("Score") + " ");
	m_pLayout->addWidget(m_pScoreLabel);
	m_pScoreEdit = new QLineEdit(this);
	m_pScoreEdit->setObjectName("score");
    m_pScoreEdit->setMinimumSize(QSize(150, 25));
	m_pScoreEdit->setMaximumSize(QSize(150, 25));
	m_pScoreEdit->setReadOnly(true);
	m_pScoreEdit->setText("0");
	m_pLayout->addWidget(m_pScoreEdit);
	m_pSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Fixed);
	m_pLayout->addItem(m_pSpacer);
	m_pBestLabel = new QLabel(this);
	m_pBestLabel->setObjectName("best");
	m_pBestLabel->setText(" " + tr("Best") + " ");
	m_pLayout->addWidget(m_pBestLabel);
	m_pBestEdit = new QLineEdit(this);
	m_pBestEdit->setObjectName("best");
    m_pBestEdit->setMinimumSize(QSize(150, 25));
	m_pBestEdit->setMaximumSize(QSize(150, 25));
	m_pBestEdit->setReadOnly(true);
	m_pBestEdit->setText("0");
	m_pLayout->addWidget(m_pBestEdit);
	m_pSpacerEnd = new QSpacerItem(10, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_pLayout->addItem(m_pSpacerEnd);
}

ScoreBar::~ScoreBar()
{

}
