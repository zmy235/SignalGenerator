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

	QFont font = QFont("Cambria", 12, 26, false);
	font.setBold(true);
	QPalette pe;
	pe.setColor(QPalette::WindowText, Qt::white);

	text = new QLabel(this);
	text->setText(tr("This Program is Protected By Chinese Law.\n(C) 2016 NWPU CV&VR Lab.\n2016."));
	text->setContentsMargins(50,50,50,100);
	text->setFont(font);
	text->setPalette(pe);

	BackButton = new QPushButton(QIcon("./Resources/ok.png"), tr(""), this);
	BackButton->setStatusTip(tr("·µ»Ø"));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));

	QVBoxLayout * layout = new QVBoxLayout(this);
	layout->setSpacing(50);
	layout->setAlignment(Qt::AlignCenter);
	layout->addWidget(text);
	layout->addWidget(BackButton);

	connect(BackButton, SIGNAL(clicked()), this, SLOT(close()));
}

AboutView::~AboutView()
{

}
