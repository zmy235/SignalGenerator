#include "SignalGenerator.h"
#include "BaseWidget.h"
#include "Task.h"
#include <QDebug>

float BaseWidget::v = 1;

BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent)
{
	//可移动区域
	m_areaMovable = geometry();
	m_bPressed = false;

	//设置窗口大小
	setMinimumSize(640, 480);
	//setMaximumSize(1280, 960);

	//设置无边框透明
	setWindowOpacity(v);
	//setWindowFlags(Qt::FramelessWindowHint);
	//setWindowFlags(Qt::CustomizeWindowHint);

	//设置QPalette对象的背景属性（颜色或图片）  
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QPixmap("./Resources/bg0.png")));
	setPalette(palette);

	setAttribute(Qt::WA_QuitOnClose, false);
}

void BaseWidget::mousePressEvent(QMouseEvent *e)
{
	//鼠标左键
	if (e->button() == Qt::LeftButton)
	{
		m_ptPress = e->pos();
		qDebug() << pos() << e->pos() << m_ptPress;
		m_bPressed = m_areaMovable.contains(m_ptPress);
	}
}

void BaseWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (m_bPressed)
	{
		qDebug() << pos() << e->pos() << m_ptPress;
		move(pos() + e->pos() - m_ptPress);
	}
}

void BaseWidget::mouseReleaseEvent(QMouseEvent *)
{
	m_bPressed = false;
}

//设置鼠标按下的区域
void BaseWidget::setAreaMovable(const QRect rt)
{
	if (m_areaMovable != rt)
	{
		m_areaMovable = rt;
	}
}