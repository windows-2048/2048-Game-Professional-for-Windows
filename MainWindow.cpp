#define VERSION_2048 160

/////////////////////////////////////////////////////////

#include "MainWindow.h"

#include "GameField.h"
#include "2048lib/utils.h"

#include <QCoreApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QMessageBox>
#include <QIcon>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QTime>
#include <QTimer>
#include <QDebug>

#include <cstdlib>

/////////////////////////////////////////////////////////

const QString strStyleCSS = "<style>a:link { color: DeepSkyBlue; text-decoration: none; }</style>";

const QString strStrategiesHttpM = "<a href='https://sourceforge.net/projects/exe-2048/'><b>https://sourceforge.net/projects/exe-2048/</b></a>";
const QString strHomeHttpM = "<a href='https://sourceforge.net/projects/exe-2048/'><b>https://sourceforge.net/projects/exe-2048/</b></a>";

//const QString trCompanyName = MainWindow::tr("Win-2048");
const QString trCompanyNameJuridical = MainWindow::tr("Open Source Developer<br/>Masha Novedad<br/>(twitter.com/WIN_2048_CLUB)"); // Do not add "." again
const QString trCompanyNameJuridicalUPPER = trCompanyNameJuridical; // Do not add "." again
const QString trCompanyName = trCompanyNameJuridical;

static const char* DIR_WIN_AUTO_RUN_NAME = "2048GameProfessional";

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
    :   QMainWindow(parent, flags)
	,	m_pLayoutWidget(0)
	,	m_pGameField(0)
	,	m_pAction4x4(0)
	,	m_pAction8x8(0)
    ,	m_pAction16x16(0)
	,	m_pActionNewGame(0)
	,	m_pActionHelp(0)
	,	m_pActionAbout(0)
	,	m_pActionThemeGirls(0)
	,	m_pActionThemeClassic(0)
    ,	m_pGlobalShortcutExit(0)
	,	m_settings(QSettings::IniFormat, QSettings::UserScope, DIR_WIN_AUTO_RUN_NAME, DIR_WIN_AUTO_RUN_NAME)
	,	m_nDim(4)
    ,	m_nTimerId(0)
	,	m_nBestTile(0)
{
	ctor();
}

