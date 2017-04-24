#include "SignalGenerator.h"
#include "BaseWidget.h"
#include <QDebug>

BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent)
{
	//���ƶ�����
	m_areaMovable = geometry();
	m_bPressed = false;

	//���ô��ڴ�С
	setFixedSize(500, 400);

	//�����ޱ߿�͸��
	setWindowOpacity(0.9);
	setWindowFlags(Qt::FramelessWindowHint);

	//����QPalette����ı������ԣ���ɫ��ͼƬ��  
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QPixmap("./Resources/background.png")));
	setPalette(palette);
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