#include "SignalGenerator.h"
#include "BaseWidget.h"
#include "SetView.h"
#include "Task.h"

#include<Windows.h>
#include<QWidget>
#include<QSlider>
#include<QSpinBox>
#include<QPalette>
#include<QFont>
#include<QHBoxLayout>
#include<QRect>
#include<QPoint>
#include<QPushButton>
#include<QTextEdit>
#include<QCheckBox>
#include<QComboBox>
#include<QUrl>
#include<QFileDialog>

SetView::SetView(QWidget *parent) : BaseWidget(parent)
{
	setWindowTitle(QString::fromLocal8Bit("ÉèÖÃ"));
	setFixedSize(640, 480);

	QFont font;
	font = QFont("Times", 16, 32, false);
	font.setBold(true);

	QPalette pe;
	pe.setColor(QPalette::WindowText, Qt::white);

	VH = new QCheckBox(this);
	VH->setText(QString::fromLocal8Bit("²Ëµ¥À¸¶¥ÖÃ"));
	VH->setFont(font);
	VH->setPalette(pe);

	Fram = new QCheckBox(this);
	Fram->setText(QString::fromLocal8Bit("¼ò½à»¯´°¿Ú"));
	Fram->setFont(font);
	Fram->setPalette(pe);

	hSlider = new QSlider(this);
	hSlider->setWindowIconText(QString::fromLocal8Bit("Set The Windows Style"));
	hSlider->setOrientation(Qt::Horizontal);
	hSlider->setMaximum(100);
	hSlider->setFixedWidth(180);
	hSlider->setSingleStep(1);
	spinBox = new QSpinBox(this);
	spinBox->setFixedWidth(50);
	spinBox->setMaximum(100);

	QHBoxLayout *hlayout1 = new QHBoxLayout();
	hlayout1->setSpacing(50);
	hlayout1->addWidget(hSlider);
	hlayout1->addWidget(spinBox);

	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr(""), this);
	BackButton->setStatusTip(tr("·µ»Ø"));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));
	OKButton = new QPushButton(QIcon("./Resources/ok.png"), tr(""), this);
	OKButton->setStatusTip(tr("·µ»Ø"));
	OKButton->setFixedSize(QSize(45, 45));
	OKButton->setIconSize(QSize(45, 45));

	QHBoxLayout *hlayout = new QHBoxLayout();
	hlayout->setSpacing(50);
	hlayout->addWidget(BackButton);
	hlayout->addWidget(OKButton);

	QVBoxLayout *vlayout = new QVBoxLayout(this);
	vlayout->setSpacing(50);
	vlayout->setAlignment(Qt::AlignCenter);
	vlayout->addWidget(VH);
	vlayout->addWidget(Fram);
	vlayout->addLayout(hlayout1);
	vlayout->addLayout(hlayout);

	connect(VH, SIGNAL(stateChanged(int)), this, SLOT(setVH(int)));
	connect(Fram, SIGNAL(stateChanged(int)), this, SLOT(setFramless(int)));
	connect(hSlider, SIGNAL(valueChanged(int)), this, SLOT(setOpacity(int)));
	connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(setOpacity(int)));
	connect(BackButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(OKButton, SIGNAL(clicked()), this, SLOT(close()));
}

SetView::~SetView()
{
	delete VH;
	delete Fram;
	delete spinBox;
	delete hSlider;
	delete BackButton;
	delete OKButton;
}

void SetView::setOpacity(int value)
{
	BaseWidget::v = (100.0 - value) / 100.0;
	setWindowOpacity(BaseWidget::v);
	spinBox->setValue(value);
	hSlider->setValue(value);
	emit updateOpacity(value);
}

void SetView::setVH(int value)
{
	emit updateVH(value);
}

void SetView::setFramless(int value)
{
	if(value) setWindowFlags(Qt::FramelessWindowHint);
	else setWindowFlags(Qt::WindowCloseButtonHint);
	emit updateFramless(value);
	show();
}