void MainWindow::ctor()
{
	qDebug() << Q_FUNC_INFO;

	if (objectName().isEmpty())
		setObjectName("mainWindow");

    setWindowTitle(tr("2048 Game Professional"));

	m_settings.sync(); // To load possible new keyOffset

	m_settings.beginGroup("UpgradeInfo");

	if (m_settings.value("keyOffset", 0).toInt() == 0)
	{
		qDebug() << Q_FUNC_INFO << "m_settings.setValue keyOffset";
		m_settings.setValue("keyOffset", 100 + int(QDateTime::currentMSecsSinceEpoch() % 57000));
	}

	int nKeyOffset = m_settings.value("keyOffset", 100).toInt();
	qDebug() << Q_FUNC_INFO << "setKeyOffset nKeyOffset" << nKeyOffset;
	m_settings.setKeyOffset(nKeyOffset);

	m_settings.endGroup();
	m_settings.sync();

	setContentsMargins(0, 0, 0, 0);

	//resize(500, 500);
	//setMinimumSize(QSize(500, 500));
	//setMaximumSize(QSize(16777215, 16777215));

	QIcon icon;
	icon.addFile(":/MainWindow/Resources/favicon_16.png", QSize(16, 16), QIcon::Normal, QIcon::Off);
	icon.addFile(":/MainWindow/Resources/favicon_24.png", QSize(24, 24), QIcon::Normal, QIcon::Off);
	icon.addFile(":/MainWindow/Resources/favicon_32.png", QSize(32, 32), QIcon::Normal, QIcon::Off);
	icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(48, 48), QIcon::Normal, QIcon::Off);
	icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(64, 64), QIcon::Normal, QIcon::Off);
	icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(96, 96), QIcon::Normal, QIcon::Off);
	icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(128, 128), QIcon::Normal, QIcon::Off);
	icon.addFile(":/MainWindow/Resources/T2048_pink.png", QSize(256, 256), QIcon::Normal, QIcon::Off);
	setWindowIcon(icon);

	setMenuBar(new QMenuBar(this));
	QMenu* pMenuGame = menuBar()->addMenu(tr("Game"));

	m_pAction4x4 = pMenuGame->addAction(tr("Game 4x4, target %1").arg(TARGET * (4 * 4 / 16)));
	m_pAction4x4->setCheckable(true);
	m_pAction4x4->setShortcut(QKeySequence("4"));
	m_pAction4x4->setShortcutContext(Qt::ApplicationShortcut);

	m_pAction8x8 = pMenuGame->addAction(tr("Game 8x8, target %1").arg(TARGET * (8 * 8 / 16)));
	m_pAction8x8->setCheckable(true);
	m_pAction8x8->setShortcut(QKeySequence("8"));
	m_pAction8x8->setShortcutContext(Qt::ApplicationShortcut);

    m_pAction16x16 = pMenuGame->addAction(tr("Game 16x16, target %1").arg(TARGET * (16 * 16 / 16)));
    m_pAction16x16->setCheckable(true);
	m_pAction16x16->setShortcut(QKeySequence("6"));
	m_pAction16x16->setShortcutContext(Qt::ApplicationShortcut);

	m_pActionNewGame = pMenuGame->addAction(tr("New game"));
	m_pActionNewGame->setCheckable(false);
	m_pActionNewGame->setShortcut(QKeySequence(Qt::Key_End));
	m_pActionNewGame->setShortcutContext(Qt::ApplicationShortcut);

	QObject::connect(m_pAction4x4, SIGNAL(triggered(bool)), this, SLOT(onTriggered4x4(bool)));
	QObject::connect(m_pAction8x8, SIGNAL(triggered(bool)), this, SLOT(onTriggered8x8(bool)));
    QObject::connect(m_pAction16x16, SIGNAL(triggered(bool)), this, SLOT(onTriggered16x16(bool)));
	QObject::connect(m_pActionNewGame, SIGNAL(triggered()), this, SLOT(onTriggeredNewGame()));

	QMenu* pMenuTheme = menuBar()->addMenu(tr("Colors"));

	m_pActionThemeGirls = pMenuTheme->addAction(tr("Colors 'Professional'"));
	m_pActionThemeGirls->setCheckable(true);
	m_pActionThemeGirls->setShortcut(QKeySequence("g"));
	m_pActionThemeGirls->setShortcutContext(Qt::ApplicationShortcut);

	m_pActionThemeClassic = pMenuTheme->addAction(tr("Colors 'Classic'"));
	m_pActionThemeClassic->setCheckable(true);
	m_pActionThemeClassic->setShortcut(QKeySequence("c"));
	m_pActionThemeClassic->setShortcutContext(Qt::ApplicationShortcut);

	QObject::connect(m_pActionThemeGirls, SIGNAL(triggered(bool)), this, SLOT(onTriggeredThemeGirls(bool)));
	QObject::connect(m_pActionThemeClassic, SIGNAL(triggered(bool)), this, SLOT(onTriggeredThemeClassic(bool)));

	QMenu* pMenuHelp = menuBar()->addMenu(tr("Help"));

	m_pActionHelp = pMenuHelp->addAction(tr("Game rules"));
	m_pActionAbout = pMenuHelp->addAction(tr("About"));

	QObject::connect(m_pActionHelp, SIGNAL(triggered()), this, SLOT(onTriggeredHelp()));
	QObject::connect(m_pActionAbout, SIGNAL(triggered()), this, SLOT(onTriggeredAbout()));

    m_pGlobalShortcutExit = new QxtGlobalShortcut(QKeySequence("Esc"), this);
    QObject::connect(m_pGlobalShortcutExit, SIGNAL(activated()), this, SLOT(onTriggeredExit()));

	m_settings.beginGroup("MainWindow");

	m_pAction4x4->setChecked(m_settings.value("pAction4x4Checked", true).toBool());
	m_pAction8x8->setChecked(m_settings.value("pAction8x8Checked", false).toBool());
    m_pAction16x16->setChecked(m_settings.value("pAction16x16Checked", false).toBool());

	m_pActionThemeGirls->setChecked(m_settings.value("pActionThemeGirls", true).toBool());
	m_pActionThemeClassic->setChecked(m_settings.value("pActionThemeClassic", false).toBool());

	if (m_pAction4x4->isChecked())
		m_nDim = 4;
	else if (m_pAction8x8->isChecked())
		m_nDim = 8;
    else if (m_pAction16x16->isChecked())
        m_nDim = 16;
	else
		m_nDim = 32;

	m_settings.endGroup();

	m_pLayoutWidget = new QWidget(this);
	QVBoxLayout* pLayout = new QVBoxLayout(m_pLayoutWidget);
	pLayout->setContentsMargins(0, 0, 0, 0);
	pLayout->setSpacing(0);
	pLayout->insertWidget(0, new ScoreBar(m_pLayoutWidget));
	setCentralWidget(m_pLayoutWidget);

	setupUIForGame(m_nDim, m_pActionThemeGirls->isChecked());

	pLayout->insertWidget(2, new QLabel(m_pLayoutWidget));
	qobject_cast<QLabel*>(m_pLayoutWidget->layout()->itemAt(2)->widget())->setObjectName("tinyHelp");
    qobject_cast<QLabel*>(m_pLayoutWidget->layout()->itemAt(2)->widget())->setTextFormat(Qt::RichText);
    qobject_cast<QLabel*>(m_pLayoutWidget->layout()->itemAt(2)->widget())->setText(tr("<font color='#fd97a2'><b>2048</b></font> game-play tiny help. Game step: keyboard arrows '<b>Left</b>', '<b>Right</b>', '<b>Up</b>', '<b>Down</b>' (or corresponding '<b>Mouse click-moves</b>'). New game: '<b>End</b>'. Exit & save: '<b>Esc</b>'"));
	qobject_cast<QLabel*>(m_pLayoutWidget->layout()->itemAt(2)->widget())->setAlignment(Qt::AlignRight);
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent* pEvent)
{
	saveGame();
	pEvent->accept();
}

