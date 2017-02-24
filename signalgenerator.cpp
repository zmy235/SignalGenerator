#include "SignalGenerator.h"
#include "BaseWidget.h"
#include "Task.h"
#include "AddView.h"
#include "SetView.h"
#include "AboutView.h"

#include<QDebug>
#include<QList>
#include<QMouseEvent>
#include<QRect>
#include<QPoint>
#include<QLayout>
#include<QDockWidget>
#include<QVariant> 
#include<QProgressBar>
#include<QListView>
#include<QPushButton>
#include<QLabel>
#include<QTimer>

//静态全局变量
QList<Task*>* MainWindow::taskList = new QList<Task*>();

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setWindowTitle(tr("Main Window"));
	ui.setupUi(this);

	//初始化
	ListNum = 0;
	WigetList = new QList<QWidget*>();
	setView = new SetView();
	toolBarView = true;

	//QTimer对象  
	QTimer* timer = new QTimer();
	timer->setInterval(2000);
	timer->start();
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));

	//初始化工具栏
	createActions();
	createToolBars();

	//设置可移动区域
	m_areaMovable = geometry();
	m_bPressed = false;

	//设置窗口大小
	setFixedSize(720, 500);

	//工具栏横竖
	addToolBar(toolBarView ? Qt::LeftToolBarArea : Qt::TopToolBarArea, MainToolBar); //TopToolBarArea

	//底页大小
	Base = new QWidget(this);
	Base->setFixedSize(670, 500);
	Base->setGeometry(QRect(toolBarView ? 60 : 0, toolBarView ? 0 : 60, 670, 500));

	//字体
	font = QFont("Cambria", 12, 28, false);
	font.setBold(true);
	font_pe.setColor(QPalette::WindowText, Qt::white);

	//透明无边框
	setWindowFlags(Qt::FramelessWindowHint);

	//设置QPalette对象的背景属性（颜色或图片）  
	palette.setBrush(QPalette::Background, QBrush(QPixmap("./Resources/background.png")));
	setPalette(palette);

	connect(setView, &SetView::updateVH, this, &MainWindow::updateVH);
	connect(setView, &SetView::updateOpacity, this, &MainWindow::updateOpacity);
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateVH(bool value)
{
	addToolBar(value ? Qt::TopToolBarArea : Qt::LeftToolBarArea, MainToolBar);
	Base->setGeometry(QRect(value ? 0 : 60, value ? 60 : 0, 670, 500));
	toolBarView = value;
}

void MainWindow::updateOpacity(int value)
{
	this->setWindowOpacity((100.0 - value) / 100.0);
}

void MainWindow::createActions()
{

	refresh = new QAction(QIcon("./Resources/list.png"), tr("&ReFresh"), this);
	refresh->setStatusTip(tr("add a new file"));
	connect(refresh, SIGNAL(triggered()), this, SLOT(createList()));

	add = new QAction(QIcon("./Resources/add.png"), tr("&New"), this);
	add->setStatusTip(tr("add a new file"));
	connect(add, SIGNAL(triggered()), this, SLOT(Add()));

	history = new QAction(QIcon("./Resources/history.png"), tr("&history"), this);
	history->setStatusTip(tr("history"));
	connect(history, SIGNAL(triggered()), this, SLOT(History()));

	lookfor = new QAction(QIcon("./Resources/lookfor.png"), tr("&lookfor"), this);
	lookfor->setStatusTip(tr("lookfor a file"));
	connect(lookfor, SIGNAL(triggered()), this, SLOT(Lookfor()));

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
	MainToolBar->addAction(refresh);
	MainToolBar->addAction(add);
	MainToolBar->addAction(history);
	MainToolBar->addAction(lookfor);
	MainToolBar->addAction(setting);
	MainToolBar->addAction(about);
	MainToolBar->addAction(shutdown);
	//insertToolBarBreak(AddToolBar);
}

//Mouse Action----------------------------------------------------------

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

//Main----------------------------------------------------------

