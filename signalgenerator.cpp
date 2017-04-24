#include "SignalGenerator.h"
#include "BaseWidget.h"
#include "AddAudio.h"
#include "AddVedio.h"

#include<QUrl>
#include<QFileDialog>
#include<QDesktopServices>
#include<QMouseEvent>
#include<QRect>
#include<QPoint>
#include<QLayout>
#include<QProgressBar>
#include<QListView>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setWindowTitle(tr("Main Window"));

	ui.setupUi(this);

	createActions();
	createToolBars();

	//设置可移动区域
	m_areaMovable = geometry();
	m_bPressed = false;

	//设置窗口大小
	//setFixedSize(600, 500);

	//透明无边框
	setWindowOpacity(0.8);
	setWindowFlags(Qt::FramelessWindowHint);

	//设置QPalette对象的背景属性（颜色或图片）  
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QPixmap("./Resources/background.png")));
	setPalette(palette);
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{

	//MainToolBar

	list = new QAction(QIcon("./Resources/list.png"), tr("&list"), this);
	list->setStatusTip(tr("filelist"));
	connect(list, SIGNAL(triggered()), this, SLOT(List()));

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

	shutdown = new QAction(QIcon("./Resources/shutdown.png"), tr("&shutdown"), this);
	shutdown->setStatusTip(tr("exit"));
	connect(shutdown, SIGNAL(triggered()), this, SLOT(close()));

	about = new QAction(QIcon("./Resources/about.png"), tr("&about"), this);
	about->setStatusTip(tr("about"));
	connect(about, SIGNAL(triggered()), this, SLOT(About()));

	//AddToolBar

	back = new QAction(QIcon("./Resources/back.png"), tr("&Back"), this);
	back->setStatusTip(tr("back"));
	connect(back, SIGNAL(triggered()), this, SLOT(Back()));

	audio = new QAction(QIcon("./Resources/audio.png"), tr("&New Audio"), this);
	audio->setStatusTip(tr("add a new Audio file"));
	connect(audio, SIGNAL(triggered()), this, SLOT(AddAudioWin()));

	vedio = new QAction(QIcon("./Resources/vedio.png"), tr("&New Vedio"), this);
	vedio->setStatusTip(tr("add a new Vedio file"));
	connect(vedio, SIGNAL(triggered()), this, SLOT(AddVedioWin()));

	remove = new QAction(QIcon("./Resources/remove.png"), tr("&remove"), this);
	remove->setStatusTip(tr("remove a file"));
	connect(remove, SIGNAL(triggered()), this, SLOT(test()));

	stop = new QAction(QIcon("./Resources/stop.png"), tr("&stop"), this);
	stop->setStatusTip(tr("stop"));
	connect(stop, SIGNAL(triggered()), this, SLOT(test()));

	start = new QAction(QIcon("./Resources/start.png"), tr("&start"), this);
	start->setStatusTip(tr("start"));
	connect(start, SIGNAL(triggered()), this, SLOT(test()));

}

void MainWindow::createToolBars()
{
	//MainToolBar
	MainToolBar = addToolBar(tr("Tools"));
	MainToolBar->setObjectName(QStringLiteral("MainToolBar"));
	addToolBar(Qt::LeftToolBarArea, MainToolBar); //TopToolBarArea
	insertToolBarBreak(AddToolBar);
	MainToolBar->setMovable(false);
	MainToolBar->setFloatable(false);
	MainToolBar->setIconSize(QSize(45, 45));
	MainToolBar->addAction(list);
	MainToolBar->addAction(add);
	MainToolBar->addAction(history);
	MainToolBar->addAction(lookfor);
	MainToolBar->addAction(setting);
	MainToolBar->addAction(shutdown);
	MainToolBar->addAction(about);

	//AddToolBar
	AddToolBar = addToolBar(tr("AddToolBar"));
	AddToolBar->setVisible(false);
	AddToolBar->setObjectName(QStringLiteral("AddToolBar"));
	addToolBar(Qt::LeftToolBarArea, AddToolBar);
	insertToolBarBreak(AddToolBar);
	AddToolBar->setFloatable(false);
	AddToolBar->setMovable(false);
	AddToolBar->setIconSize(QSize(45, 45));
	AddToolBar->addAction(back);
	AddToolBar->addAction(audio);
	AddToolBar->addAction(vedio);
}

void MainWindow::createList()
{
	centralwidget = new QWidget(this);
	centralwidget->setObjectName(QStringLiteral("centralwidget"));

	progressBar = new QProgressBar(centralwidget);
	progressBar->setObjectName(QStringLiteral("progressBar"));
	progressBar->setGeometry(QRect(45, 0, 240, 40));
	QFont font;
	font.setFamily(QStringLiteral("Cambria"));
	font.setPointSize(12);
	progressBar->setFont(font);
	progressBar->setInputMethodHints(Qt::ImhNone);
	progressBar->setValue(60);
	progressBar->setAlignment(Qt::AlignCenter);
	progressBar->setTextVisible(true);
	progressBar->setInvertedAppearance(false);

	listView = new QListView(centralwidget);
	listView->setObjectName(QStringLiteral("listView"));
	listView->setGeometry(QRect(0, 0, 660, 450));




	tabWidget->setObjectName(QStringLiteral("tabWidget"));
	tabWidget->setGeometry(QRect(0, 0, 660, 500));
	tabWidget->setIconSize(QSize(45, 45));
	tab = new QWidget();
	tab->setObjectName(QStringLiteral("tab"));
	tabWidget->addTab(tab, QString());
	tab_2 = new QWidget();
	tab_2->setObjectName(QStringLiteral("tab_2"));
	tabWidget->addTab(tab_2, QString());
}

//Mouse Action----------------------------------------------------------

//鼠标左键
void MainWindow::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton)
	{
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

//Main Slots----------------------------------------------------------

void MainWindow::List()
{
	createList();
}

void MainWindow::Add()
{
	AddToolBar->setVisible(true);
}


void MainWindow::History()
{
}

void MainWindow::Lookfor()
{
}

void MainWindow::Setting()
{
	//AddToolBar->setOrientation(Qt::Vertical);//Horizontal
}

void MainWindow::About()
{
}

//Add Slots----------------------------------------------------------

void MainWindow::Back()
{
	AddToolBar->setVisible(false);
}

void MainWindow::AddAudioWin()
{
	AddAudio *Add_audio = new AddAudio();
	Add_audio->show();
}

void MainWindow::AddVedioWin()
{
	AddVedio *Add_audio = new AddVedio();
	Add_audio->show();
}

void MainWindow::Remove()
{

}

void MainWindow::Start()
{

}

void MainWindow::Stop()
{

}