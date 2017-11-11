#include "SignalGenerator.h"
#include "BaseWidget.h"
#include "SetView.h"
#include "Task.h"

#include<Windows.h>
#include<QWidget>
#include<QSlider>
#include<QPalette>
#include<QFont>
#include<QHBoxLayout>
#include<QRect>
#include<QPoint>
#include<QDockWidget>
#include<QPushButton>
#include<QTextEdit>
#include<QRadioButton>
#include<QComboBox>
#include<QUrl>
#include<QFileDialog>

SetView::SetView(QWidget *parent) : BaseWidget(parent)
{
	setWindowTitle(tr("Setting"));
	font = QFont("Times", 16, 32, false);
	font.setBold(true);
	pe.setColor(QPalette::ButtonText, Qt::white);

	BaseView = new QWidget(this);
	BaseView->setFixedSize(500, 350);
	BaseView->setGeometry(QRect(0, 50, 500, 350));

	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr(""), this);
	BackButton->setStatusTip(tr("返回"));
	BackButton->setGeometry(QRect(0, 0, 50, 50));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));

	VH = new QRadioButton(BaseView);
	VH->setText(tr("Set The ToolBar To The Top"));
	VH->setGeometry(QRect(100, 110, 89, 16));

	Fram = new QRadioButton(BaseView);
	Fram->setText(tr("Set The Windows Framless"));
	Fram->setGeometry(QRect(100, 110, 89, 16));

	hSlider = new QSlider(BaseView);
	hSlider->setGeometry(QRect(100, 200, 160, 20));
	hSlider->setMaximum(100);
	hSlider->setSingleStep(1);
	hSlider->setOrientation(Qt::Horizontal);

	connect(BackButton, SIGNAL(clicked()), this, SLOT(Back()));
	connect(VH, SIGNAL(clicked(bool)), this, SLOT(setVH(bool)));
	connect(Fram, SIGNAL(clicked(bool)), this, SLOT(setFramless(bool)));
	connect(hSlider, SIGNAL(valueChanged(int)), this, SLOT(setOpacity(int)));
}

SetView::~SetView()
{

}

void SetView::setOpacity(int value)
{
	BaseWidget::v = (100.0 - value) / 100.0;
	this->setWindowOpacity(BaseWidget::v);
	//更新主窗体
	emit updateOpacity(value);
}

void SetView::setVH(bool value)
{
	//更新主窗体
	emit updateVH(value);
}

void SetView::setFramless(bool value)
{
	//更新主窗体
	emit updateFramless(value);
}

void SetView::Back()
{
	this->close();
}