void MainWindow::createList()
{
	TaskListHead = MainWindow::taskList->begin();
	TaskListSize = MainWindow::taskList->size();

	//主要根据taskList的具体信息来绘制界面
	for (auto i = TaskListHead + ListNum; ListNum < TaskListSize&&i != MainWindow::taskList->end(); i++, ListNum++)
	{
		qDebug() << (*i)->type << endl << (*i)->filePath << endl <<
			(*i)->name << endl << (*i)->size << endl << (*i)->progress << endl <<
			(*i)->state << endl << (*i)->time << endl << TaskListSize << endl;

		WigetList->push_back(new QWidget(Base));
		WigetList->at(ListNum)->setFixedSize(650, 45);
		WigetList->at(ListNum)->setGeometry(QRect(0, (i - TaskListHead) * 50, 650, 45));
		WigetList->at(ListNum)->setWindowFlags(Qt::FramelessWindowHint);
		WigetList->at(ListNum)->setPalette(palette);

		taskName = new QLabel(WigetList->at(ListNum));
		taskName->setText((*i)->name);
		taskName->setFixedHeight(45);
		taskName->setMaximumWidth(120);
		taskName->setGeometry(QRect(0, 0, 120, 45));
		taskName->setFont(font);
		taskName->setPalette(font_pe);

		progressBar = new QProgressBar(WigetList->at(ListNum));
		progressBar->setObjectName(QString::number(ListNum));//QString::fromUtf8("progressBar")
		progressBar->setFixedSize(300, 45);
		progressBar->setGeometry(QRect(130, 0, 300, 45));
		progressBar->setFont(font);
		progressBar->setInputMethodHints(Qt::ImhNone);
		progressBar->setValue((*i)->progress);
		progressBar->setAlignment(Qt::AlignCenter);
		progressBar->setTextVisible(true);
		progressBar->setInvertedAppearance(false);

		start = new QPushButton(QIcon("./Resources/start.png"), tr(""), WigetList->at(ListNum));
		start->setObjectName(QString::number(ListNum));
		start->setFixedSize(QSize(45, 45));
		start->setIconSize(QSize(45, 45));
		start->setGeometry(QRect(450, 0, 45, 45));
		connect(start, SIGNAL(clicked()), this, SLOT(Start()));

		stop = new QPushButton(QIcon("./Resources/stop.png"), tr(""), WigetList->at(ListNum));
		stop->setObjectName(QString::number(ListNum));
		stop->setFixedSize(QSize(45, 45));
		stop->setIconSize(QSize(45, 45));
		stop->setGeometry(QRect(500, 0, 45, 45));
		connect(stop, SIGNAL(clicked()), this, SLOT(Stop()));

		remove = new QPushButton(QIcon("./Resources/remove.png"), tr(""), WigetList->at(ListNum));
		remove->setObjectName(QString::number(ListNum));
		remove->setFixedSize(QSize(45, 45));
		remove->setIconSize(QSize(45, 45));
		remove->setGeometry(QRect(550, 0, 45, 45));
		connect(remove, SIGNAL(clicked()), this, SLOT(Remove()));

		info = new QPushButton(QIcon("./Resources/info.png"), tr(""), WigetList->at(ListNum));
		info->setObjectName(QString::number(ListNum));
		info->setFixedSize(QSize(45, 45));
		info->setIconSize(QSize(45, 45));
		info->setGeometry(QRect(600, 0, 45, 45));
		connect(info, SIGNAL(clicked()), this, SLOT(Info()));

		//for (auto j = WigetList->begin(); j != WigetList->end(); j++)
		//{
		//	(*j)->findChild<QProgressBar*>("progressBar", Qt::FindDirectChildrenOnly)->setValue(10 * (j - (WigetList->begin())));
		//}

		WigetList->at(ListNum)->show();
	}

}

void MainWindow::Add()
{
	AddView *addView = new AddView();
	addView->show();
}

void MainWindow::History()
{
	//测试项
	Task* test = new Task;
	test->name = QString::number(ListNum);
	test->size = 12000;
	test->filePath = "QQQ";
	test->time = QDateTime::currentDateTime();
	test->progress = 23;
	test->state = false;
	test->type = tr("Audio");
	MainWindow::taskList->append(test);
}

void MainWindow::Lookfor()
{

}

void MainWindow::Setting()
{
	setView->show();
}

void MainWindow::About()
{
	AboutView *aboutView = new AboutView();
	aboutView->show();
}

//List----------------------------------------------------------

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
		WigetList->at(i)->move(WigetList->at(i)->x() , 50 * i);
		qDebug() << i;
	}
}

void MainWindow::Start()
{

}

void MainWindow::Stop()
{

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

	qDebug() << str;
}