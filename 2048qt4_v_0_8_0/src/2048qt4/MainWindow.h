#pragma once

//#define VERSION_2048 80

#include "ScoreBar.h"

#include <QMainWindow>
#include <QAction>
#include <QCloseEvent>
#include <QScopedPointer>

#include <exception>

// Forward declarations

class GameField;
class QSplashScreen;
class AnotherThreadStarter;


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~MainWindow();
	void ctor();

public:

	ScoreBar* scoreBar();

protected slots:

	void onTriggered4x4(bool);
	void onTriggered8x8(bool);
    void onTriggered16x16(bool);
	void onTriggeredNewGame();
	void onTriggeredHelp();
    void onTriggeredCheckUpdates();
	void onTriggeredAbout();
    void onTriggeredExit();
	void onFieldChanged(int nScore);
    void onShowWindowTopLevel();

protected:

	void closeEvent(QCloseEvent* pEvent); // We re-implement from Qt
	void installGameApp();
	void setupUIForGame(int nDim);
	void saveGame();
    int spacingForDimention(int nDim);

protected:

	int m_nDim;

private:

	QWidget* m_pLayoutWidget;
	QScopedPointer<GameField> m_pGameField;
	QAction* m_pAction4x4;
	QAction* m_pAction8x8;
    QAction* m_pAction16x16;
	QAction* m_pActionNewGame;
	QAction* m_pActionHelp;
    QAction* m_pActionCheckUpdates;
	QAction* m_pActionAbout;
};