void MainWindow::tmUpd()
{
	m_pGameField->update();
	m_pGameField->parentWidget()->update();
}

void MainWindow::saveGame()
{
	if (scoreBar()->scoreEdit()->text().toInt() > scoreBar()->bestEdit()->text().toInt())
		scoreBar()->bestEdit()->setText(scoreBar()->scoreEdit()->text());

	if (m_pGameField->game()->tile() > m_nBestTile)
		m_nBestTile = m_pGameField->game()->tile();

	m_pGameField->setBestScore(scoreBar()->bestEdit()->text().toInt());
	m_pGameField->setBestTile(m_nBestTile);
	tmUpd();

	m_settings.beginGroup("MainWindow");
	m_settings.setValue("pAction4x4Checked", m_pAction4x4->isChecked());
	m_settings.setValue("pAction8x8Checked", m_pAction8x8->isChecked());
	m_settings.setValue("pAction16x16Checked", m_pAction16x16->isChecked());
	m_settings.setValue("pActionThemeGirls", m_pActionThemeGirls->isChecked());
	m_settings.setValue("pActionThemeClassic", m_pActionThemeClassic->isChecked());
	m_settings.endGroup();

	m_settings.beginGroup("GameAlgorithm");

	try
	{
		m_settings.setValue(QString("GameAlgorithm%1x%1").arg(m_nDim), m_pGameField->game()->serializeToString());
		m_settings.setValue(QString("GameAlgorithm%1x%1score").arg(m_nDim), scoreBar()->scoreEdit()->text().toInt());
		m_settings.setValue(QString("GameAlgorithm%1x%1best").arg(m_nDim), scoreBar()->bestEdit()->text().toInt());
		m_settings.setValue(QString("GameAlgorithm%1x%1tile").arg(m_nDim), m_pGameField->game()->tile());
		m_settings.setValue(QString("GameAlgorithm%1x%1btile").arg(m_nDim), m_nBestTile);
	}
	catch (std::exception& ex)
	{
		QMessageBox msgBox(this);
		msgBox.setModal(true);
		msgBox.setTextFormat(Qt::RichText);
		msgBox.addButton(QMessageBox::Close);
		msgBox.setWindowTitle(tr("Error - 2048 Game Professional"));
		msgBox.setText(tr("<font color='#fd97a2'><b>2048 Game Professional</b></font><br/>has detected internal error:<br/>%1.").arg(ex.what()));
		msgBox.setInformativeText("<small>" + tr("Report this bug to support@win-2048.org or support@windows-2048.com.") + "</small>");

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

		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setIconPixmap(QPixmap(":/MainWindow/Resources/error.png"));

		msgBox.exec();
	}

	m_settings.endGroup();

	m_settings.sync();
}

