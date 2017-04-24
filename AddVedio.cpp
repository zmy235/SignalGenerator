#include "SignalGenerator.h"
#include "AddVedio.h"
#include<QUrl>
#include<QFileDialog>
#include<QDesktopServices>
#include<QMouseEvent>
#include<QRect>
#include<QPoint>

AddVedio::AddVedio(QWidget *parent) : BaseWidget(parent)
{
	setWindowTitle(tr("AddVedio"));
	createActions();
	createToolBars();

	//设置窗口大小
	setFixedSize(600, 500);
	setWindowOpacity(0.8);
	setWindowFlags(Qt::FramelessWindowHint);

	//设置QPalette对象的背景属性（颜色或图片）  
	QPalette palette;
	palette.setBrush(QPalette::Background, QBrush(QPixmap("./Resources/background.png")));
	setPalette(palette);

}

AddVedio::~AddVedio()
{

}


void AddVedio::createActions()
{

	AV = new QAction( tr("&AV"), this);
	AV->setStatusTip(tr("add a AV file"));
	connect(AV, SIGNAL(triggered()), this, SLOT(Add()));

	VGA = new QAction( tr("&VGA"), this);
	VGA->setStatusTip(tr("add a AV file"));
	connect(VGA, SIGNAL(triggered()), this, SLOT(Add()));

	RGB = new QAction(tr("&RGB"), this);
	RGB->setStatusTip(tr("add a AV file"));
	connect(RGB, SIGNAL(triggered()), this, SLOT(Add()));

	YPbPr = new QAction( tr("&YPbPr"), this);
	YPbPr->setStatusTip(tr("add a new file"));
	connect(YPbPr, SIGNAL(triggered()), this, SLOT(Add()));

	DVI = new QAction(tr("&DVI"), this);
	DVI->setStatusTip(tr("add a new file"));
	connect(DVI, SIGNAL(triggered()), this, SLOT(Add()));

	LVDS = new QAction(tr("&LVDS"), this);
	LVDS->setStatusTip(tr("add a AV file"));
	connect(LVDS, SIGNAL(triggered()), this, SLOT(Add()));

	SDI = new QAction( tr("&SDI"), this);
	SDI->setStatusTip(tr("add a AV file"));
	connect(SDI, SIGNAL(triggered()), this, SLOT(Add()));

	FPDLink = new QAction( tr("&FPDLink"), this);
	FPDLink->setStatusTip(tr("add a new file"));
	connect(FPDLink, SIGNAL(triggered()), this, SLOT(Add()));

	CameraLink = new QAction( tr("&CameraLink"), this);
	CameraLink->setStatusTip(tr("add a AV file"));
	connect(CameraLink, SIGNAL(triggered()), this, SLOT(Add()));

	back = new QAction(QIcon("./Resources/back.png"), tr("&Back"), this);
	back->setStatusTip(tr("BACK"));
	connect(back, SIGNAL(triggered()), this, SLOT(Back()));
}

void AddVedio::createToolBars()
{
	VedioToolBar = new QToolBar(tr("Vudio"), this);
	VedioToolBar->setObjectName(QStringLiteral("VedioToolBar"));
	VedioToolBar->setOrientation(Qt::Vertical);//Horizontal
	VedioToolBar->setMovable(false);
	VedioToolBar->setIconSize(QSize(45, 45));
	VedioToolBar->setMaximumWidth(90);

	//Font
	QFont font = QFont("Times", 16, 32, false);
	font.setBold(true);
	VedioToolBar->setFont(font);

	//Color
	QPalette pe;
	pe.setColor(QPalette::ButtonText, Qt::white);
	VedioToolBar->setPalette(pe);

	VedioToolBar->addAction(back);
	VedioToolBar->addAction(AV);
	VedioToolBar->addAction(VGA);
	VedioToolBar->addAction(RGB);
	VedioToolBar->addAction(YPbPr);
	VedioToolBar->addAction(DVI);
	VedioToolBar->addAction(LVDS);
	VedioToolBar->addAction(SDI);
	VedioToolBar->addAction(FPDLink);
	VedioToolBar->addAction(CameraLink);
}


void AddVedio::Add()
{

	QString file_path = QFileDialog::getOpenFileName(this, "选择文件", "./", "Vedio(*.mp4 *.mkv)");
	QFileInfo file_info = QFileInfo(file_path);
	QString file_name = file_info.fileName();
	QString ab_path = file_info.absolutePath();

}

void AddVedio::Back()
{
	this->close();
}