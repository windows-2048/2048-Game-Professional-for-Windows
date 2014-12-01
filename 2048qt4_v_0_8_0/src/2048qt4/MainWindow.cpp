#include "MainWindow.h"

#include "GameField.h"

#include <QCoreApplication>
#include <QSplashScreen>
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

#include <Windows.h>

const QString strStyleCSS = "<style>a:link { color: DeepSkyBlue; text-decoration: none; }</style>";

const QString strStrategiesHttp = "<a href='https://win-2048.org/2048exe-faq.html'><big><b>win-2048.org/2048exe-faq.html</b></big></a>";
const QString strStrategiesHttpM = "<a href='https://windows-2048.com/2048exe-faq.html'><big><b>windows-2048.com/2048exe-faq.html</b></big></a>";
const QString strBrandNewChatHttp = "<a href='https://win-2048.org/brand-new-chat.html'><big><b>win-2048.org/brand-new-chat.html</b></big></a>";
const QString strBrandNewChatHttpM = "<a href='https://windows-2048.com/brand-new-chat.html'><big><b>windows-2048.com/brand-new-chat.html</b></big></a>";
const QString strUpdatesHttp = "<a href='https://win-2048.org/pages/downloads.html'><big><b>win-2048.org/pages/downloads.html</b></big></a>";
const QString strUpdatesHttpM = "<a href='https://windows-2048.com/pages/downloads.html'><big><b>windows-2048.com/pages/downloads.html</b></big></a>";
const QString strHomeHttp = "<a href='https://win-2048.org/'><big><b>win-2048.org</b></big></a>";
const QString strHomeHttpM = "<a href='https://windows-2048.com/'><big><b>windows-2048.com</b></big></a>";

const QString trCompanyName = MainWindow::tr("Win-2048");
const QString trCompanyNameJuridical = MainWindow::tr("Win-2048 Society Club"); // Do not add "." again
const QString trCompanyNameJuridicalUPPER = MainWindow::tr("WIN-2048 SOCIETY CLUB"); // Do not add "." again


MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
    :   QMainWindow(parent, flags)
	,	m_pLayoutWidget(0)
	,	m_pGameField(0)
	,	m_pAction4x4(0)
	,	m_pAction8x8(0)
    ,	m_pAction16x16(0)
	,	m_pActionNewGame(0)
	,	m_pActionHelp(0)
    ,   m_pActionCheckUpdates(0)
	,	m_pActionAbout(0)
	,	m_nDim(4)
{
}

void MainWindow::ctor()
{
	if (objectName().isEmpty())
		setObjectName("mainWindow");

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

	m_pAction4x4 = pMenuGame->addAction(tr("Game 4x4"));
	m_pAction4x4->setCheckable(true);
	m_pAction4x4->setShortcut(QKeySequence("4"));
	m_pAction4x4->setShortcutContext(Qt::ApplicationShortcut);

	m_pAction8x8 = pMenuGame->addAction(tr("Game 8x8"));
	m_pAction8x8->setCheckable(true);
	m_pAction8x8->setShortcut(QKeySequence("8"));
	m_pAction8x8->setShortcutContext(Qt::ApplicationShortcut);

    m_pAction16x16 = pMenuGame->addAction(tr("Game 16x16"));
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

	QMenu* pMenuHelp = menuBar()->addMenu(tr("Help"));

	m_pActionHelp = pMenuHelp->addAction(tr("Help"));
    m_pActionCheckUpdates = pMenuHelp->addAction(tr("Check updates"));
	m_pActionAbout = pMenuHelp->addAction(tr("About"));

	QObject::connect(m_pActionHelp, SIGNAL(triggered()), this, SLOT(onTriggeredHelp()));
    QObject::connect(m_pActionCheckUpdates, SIGNAL(triggered()), this, SLOT(onTriggeredCheckUpdates()));
	QObject::connect(m_pActionAbout, SIGNAL(triggered()), this, SLOT(onTriggeredAbout()));

    m_pAction4x4->setChecked(true);
    m_pAction8x8->setChecked(false);
    m_pAction16x16->setChecked(false);

	if (m_pAction4x4->isChecked())
		m_nDim = 4;
	else if (m_pAction8x8->isChecked())
		m_nDim = 8;
    else if (m_pAction16x16->isChecked())
        m_nDim = 16;
	else
		m_nDim = 32;

	m_pLayoutWidget = new QWidget(this);
	QVBoxLayout* pLayout = new QVBoxLayout(m_pLayoutWidget);
	pLayout->setContentsMargins(0, 0, 0, 0);
	pLayout->setSpacing(0);
	pLayout->insertWidget(0, new ScoreBar(m_pLayoutWidget));
	setCentralWidget(m_pLayoutWidget);

	setupUIForGame(m_nDim);

	pLayout->insertWidget(2, new QLabel(m_pLayoutWidget));
	qobject_cast<QLabel*>(m_pLayoutWidget->layout()->itemAt(2)->widget())->setObjectName("tinyHelp");
    qobject_cast<QLabel*>(m_pLayoutWidget->layout()->itemAt(2)->widget())->setTextFormat(Qt::RichText);
    qobject_cast<QLabel*>(m_pLayoutWidget->layout()->itemAt(2)->widget())->setText(tr("<font color='Gold'><b>2048</b></font> tiny help. Game step: keyboard arrows '<b>Left</b>', '<b>Right</b>', '<b>Up</b>', '<b>Down</b>' (or corresponding '<b>Mouse click-moves</b>'). New game: '<b>End</b>'. Exit & save: '<b>Esc</b>'"));
	qobject_cast<QLabel*>(m_pLayoutWidget->layout()->itemAt(2)->widget())->setAlignment(Qt::AlignRight);

	installGameApp();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent* pEvent)
{
	saveGame();
	pEvent->accept();
}