void MainWindow::onTriggered4x4(bool bT)
{
	m_pAction4x4->setChecked(true);
	m_pAction8x8->setChecked(false);
    m_pAction16x16->setChecked(false);

	saveGame();

	m_nDim = 4;
	setupUIForGame(m_nDim, m_pActionThemeGirls->isChecked());
}

void MainWindow::onTriggered8x8(bool bT)
{
	m_pAction4x4->setChecked(false);
	m_pAction8x8->setChecked(true);
    m_pAction16x16->setChecked(false);

	saveGame();

	m_nDim = 8;
	setupUIForGame(m_nDim, m_pActionThemeGirls->isChecked());
}

void MainWindow::onTriggered16x16(bool bT)
{
    m_pAction4x4->setChecked(false);
    m_pAction8x8->setChecked(false);
    m_pAction16x16->setChecked(true);

	saveGame();

    m_nDim = 16;
    setupUIForGame(m_nDim, m_pActionThemeGirls->isChecked());
}

void MainWindow::onTriggeredNewGame()
{
	m_pGameField->newGame();
}

void MainWindow::onTriggeredThemeGirls(bool)
{
	m_pActionThemeGirls->setChecked(true);
	m_pActionThemeClassic->setChecked(false);
	m_pGameField->setThemeGirls(true);

	saveGame();

	QTimer::singleShot(100, this, SLOT(tmUpd()));
}

void MainWindow::onTriggeredThemeClassic(bool)
{
	m_pActionThemeGirls->setChecked(false);
	m_pActionThemeClassic->setChecked(true);
	m_pGameField->setThemeGirls(false);

	saveGame();

	QTimer::singleShot(100, this, SLOT(tmUpd()));
}

