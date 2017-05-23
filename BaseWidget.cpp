#include "SignalGenerator.h"
#include "BaseWidget.h"
#include "Task.h"
#include <QDebug>

float BaseWidget::v = 1;

BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent)
{
	//���ƶ�����
	m_areaMovable = geometry();
	m_bPressed = false;

	//���ô��ڴ�С
	setMinimumSize(640, 480);
	//setMaximumSize(1280, 960);

	//�����ޱ߿�͸��
	setWindowOpacity(v);
	//setWindowFlags(Qt::FramelessWindowHint);
	//setWindowFlags(Qt::CustomizeWindowHint);

	//����QPalette����ı������ԣ���ɫ��ͼƬ��  
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QPixmap("./Resources/bg0.png")));
	setPalette(palette);

	setAttribute(Qt::WA_QuitOnClose, false);
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

//������갴�µ�����
void BaseWidget::setAreaMovable(const QRect rt)
{
	if (m_areaMovable != rt)
	{
		m_areaMovable = rt;
	}
}