#pragma once

#include "ScoreBar.h"

#include <QMainWindow>
#include <QAction>
#include <QCloseEvent>
#include <QSettings>
#include <QScopedPointer>
#include <QProcess>
#include <QThread>
#include <QSharedMemory>

#include "qxtglobalshortcut.h"

#include "2048lib/utils.h"

#include <exception>

// Forward declarations

class GameField;


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());
	virtual ~MainWindow();

public:

	ScoreBar* scoreBar();

protected slots:

	void onTriggered4x4(bool);
	void onTriggered8x8(bool);
    void onTriggered16x16(bool);
	void onTriggeredNewGame();
	void onTriggeredHelp();
	void onTriggeredAbout();
    void onTriggeredExit();
	void onTriggeredThemeGirls(bool);
	void onTriggeredThemeClassic(bool);
	void onFieldChanged(int nScore);
    void onShowWindowTopLevel();
	void tmUpd();

protected:

	void closeEvent(QCloseEvent* pEvent); // We re-implement from Qt
	void setupUIForGame(int nDim, bool bGirls);
	void saveGame();
    int spacingForDimention(int nDim);
	void ctor();

protected:

	SettingsEx m_settings;
	int m_nDim;

private:

	QWidget* m_pLayoutWidget;
	QScopedPointer<GameField> m_pGameField;
	QAction* m_pAction4x4;
	QAction* m_pAction8x8;
    QAction* m_pAction16x16;
	QAction* m_pActionNewGame;
	QAction* m_pActionHelp;
	QAction* m_pActionAbout;
	QAction* m_pActionThemeGirls;
	QAction* m_pActionThemeClassic;
    QxtGlobalShortcut* m_pGlobalShortcutExit;
    int m_nTimerId;
	int m_nBestTile;
};