void MainWindow::onTriggeredHelp()
{
	QString strHtmlStrategiesM = strStyleCSS + strStrategiesHttpM;

	QMessageBox msgBox(this);
	msgBox.setModal(true);
    msgBox.setTextFormat(Qt::RichText);
	msgBox.addButton(QMessageBox::Close);
	msgBox.setWindowTitle(tr("Help - 2048 Game Professional"));
    msgBox.setText(tr(
        "Help on <font color='#fd97a2'><b>2048 Game Professional</b></font>.<br/><br/>"
		"<b>Keyboard hot keys</b>:"
		"<small>"
		"<table cellpadding='0' cellspacing='0' border='0'>"
		"<tr><td>"
        "Game step"
		"</td><td>"
        "'<b>Left</b>', '<b>Right</b>',<br/>'<b>Up</b>',<br/>'<b>Down</b>' arrows<br/>(or corresponding '<b>Mouse click-moves</b>')"
		"</td></tr>"
		"<tr><td colspawn='2'>"
        "Switch to another game"
		"</td></tr>"
		"<tr><td>"
        "- game 4x4"
		"</td><td>"
		"'<b>4</b>'"
		"</td></tr>"
		"<tr><td>"
        "- game 8x8"
		"</td><td>"
		"'<b>8</b>'"
		"</td></tr>"
		"<tr><td>"
        "- game 16x16"
		"</td><td>"
		"'<b>6</b>'"
		"</td></tr>"
		"<tr><td>"
        "New game"
		"</td><td>"
		"'<b>End</b>'"
		"</td></tr>"
		"<tr><td>"
        "Exit"
		"</td><td>"
		"'<b>Esc</b>'"
		"</td></tr>"
		"</table>"
		"</small>"
		"<br/><br/>"
		"Game state is saved automatically each time you change something (excepts starting new game). "
		"Game goal is to reach score 2048 or more, making tiles merged, while you can still do next step."
    ));
    msgBox.setInformativeText(tr(
		"<small>"
		"When you are doing a&nbsp;game step (by keyboard or mouse), sibling tiles with the&nbsp;same value are being merged into one tile of double value. "
		"Game score is therefore increased by total value of all merged tiles. Note, though, some new tiles of value&nbsp;2&nbsp;(probability&nbsp;90%) or&nbsp;4&nbsp;(probability&nbsp;10%) will appear. "
		"The number of new tiles to appear before the&nbsp;first game step is&nbsp;2 for game 4x4, 8&nbsp;for game 8x8, 32&nbsp;for game 16x16. "
        "For all the&nbsp;next game steps, the&nbsp;number halves to&nbsp;1, 4 or 16&nbsp;respectively. "
        "Thus, the&nbsp;relative square covered by the&nbsp;new tiles is kept constant for all the&nbsp;game dimensions. "
		"So, your risk is a&nbsp;game field full of tiles while absence of a&nbsp;next step."
		"<br/><br/>"
		"Good luck in making 2048 tile!<br/>Learn more about how to play and the game strategies at our&nbsp;site:<br/>%1."
		"</small>"
	).arg(strHtmlStrategiesM));
	
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

    msgBox.setIcon(QMessageBox::Information);
    msgBox.setIconPixmap(QPixmap(":/MainWindow/Resources/help_128.png"));

	msgBox.exec();
}

void MainWindow::onTriggeredAbout()
{
    QString strVer = stringVersion(VERSION_2048);
	QString strHomeM = strStyleCSS + strHomeHttpM;

    QMessageBox msgBox(this);
    msgBox.setModal(true);
    msgBox.setTextFormat(Qt::RichText);
    msgBox.addButton(QMessageBox::Close);
    msgBox.setWindowTitle(tr("About - 2048 Game Professional"));
    msgBox.setText(tr("<font color='#fd97a2'><b>2048 Game Professional</b></font>,<br/>formerly <font color='#ac8691'><b>2048 Game Girls Choice With Kitties For Windows Desktop</b></font>,<br/>version <b>%1</b>,<br/>build %2 - %3.<br/><br/>Visit our&nbsp;site:<br/>%4").arg(strVer).arg(__DATE__).arg(__TIME__).arg(strHomeM));
    msgBox.setInformativeText("<small>" + tr("Copyright (c) 2017-2018 ") + trCompanyNameJuridicalUPPER + ".</small>");

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

    msgBox.setIcon(QMessageBox::Information);
    msgBox.setIconPixmap(QPixmap(":/MainWindow/Resources/T2048_pink.png"));

    msgBox.exec();
}

void MainWindow::onTriggeredExit()
{
    close();
}

ScoreBar* MainWindow::scoreBar()
{
	return qobject_cast<ScoreBar*>(m_pLayoutWidget->layout()->itemAt(0)->widget());
}

void MainWindow::onFieldChanged(int nScore)
{
	scoreBar()->scoreEdit()->setText(QString("%1").arg(nScore));
	saveGame();
}

int MainWindow::spacingForDimention(int nDim)
{
	if (nDim <= 4)
		return 4;
	if (nDim <= 8)
		return 3;
	if (nDim <= 16)
		return 2;
	return 4;
}

