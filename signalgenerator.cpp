#include "SignalGenerator.h"
#include "BaseWidget.h"
#include "Task.h"
#include "HistoryView.h"
#include "FindView.h"
#include "SetView.h"
#include "AboutView.h"
#include "VideoWall.h"

#include<QDebug>
#include<QList>
#include<QMouseEvent>
#include<QRect>
#include<QPoint>
#include<QLayout>
#include<QScrollBar>
#include<QScrollArea>
#include<QDockWidget>
#include<QVariant> 
#include<QProgressBar>
#include<QListView>
#include<QPushButton>
#include<QLabel>
#include<QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setWindowTitle(tr("Main Window"));
	ui.setupUi(this);

	//----------------------------------------------------------

	//全局变量初始化
	ListNum = 0;
	taskList = new QList<Task*>();
	WigetList = new QList<QWidget*>();

	//窗口对象初始化

	vLayout = new QVBoxLayout();
	vLayout->setAlignment(Qt::AlignTop);
	//vLayout->setSizeConstraint();

	Base = new BaseWidget(this);
	Base->setGeometry(QRect(60, 0, this->width()-60, this->height()));
	//Base->setMinimumSize(640, 480);
	Base->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	scrollArea = new QScrollArea(this);
	//scrollArea->setMinimumSize(640, 480);
	scrollArea->setGeometry(QRect(60, 0, this->width() - 60, this->height()));
	scrollArea->setWidget(Base);
	scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	historyView = new HistoryView();
	findView = new FindView();
	setView = new SetView();
	aboutView = new AboutView();
	audioView = new BaseWidget();
	videoView = new BaseWidget();

	wall = new VideoWall();
	wall->setRows(3);
	wall->setCols(3);
	wall->setView(videoView);

	//----------------------------------------------------------

	//设置可移动区域
	m_areaMovable = geometry();
	m_bPressed = false;

	//设置窗口大小
	setMinimumSize(640, 480);

	//初始化工具栏
	createActions();
	createToolBars();
	addToolBar(Qt::LeftToolBarArea, MainToolBar); //TopToolBarArea

	//字体
	font = QFont("Cambria", 12, 28, false);
	font.setBold(true);
	font_pe.setColor(QPalette::WindowText, Qt::white);

	//透明无边框
	//setWindowFlags(Qt::FramelessWindowHint);
	//setWindowFlags(Qt::CustomizeWindowHint);

	//设置QPalette对象的背景属性（颜色或图片）  
	palette.setBrush(QPalette::Background, QBrush(QPixmap("./Resources/bg0.png")));
	setPalette(palette);

	//----------------------------------------------------------

	connect(setView, &SetView::updateVH, this, &MainWindow::updateVH);
	connect(setView, &SetView::updateOpacity, this, &MainWindow::updateOpacity);
	connect(wall, &VideoWall::updateList, this, &MainWindow::updateList);

}

MainWindow::~MainWindow()
{
	historyView->close();
	findView->close();
	setView->close();
	aboutView->close();
	//delete audioView;
	wall->stop();
	wall->close(); 
}

void MainWindow::createActions()
{
	audio = new QAction(QIcon("./Resources/audio.png"), tr("&audio"), this);
	audio->setStatusTip(tr("audio"));
	connect(audio, SIGNAL(triggered()), this, SLOT(AudioView()));

	video = new QAction(QIcon("./Resources/video.png"), tr("&video"), this);
	video->setStatusTip(tr("video"));
	connect(video, SIGNAL(triggered()), this, SLOT(VideoView()));

	history = new QAction(QIcon("./Resources/history.png"), tr("&history"), this);
	history->setStatusTip(tr("history"));
	connect(history, SIGNAL(triggered()), this, SLOT(History()));

	lookfor = new QAction(QIcon("./Resources/lookfor.png"), tr("&lookfor"), this);
	lookfor->setStatusTip(tr("lookfor a file"));
	connect(lookfor, SIGNAL(triggered()), this, SLOT(Find()));

	setting = new QAction(QIcon("./Resources/setting.png"), tr("&setting"), this);
	setting->setStatusTip(tr("setting"));
	connect(setting, SIGNAL(triggered()), this, SLOT(Setting()));

	about = new QAction(QIcon("./Resources/about.png"), tr("&about"), this);
	about->setStatusTip(tr("about"));
	connect(about, SIGNAL(triggered()), this, SLOT(About()));

	shutdown = new QAction(QIcon("./Resources/shutdown.png"), tr("&shutdown"), this);
	shutdown->setStatusTip(tr("exit"));
	connect(shutdown, SIGNAL(triggered()), this, SLOT(close()));

}

