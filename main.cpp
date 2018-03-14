#include <QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
Q_IMPORT_PLUGIN(QJpegPlugin);

#include "MainWindow.h"

#include <QApplication>
#include <QLocale>
#include <QMutex>
#include <QDateTime>
#include <QDebug>

static QString g_strLogFile = "";

void myMessageOutput(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{
#ifndef _DEBUG
	if (type != QtFatalMsg)
		return;
	else
		_exit(1);
#else
	QString txt;

	switch (type)
	{
	case QtDebugMsg:
		txt = QString("%1: Debug: %2").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(msg);
		break;
	case QtWarningMsg:
		txt = QString("%1: Warning: %2").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(msg);
		break;
	case QtCriticalMsg:
		txt = QString("%1: Critical: %2").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(msg);
		break;
	case QtFatalMsg:
		txt = QString("%1: Fatal: %2").arg(QDateTime::currentDateTime().toString(Qt::ISODate)).arg(msg);
		break;
	}

	{ // stack frame to ensure output even id abort() called below
		QFile outFile(g_strLogFile);
		outFile.open(QIODevice::WriteOnly | QIODevice::Append);
		QTextStream ts(&outFile);
		ts << txt << endl;
	}

	if (type == QtFatalMsg)
		abort();
#endif
}

int main(int argc, char *argv[])
{
#ifdef _DEBUG
	g_strLogFile = tempFolderPath() + "\\" + QString("_tmp%1_2048GameProfessional").arg(argc) + ".log";
#endif

	qInstallMessageHandler(myMessageOutput);

	////////////////////////////////////

	QApplication a(argc, argv);
	QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

	qDebug() << Q_FUNC_INFO << "QApplication created";

	Q_INIT_RESOURCE(MainWindow);

	QFile styleFile(":/MainWindow/Resources/client.css");
	if (styleFile.open(QIODevice::ReadOnly))
	{
		QByteArray ba = styleFile.readAll();
		a.setStyleSheet(QString::fromLatin1(ba.data(), ba.size()));
	}

	MainWindow w;
	w.showMaximized();

	return a.exec();
}