void MainWindow::setupUIForGame(int nDim, bool bGirls)
{
    m_pGameField.reset(new GameField(nDim, bGirls, spacingForDimention(m_nDim), m_pLayoutWidget));

	m_pGameField->setObjectName("centralWidget");
	m_pGameField->setMinimumSize(QSize(500, 500));
	m_pGameField->setMaximumSize(QSize(16777215, 16777215));
	m_pGameField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	qobject_cast<QVBoxLayout*>(centralWidget()->layout())->insertWidget(1, m_pGameField.data());

	m_pLayoutWidget->layout()->invalidate();

	m_settings.beginGroup("GameAlgorithm"); // Encrypted group

	try
	{
		QString strImage = m_settings.value(QString("GameAlgorithm%1x%1").arg(nDim), "").toString();
		if (strImage != "")
			m_pGameField->game()->deserializeFromString(strImage);
		int nScore = m_settings.value(QString("GameAlgorithm%1x%1score").arg(nDim), 0).toInt();
		m_pGameField->game()->setScore(nScore);
		scoreBar()->scoreEdit()->setText(QString("%1").arg(nScore));
		int nBest = m_settings.value(QString("GameAlgorithm%1x%1best").arg(nDim), 0).toInt();
		scoreBar()->bestEdit()->setText(QString("%1").arg(nBest));
		m_nBestTile = m_settings.value(QString("GameAlgorithm%1x%1btile").arg(nDim), 0).toInt();
		m_pGameField->setBestScore(scoreBar()->bestEdit()->text().toInt());
		m_pGameField->setBestTile(m_nBestTile);
	}
	catch (std::exception& ex)
	{
		QMessageBox msgBox(this);
		msgBox.setModal(true);
		msgBox.setTextFormat(Qt::RichText);
		msgBox.addButton(QMessageBox::Close);
		msgBox.setWindowTitle(tr("Error - 2048 Game Professional"));
		msgBox.setText(tr("<font color='#fd97a2'><b>2048 Game Professional</b></font><br/>has detected corrupted game settings:<br/>%1.").arg(ex.what()));
		msgBox.setInformativeText("<small>" + tr("Do you try to cheat on your score or game state?<br/>If not, report this bug to support@win-2048.org or support@windows-2048.com.") + "</small>");

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

		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setIconPixmap(QPixmap(":/MainWindow/Resources/error.png"));

		msgBox.exec();

		qobject_cast<QVBoxLayout*>(centralWidget()->layout())->removeWidget(m_pGameField.data());

		m_pGameField.reset(new GameField(nDim, bGirls, spacingForDimention(m_nDim), m_pLayoutWidget));

		m_pGameField->setObjectName("centralWidget");
		m_pGameField->setMinimumSize(QSize(500, 500));
		m_pGameField->setMaximumSize(QSize(16777215, 16777215));
		m_pGameField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		qobject_cast<QVBoxLayout*>(centralWidget()->layout())->insertWidget(1, m_pGameField.data());

		m_pLayoutWidget->layout()->invalidate();

		m_pGameField->game()->setScore(0);
		scoreBar()->scoreEdit()->setText(QString("%1").arg(0));
		scoreBar()->bestEdit()->setText(QString("%1").arg(0));
		m_nBestTile = 0;
		m_pGameField->setBestScore(scoreBar()->bestEdit()->text().toInt());
		m_pGameField->setBestTile(m_nBestTile);
	}

	m_settings.endGroup();

	m_pGameField->updateGame();
	m_pGameField->setFocus();

	QObject::connect(m_pGameField.data(), SIGNAL(fieldChanged(int)), this, SLOT(onFieldChanged(int)));

	statusBar()->showMessage(tr("Switch to game %1x%1 (auto-save ok)").arg(nDim), 3000);
}

void MainWindow::onShowWindowTopLevel()
{
    activateWindow();
    raise();
}