void MainWindow::createToolBars()
{
	MainToolBar = addToolBar(tr("Tools"));
	MainToolBar->setObjectName(QStringLiteral("MainToolBar"));
	MainToolBar->setMovable(false);
	MainToolBar->setFloatable(false);
	MainToolBar->setIconSize(QSize(45, 45));
	MainToolBar->addAction(audio);
	MainToolBar->addAction(video);
	MainToolBar->addAction(history);
	MainToolBar->addAction(lookfor);
	MainToolBar->addAction(setting);
	MainToolBar->addAction(about);
	MainToolBar->addAction(shutdown);
	//insertToolBarBreak(AddToolBar);
}

//Actions

//鼠标左键
void MainWindow::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
		qDebug() << e->pos();
		m_ptPress = e->pos();
		m_bPressed = m_areaMovable.contains(m_ptPress);
	}
}

//鼠标
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
	if (m_bPressed)
	{
		move(pos() + e->pos() - m_ptPress);
	}
}

//鼠标
void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
	m_bPressed = false;
}

//设置鼠标按下的区域
void MainWindow::setAreaMovable(const QRect rt)
{
	if (m_areaMovable != rt)
	{
		m_areaMovable = rt;
	}
}

//slot:

void MainWindow::updateList(Task* task)
{
	taskList->append(task);
	TaskListHead = taskList->begin();
	TaskListSize = taskList->size();

	qDebug()
		<< "============================================================="
		<< task->type << endl
		<< task->filePath << endl
		<< task->name << endl
		<< task->size << endl
		<< task->progress << endl
		<< task->playing << endl
		<< task->finished << endl
		<< task->time << endl
		<< TaskListSize << "  ListNum："<<ListNum<<endl
		<< "=============================================================";

	QHBoxLayout *hLayout = new QHBoxLayout();
	hLayout->setSpacing(5);
	vLayout->addLayout(hLayout);

	QWidget *row = new QWidget(Base);
	row->setFixedHeight(45);
	row->setWindowFlags(Qt::FramelessWindowHint);
	row->setPalette(palette);
	WigetList->push_back(row);

	taskName = new QLabel(row);
	taskName->setText(task->name);
	taskName->setFixedHeight(45);
	taskName->setFont(font);
	taskName->setPalette(font_pe);
	hLayout->addWidget(taskName,26);

	progressBar = new QProgressBar(row);
	progressBar->setObjectName(QString::fromUtf8("progressBar") + QString::number(ListNum));
	progressBar->setFixedHeight(45);
	progressBar->setFont(font);
	progressBar->setInputMethodHints(Qt::ImhNone);
	progressBar->setValue(10 * ListNum);//task->progress
	progressBar->setAlignment(Qt::AlignCenter);
	progressBar->setTextVisible(true);
	progressBar->setInvertedAppearance(false);
	hLayout->addWidget(progressBar, 26);

	start = new QPushButton(QIcon("./Resources/start.png"), tr(""), row);
	start->setObjectName(QString::fromUtf8("start"));
	start->setFixedSize(QSize(45, 45));
	start->setIconSize(QSize(45, 45));
	start->setGeometry(QRect(450, 0, 45, 45));
	hLayout->addWidget(start, 10);

	stop = new QPushButton(QIcon("./Resources/stop.png"), tr(""), row);
	stop->setObjectName(QString::fromUtf8("stop"));
	stop->setFixedSize(QSize(45, 45));
	stop->setIconSize(QSize(45, 45));
	stop->setGeometry(QRect(500, 0, 45, 45));
	hLayout->addWidget(stop, 10);

	remove = new QPushButton(QIcon("./Resources/remove.png"), tr(""), row);
	remove->setObjectName(QString::number(ListNum));
	remove->setFixedSize(QSize(45, 45));
	remove->setIconSize(QSize(45, 45));
	remove->setGeometry(QRect(550, 0, 45, 45));
	hLayout->addWidget(remove, 10);

	info = new QPushButton(QIcon("./Resources/info.png"), tr(""), row);
	info->setObjectName(QString::number(ListNum));
	info->setFixedSize(QSize(45, 45));
	info->setIconSize(QSize(45, 45));
	info->setGeometry(QRect(600, 0, 45, 45));
	hLayout->addWidget(info, 10);


	connect(start, SIGNAL(clicked()), this, SLOT(Start()));
	connect(stop, SIGNAL(clicked()), this, SLOT(Stop()));
	connect(remove, SIGNAL(clicked()), this, SLOT(Remove()));
	connect(info, SIGNAL(clicked()), this, SLOT(Info()));

	//for (auto j = WigetList->begin(); j != WigetList->end(); j++)
	//{
	//	(*j)->findChild<QProgressBar*>("progressBar", Qt::FindDirectChildrenOnly)->setValue(10 * (j - (WigetList->begin())));
	//}

	row->show();

	ListNum++;

}

