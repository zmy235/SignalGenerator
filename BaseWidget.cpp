#include "SignalGenerator.h"
#include "BaseWidget.h"
#include "Task.h"

#include <QDebug>

float BaseWidget::v = 1;

BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent)
{
	setWindowOpacity(v);
	//setWindowFlags(Qt::FramelessWindowHint);

	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QPixmap("./Resources/bg.png")));
	setPalette(palette);
	setAttribute(Qt::WA_QuitOnClose, false);

	//���ƶ�����
	m_areaMovable = geometry();
	m_bPressed = false;
}

void BaseWidget::mousePressEvent(QMouseEvent *e)
{
	//������
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

void BaseWidget::setAreaMovable(const QRect rt)
{
	//���ÿ��ƶ�������
	if (m_areaMovable != rt)
	{
		m_areaMovable = rt;
	}
}