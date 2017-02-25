#include "SignalGenerator.h"
#include "BaseWidget.h"
#include "AddView.h"
#include "Task.h"

#include<Windows.h>
#include<QWidget>
#include<QPalette>
#include<QFont>
#include<QHBoxLayout>
#include<QRect>
#include<QPoint>
#include<QDockWidget>
#include<QPushButton>
#include<QTextEdit>
#include<QComboBox>
#include<QUrl>
#include<QFileDialog>
#include<QMessageBox>
#include <QDebug>

AddView::AddView(QWidget *parent) : BaseWidget(parent)
{
	setWindowTitle(tr("AddView"));

	task = new Task();

	audioTypes = { tr("0"), tr("1"), tr("2"), tr("3"), tr("4"), tr("5"), tr("6"), tr("7"), tr("8"), tr("9") };
	vedioTypes = { tr("AV"), tr("VGA"), tr("RGB"), tr("YPbPr"), tr("LVDS"), tr("DVI"), tr("SDI"), tr("FPDLINK"), tr("CameraLink") };

	//Font
	font = QFont("Times", 16, 32, false);
	font.setBold(true);

	//Color
	pe.setColor(QPalette::ButtonText, Qt::white);

	//画布
	BaseView = new QWidget(this);
	BaseView->setFixedSize(500, 350);
	BaseView->setGeometry(QRect(0, 50, 500, 350));

	//Button
	BackButton = new QPushButton(QIcon("./Resources/back.png"), tr(""), this);
	BackButton->setStatusTip(tr("返回"));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));
	BackButton->setGeometry(QRect(0, 0, 45, 45));
	connect(BackButton, SIGNAL(clicked()), this, SLOT(close()));

	AudioButton = new QPushButton(QIcon("./Resources/audio.png"), tr(""), BaseView);
	AudioButton->setStatusTip(tr("添加音频"));
	AudioButton->setFixedSize(QSize(45, 45));
	AudioButton->setIconSize(QSize(45, 45));
	AudioButton->setGeometry(QRect(150, 100, 45, 45));
	connect(AudioButton, SIGNAL(clicked()), this, SLOT(addAudio()));

	VedioButton = new QPushButton(QIcon("./Resources/vedio.png"), tr(""), BaseView);
	VedioButton->setStatusTip(tr("添加视频"));
	VedioButton->setFixedSize(QSize(45, 45));
	VedioButton->setIconSize(QSize(45, 45));
	VedioButton->setGeometry(QRect(250, 100, 45, 45));
	connect(VedioButton, SIGNAL(clicked()), this, SLOT(addVedio()));

}

AddView::~AddView()
{

}

void AddView::addAudio()
{
	//隐藏BaseView
	BaseView->hide();

	task->type = "audio";

	//创建AudioView
	AudioView = new QWidget(this);
	AudioView->setFixedSize(500, 350);
	AudioView->setGeometry(QRect(0, 50, 500, 350));

	//AudioComboBox
	AudioComboBox = new QComboBox(AudioView);
	AudioComboBox->insertItems(0, QStringList() << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9");
	AudioComboBox->setFont(font);
	AudioComboBox->setStyleSheet("QComboBox { background-color: #454545; }");
	AudioComboBox->setGeometry(QRect(100, 0, 120, 45));
	connect(AudioComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_sel_audio(int)));

	//文件选择
	AudioFileButton = new QPushButton(QIcon("./Resources/file.png"), tr(""), AudioView);
	AudioFileButton->setStatusTip(tr("Choose a File"));
	AudioFileButton->setFixedSize(QSize(45, 45));
	AudioFileButton->setIconSize(QSize(45, 45));
	AudioFileButton->setGeometry(QRect(100, 50, 45, 45));
	connect(AudioFileButton, SIGNAL(clicked()), this, SLOT(AudioFile()));

	//取消
	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr(""), AudioView);
	BackButton->setStatusTip(tr("返回"));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));
	BackButton->setGeometry(QRect(120, 260, 45, 45));
	connect(BackButton, SIGNAL(clicked()), this, SLOT(AudioBack()));

	//确定
	AudioOKButton = new QPushButton(QIcon("./Resources/ok.png"), tr(""), AudioView);
	AudioOKButton->setStatusTip(tr("确认"));
	AudioOKButton->setFixedSize(QSize(45, 45));
	AudioOKButton->setIconSize(QSize(45, 45));
	AudioOKButton->setGeometry(QRect(250, 260, 45, 45));
	connect(AudioOKButton, SIGNAL(clicked()), this, SLOT(AudioOK()));

	AudioView->show();

}