void MainWindow::updateVH(bool value)
{
	addToolBar(value ? Qt::TopToolBarArea : Qt::LeftToolBarArea, MainToolBar);
	Base->setGeometry(QRect(value ? 0 : 60, value ? 60 : 0, 670, 500));
}

void MainWindow::updateOpacity(int value)
{
	this->setWindowOpacity((100.0 - value) / 100.0);
	this->historyView->setWindowOpacity((100.0 - value) / 100.0);
	this->aboutView->setWindowOpacity((100.0 - value) / 100.0);
	this->findView->setWindowOpacity((100.0 - value) / 100.0);
}

//Menu

void MainWindow::AudioView()
{
	;
}

void MainWindow::VideoView()
{
	wall->show();
}

void MainWindow::History()
{
	historyView->show();
}

void MainWindow::Find()
{
	findView->show();
}

void MainWindow::Setting()
{
	setView->show();
}

void MainWindow::About()
{
	aboutView->show();
}

//List

void MainWindow::Remove()
{
	QPushButton *test = qobject_cast<QPushButton *>(sender());
	QWidget *w = test->parentWidget();

	//获取项的坐标
	//int op = test->objectName().toInt();
	int op = WigetList->indexOf(w);

	//关闭点击的项的窗口
	//WigetList->at(op)->close();
	w->close();

	//擦除组件
	WigetList->erase(WigetList->begin() + op);

	//擦除对应任务列表项
	MainWindow::taskList->erase(taskList->begin() + op);

	//已绘制组件数减1
	ListNum--;

	auto j = WigetList->begin();
	//被删除一行下面的所有组件向上移动一行

	for (int i = op; i < ListNum && j != WigetList->end(); i++)
	{
		(*(j + i))->setObjectName(QString::number(op));
		WigetList->at(i)->move(WigetList->at(i)->x(), 50 * i);
		qDebug() << i;
	}
}

void MainWindow::Start_Stop()
{
	QPushButton *test = qobject_cast<QPushButton *>(sender());
	QWidget *w = test->parentWidget();
	int i = 0;
	while (WigetList->at(i) != w ) i++;
	auto player = wall->players.at(i);
	if (player->isPlaying())
	{
		taskList->at(i)->playing = false;
		player->pause(!player->isPaused());
		test->setIcon(QIcon("./Resources/stop.png"));
	}
	else
	{
		taskList->at(i)->playing = true;
		player->play();
		test->setIcon(QIcon("./Resources/start.png"));
	}
}

void MainWindow::Info()
{
	//获取控件指针
	QPushButton *test = qobject_cast<QPushButton *>(sender());

	//获取控件父窗口
	QWidget *w = test->parentWidget();

	//获取项的坐标
	int op = WigetList->indexOf(w);

	//对应任务列表项
	QString str = MainWindow::taskList->at(op)->filePath;
	QString str2 = MainWindow::taskList->at(op)->type;

	qDebug() << str << str2;
}