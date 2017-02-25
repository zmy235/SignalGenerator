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
	setWindowTitle(tr("SetView"));

	//Font
	font = QFont("Times", 16, 32, false);
	font.setBold(true);

	//Color
	pe.setColor(QPalette::ButtonText, Qt::white);

	//
	BaseView = new QWidget(this);
	BaseView->setFixedSize(500, 350);
	BaseView->setGeometry(QRect(0, 50, 500, 350));

	//Back Button
	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr(""), this);
	BackButton->setStatusTip(tr("返回"));
	BackButton->setGeometry(QRect(0, 0, 50, 50));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));
	connect(BackButton, SIGNAL(clicked()), this, SLOT(Back()));

	VH = new QRadioButton(BaseView);
	VH->setObjectName(QStringLiteral("radioButton"));
	VH->setText(tr("Set The ToolBar To The Top"));
	VH->setGeometry(QRect(100, 110, 89, 16)); 
	connect(VH, SIGNAL(clicked(bool)), this, SLOT(setVH(bool)));

	//setOpacity
	hSlider = new QSlider(BaseView);
	hSlider->setObjectName(QStringLiteral("hSlider"));
	hSlider->setGeometry(QRect(100, 200, 160, 20));
	hSlider->setMaximum(100);
	hSlider->setSingleStep(1);
	hSlider->setOrientation(Qt::Horizontal);
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

void SetView::Back()
{
	this->close();
}