void AddView::addVedio()
{
	//BaseView
	BaseView->hide();

	task->type = "vedio";

	VedioView = new QWidget(this);
	VedioView->setFixedSize(500, 350);
	VedioView->setGeometry(QRect(0, 50, 500, 350));

	//AudioComboBox
	VedioComboBox = new QComboBox(VedioView);
	VedioComboBox->insertItems(0, QStringList() << "AV" << "VGA" << "RGB" << "YPbPr" << "LVDS" << "DVI" << "SDI" << "FPDLINK" << "CameraLink");
	VedioComboBox->setStatusTip(tr("返回"));
	VedioComboBox->setFont(font);
	VedioComboBox->setPalette(pe);
	VedioComboBox->setGeometry(QRect(100, 0, 160, 45));
	VedioComboBox->setStyleSheet("QComboBox { background-color: #454545; }");
	connect(VedioComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_sel_vedio(int)));

	//VedioFileButton
	VedioFileButton = new QPushButton(QIcon("./Resources/file.png"), tr(""), VedioView);
	VedioFileButton->setStatusTip(tr("Choose a File"));
	VedioFileButton->setFixedSize(QSize(45, 45));
	VedioFileButton->setIconSize(QSize(45, 45));
	VedioFileButton->setGeometry(QRect(100, 50, 45, 45));
	connect(VedioFileButton, SIGNAL(clicked()), this, SLOT(VedioFile()));

	//Button
	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr(""), VedioView);
	BackButton->setStatusTip(tr("返回"));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));
	BackButton->setGeometry(QRect(120, 260, 45, 45));
	connect(BackButton, SIGNAL(clicked()), this, SLOT(VedioBack()));

	//Button
	VedioOKButton = new QPushButton(QIcon("./Resources/ok.png"), tr(""), VedioView);
	VedioOKButton->setStatusTip(tr("返回"));
	VedioOKButton->setFixedSize(QSize(45, 45));
	VedioOKButton->setIconSize(QSize(45, 45));
	VedioOKButton->setGeometry(QRect(250, 260, 45, 45));
	connect(VedioOKButton, SIGNAL(clicked()), this, SLOT(VedioOK()));

	VedioView->show();
}

void AddView::AudioFile()
{
	QString file_path = QFileDialog::getOpenFileName(this, "选择文件", "./", "Audio(*.mp3)");
	QFileInfo file_info = QFileInfo(file_path);
	int file_size = file_info.size();
	QString file_name = file_info.fileName();
	QString ab_path = file_info.absolutePath();
	task->filePath = file_path;
	task->name = file_name;
	task->size = file_size;
	//task->info = file_info;
}

void AddView::VedioFile()
{
	QString file_path = QFileDialog::getOpenFileName(this, "选择文件", "./", "Vedio(*.mp4 *.mkv)");
	QFileInfo file_info = QFileInfo(file_path);
	int file_size = file_info.size();
	QString file_name = file_info.fileName();
	QString ab_path = file_info.absolutePath();
	task->filePath = file_path;
	task->name = file_name;
	task->size = file_size;
	//task->info = file_info;
}

void AddView::on_sel_audio(const int &text)
{
	QString str = "You select " + audioTypes[text];
	QMessageBox::information(this, tr("Info"), str);
	//task->info = str;
}

void AddView::on_sel_vedio(const int &text)
{
	QString str = "You select " + vedioTypes[text];
	QMessageBox::information(this, tr("Info"), str);
	//task->info = str;
}

void AddView::AudioOK()
{
	task->time = QDateTime::currentDateTime();
	QString str = task->time.toString("yyyy-MM-dd hh:mm:ss ddd");
	task->progress = 0;
	task->state = false;
	task->type = tr("Audio");

	emit updateList(task);

	close();
}

void AddView::VedioOK()
{
	task->time = QDateTime::currentDateTime();
	task->progress = 0;
	task->state = false;
	task->type = tr("Vedio");

	emit updateList(task);

	close();
}

void AddView::AudioBack()
{
	AudioView->close();
	BaseView->show();
}

void AddView::VedioBack()
{
	VedioView->close();
	BaseView->show();
}