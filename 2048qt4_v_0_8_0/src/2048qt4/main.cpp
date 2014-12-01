#include "MainWindow.h"

#include <QApplication>
#include <QLocale>
#include <QMutex>
#include <QDateTime>
#include <QDebug>

QString s_strLogFile = "";
QMutex mtx;

void myMessageOutput(QtMsgType type, const char* msg)
{
	if (s_strLogFile == "")
	{
		if (type != QtFatalMsg)
			return;
		else
			abort();
	}

	QMutexLocker lock(&mtx);

	FILE* myStderr = NULL;
	errno_t res = fopen_s(&myStderr, s_strLogFile.toStdString().c_str(), "a+");
	if ((res != 0) || (myStderr == NULL))
		abort();

	switch (type)
	{
	case QtDebugMsg:
		fprintf(myStderr, "[%s] Debug: %s\n", QDateTime::currentDateTime().toString(Qt::ISODate).toStdString().c_str(), msg);
		break;
	case QtWarningMsg:
		fprintf(myStderr, "[%s] Warning: %s\n", QDateTime::currentDateTime().toString(Qt::ISODate).toStdString().c_str(), msg);
		break;
	case QtCriticalMsg:
		fprintf(myStderr, "[%s] Critical: %s\n", QDateTime::currentDateTime().toString(Qt::ISODate).toStdString().c_str(), msg);
		break;
	case QtFatalMsg:
		fprintf(myStderr, "[%s] Fatal: %s\n", QDateTime::currentDateTime().toString(Qt::ISODate).toStdString().c_str(), msg);
		break;
	}

	fclose(myStderr);

	if (type == QtFatalMsg)
		abort();
}

int main(int argc, char *argv[])
{
	qInstallMsgHandler(myMessageOutput);

	QApplication a(argc, argv);
	QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

	QFile styleFile(":/MainWindow/Resources/client.css");
	if (styleFile.open(QIODevice::ReadOnly))
	{
		QByteArray ba = styleFile.readAll();
		a.setStyleSheet(QString::fromLatin1(ba.data(), ba.size()));
	}

	MainWindow w;

	try
	{
		w.ctor();
	}
	catch (std::exception& ex)
	{
		qDebug() << ex.what();
		return 0;
	}

	w.showMaximized();

	return a.exec();
}
