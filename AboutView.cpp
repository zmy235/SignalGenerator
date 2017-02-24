#include "SignalGenerator.h"
#include "BaseWidget.h"
#include "AboutView.h"
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
#include<QLabel>
#include<QComboBox>
#include<QUrl>
#include<QFileDialog>

AboutView::AboutView(QWidget *parent) : BaseWidget(parent)
{
	setWindowTitle(tr("AboutView"));

	//Font
	font = QFont("Cambria", 12, 26, false);
	font.setBold(true);

	//Color
	pe.setColor(QPalette::WindowText, Qt::white);

	//Back Button
	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr(""), this);
	BackButton->setStatusTip(tr("·µ»Ø"));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));
	BackButton->setGeometry(QRect(0, 0, 45, 45));
	connect(BackButton, SIGNAL(clicked()), this, SLOT(close()));

	text = new QLabel(this);
	text->setText(tr("This Program is Protected By Chinese Law.\n(C) 2016 NWPU CV&VR Lab.\n2016."));
	text->setFont(font);
	text->setPalette(pe);
	text->setGeometry(QRect(100, 60,360,200));
}

AboutView::~AboutView()
{

}