void MainWindow::saveGame()
{
    // TODO: implement
}

void MainWindow::onTriggered4x4(bool bT)
{
	saveGame();

	m_pAction4x4->setChecked(true);
	m_pAction8x8->setChecked(false);
    m_pAction16x16->setChecked(false);

	m_nDim = 4;
	setupUIForGame(m_nDim);
}

void MainWindow::onTriggered8x8(bool bT)
{
	saveGame();

	m_pAction4x4->setChecked(false);
	m_pAction8x8->setChecked(true);
    m_pAction16x16->setChecked(false);

	m_nDim = 8;
	setupUIForGame(m_nDim);
}

void MainWindow::onTriggered16x16(bool bT)
{
	saveGame();

    m_pAction4x4->setChecked(false);
    m_pAction8x8->setChecked(false);
    m_pAction16x16->setChecked(true);

    m_nDim = 16;
    setupUIForGame(m_nDim);
}

void MainWindow::onTriggeredNewGame()
{
	m_pGameField->newGame();
}

void MainWindow::onTriggeredHelp()
{
	QString strHtmlStrategies = strStyleCSS + strStrategiesHttp;
	QString strHtmlStrategiesM = strStyleCSS + strStrategiesHttpM;

	QMessageBox msgBox(this);
	msgBox.setModal(true);
    msgBox.setTextFormat(Qt::RichText);
	msgBox.addButton(QMessageBox::Close);
	msgBox.setWindowTitle(trCompanyNameJuridical);
    msgBox.setText(tr(
        "Help on the <font color='Gold'><b>2048</b></font> game application.<br/><br/>"
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
		"Good luck in making 2048 tile! Learn more about how to play and game strategies at our&nbsp;site<br/>%1<br/>or&nbsp;%2&nbsp;(mirror)."
		"</small>"
	).arg(strHtmlStrategies).arg(strHtmlStrategiesM));
	
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

void MainWindow::onTriggeredCheckUpdates()
{
    QString strHtmlChat = strStyleCSS + strBrandNewChatHttp;
	QString strHtmlChatM = strStyleCSS + strBrandNewChatHttpM;
	QString strHtmlUpdates = strStyleCSS + strUpdatesHttp;
	QString strHtmlUpdatesM = strStyleCSS + strUpdatesHttpM;
	QString strChatReleaseDate = tr("April 15, 2015");

    QMessageBox msgBox(this);
    msgBox.setModal(true);
    msgBox.setTextFormat(Qt::RichText);
    msgBox.addButton(QMessageBox::Close);
    msgBox.setWindowTitle(trCompanyNameJuridical);
    msgBox.setText(tr(
        "Next big release, <font color='Gold'><b>2048</b></font>&nbsp;v.&nbsp;<b>2.0.0</b>, will introduce you brand new unique feature."
        "<br/><br/>"
        "This is instant live chat that allows you to communicate directly with any person in the&nbsp;world playing 2048 right now. "
        "You can share images, music, videos each other in this chat by simply drag-n-drop them from Windows desktop, explorer or another application like facebook or vk. "
        "User interface will be most intuitive, and detailed help in both textual and video format will be applied. "
        "Lean more about chat feature on our&nbsp;site<br/>%1<br/>or&nbsp;%2&nbsp;(mirror)."
		"<table cellpadding='8' cellspacing='0' border='0'><tr><td>"
		"<img src=':/MainWindow/Resources/chat_250.png'/>"
		"</td><td>"
        "This chat feature is scheduled to be released on&nbsp;%3. "
        "Note, bug fix releases will appear time to time, prior to that date. "
        "For a&nbsp;while, you can manually and periodically check updates at<br/>%4<br/>or&nbsp;%5&nbsp;(mirror)."
		"</td></tr></table>"
    ).arg(strHtmlChat).arg(strHtmlChatM).arg(strChatReleaseDate).arg(strHtmlUpdates).arg(strHtmlUpdatesM));
    msgBox.setInformativeText(tr(
		"<small>"
		"All future applications of "
		)
		+ trCompanyName
		+ tr(" family will be equipped with the&nbsp;chat feature described above. "
		"Also, <font color='Gold'><b>2048</b></font> application as well as any other "
		)
		+ trCompanyName
		+ tr(" family's application can be updated through our standard way by simply starting downloaded new release executable. "
		"All the&nbsp;user data will be preserved during update. "
		"Automatic updates coming soon with new chat feature too."
		"</small>"
	));

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

void MainWindow::onTriggeredAbout()
{
    QString strVer = "0.8.0";
	QString strHome = strStyleCSS + strHomeHttp;
	QString strHomeM = strStyleCSS + strHomeHttpM;
	QString strUpdates = strStyleCSS + strUpdatesHttp;
	QString strUpdatesM = strStyleCSS + strUpdatesHttpM;

    QMessageBox msgBox(this);
    msgBox.setModal(true);
    msgBox.setTextFormat(Qt::RichText);
    msgBox.addButton(QMessageBox::Close);
    msgBox.setWindowTitle(trCompanyNameJuridical);
    msgBox.setText(tr("<font color='Gold'><b>2048</b></font> native game application for Windows,&nbsp;v.&nbsp;<b>%1</b>,<br/>build %2 - %3.<br/><br/>Visit our&nbsp;site<br/>%4<br/>or&nbsp;%5&nbsp;(mirror).<br/><br/>Check updates at<br/>%6<br/>or&nbsp;%7&nbsp;(mirror).").arg(strVer).arg(__DATE__).arg(__TIME__).arg(strHome).arg(strHomeM).arg(strUpdates).arg(strUpdatesM));
    msgBox.setInformativeText("<small>" + tr("Copyright (c) ")
		+ trCompanyNameJuridical + ". "
		+ tr("Press 'Show Details...' for GPL v3 license this program is under. Note license may be changed in future versions of the program.")
		+ "</small>");

	QFile fileGPLv3(":/MainWindow/Resources/gpl-3.0.txt");
	if (fileGPLv3.open(QIODevice::ReadOnly))
	{
		QByteArray baGPLv3 = fileGPLv3.readAll();
		QString strGPLv3 = QString::fromLatin1(baGPLv3);
		msgBox.setDetailedText(strGPLv3);
	}

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

	bool bOk = false;
	int nBestScore = scoreBar()->bestEdit()->text().toInt(&bOk);
	if (bOk && (nScore > nBestScore))
	{
		scoreBar()->bestEdit()->setText(QString("%1").arg(nScore));
	}

	saveGame();
}

int MainWindow::spacingForDimention(int nDim)
{
    if (nDim <= 4)
        return 4;
    if (nDim <= 8)
        return 3;
    return 2;
}

void MainWindow::setupUIForGame(int nDim)
{
    m_pGameField.reset(new GameField(nDim, spacingForDimention(m_nDim), m_pLayoutWidget));

	m_pGameField->setObjectName("centralWidget");
	m_pGameField->setMinimumSize(QSize(500, 500));
	m_pGameField->setMaximumSize(QSize(16777215, 16777215));
	m_pGameField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	qobject_cast<QVBoxLayout*>(centralWidget()->layout())->insertWidget(1, m_pGameField.data());

	m_pLayoutWidget->layout()->invalidate();

	int nScore = 0;
	m_pGameField->game()->setScore(nScore);
	scoreBar()->scoreEdit()->setText(QString("%1").arg(nScore));
	int nBest = 0;
	scoreBar()->bestEdit()->setText(QString("%1").arg(nBest));

	m_pGameField->updateGame();
	m_pGameField->setFocus();

	QObject::connect(m_pGameField.data(), SIGNAL(fieldChanged(int)), this, SLOT(onFieldChanged(int)));

	statusBar()->showMessage(tr("Switch to game %1x%1").arg(nDim), 3000);
}

void MainWindow::installGameApp()
{
	// Show main window on top level (but do not stick top)

    QTimer::singleShot(1000, this, SLOT(onShowWindowTopLevel()));
}

void MainWindow::onShowWindowTopLevel()
{
    activateWindow();
    raise();
}
