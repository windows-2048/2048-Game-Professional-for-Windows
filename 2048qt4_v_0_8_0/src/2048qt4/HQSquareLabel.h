#pragma once

#include <QGridLayout>
#include <QLabel>


class HQSquareLayout: public QGridLayout
{
public:

	HQSquareLayout(QWidget* parent)
		:	QGridLayout(parent)
	{

	}

	HQSquareLayout()
		:	QGridLayout()
	{

	}

	virtual ~HQSquareLayout()
	{

	}

	virtual bool hasHeightForWidth() const
	{
		return true;
	}

	virtual int heightForWidth(int w) const
	{
		return w;
	}
};

class HQSquareLabel: public QLabel
{
public:

	HQSquareLabel(QWidget* parent = 0, Qt::WindowFlags f = 0)
		:	QLabel(parent, f)
	{

	}

	virtual ~HQSquareLabel()
	{

	}

	virtual bool hasHeightForWidth() const
	{
		return true;
	}

	virtual int heightForWidth(int w) const
	{
		return w;
	}
};
