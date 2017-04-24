#include "SignalGenerator.h"
#include "BaseWidget.h"
#include "AddAudio.h"
#include "Task.h"

#include<QUrl>
#include<QFileDialog>
#include<QDesktopServices>
#include<QMouseEvent>
#include<QRect>
#include<QPoint>
#include<QFont>
#include<QDockWidget>
#include<QPushButton>
#include<QTextEdit>
#include<QComboBox>

AddAudio::AddAudio(QWidget *parent) : BaseWidget(parent)
{
	setWindowTitle(tr("AddAudio"));

	createOption();

	//设置窗口大小
	setFixedSize(600, 500);
	setWindowOpacity(0.9);
	setWindowFlags(Qt::FramelessWindowHint);

	//设置QPalette对象的背景属性（颜色或图片）  
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QPixmap("./Resources/background.png")));
	setPalette(palette);

}

AddAudio::~AddAudio()
{

}

void AddAudio::createOption()
{

	//Font
	QFont font = QFont("Times", 16, 32, false);
	font.setBold(true);

	//Color
	QPalette pe;
	pe.setColor(QPalette::ButtonText, Qt::white);

	//comboBox
	comboBox->insertItems(0, QStringList() << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9");
	comboBox->setStatusTip(tr("返回"));
	comboBox->setFont(font);
	comboBox->setPalette(pe);
	connect(comboBox, SIGNAL(triggered()), this, SLOT(Add()));

	//Button
	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr("&Cancel"), this);
	BackButton->setStatusTip(tr("返回"));
	BackButton->setIconSize(QSize(45, 45));
	connect(BackButton, SIGNAL(triggered()), this, SLOT(Back()));

	AddButton = new QPushButton(QIcon("./Resources/ok.png"), tr("&OK"), this);
	AddButton->setStatusTip(tr("添加"));
	AddButton->setIconSize(QSize(45, 45));
	connect(AddButton, SIGNAL(triggered()), this, SLOT(OK()));

}


void AddAudio::Add()
{

	QString file_path = QFileDialog::getOpenFileName(this, "选择文件", "./", "Audio(*.mp3);;Vedio(*.mp4 *.mkv)");
	QFileInfo file_info = QFileInfo(file_path);
	QString file_name = file_info.fileName();
	QString ab_path = file_info.absolutePath();

	//设置停靠窗口1  
	QDockWidget *dock = new QDockWidget(tr("DockWindow"), this);
	dock->setFeatures(QDockWidget::DockWidgetMovable);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	QTextEdit *te1 = new QTextEdit();
	taskList;
	te1->setText(tr("Window1,The dock widget can be moved between docks by the user"));
	dock->setWidget(te1);
	//addDockWidget(Qt::RightDockWidgetArea, dock);

}


void AddAudio::OK()
{
	this->close();
}

void AddAudio::Back()
{
	this->close();
}