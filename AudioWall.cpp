#include "BaseWidget.h"
#include "AudioWall.h"
#include "AudioTask.h"
#include "WAVFile.h"

#include <QFile>  
#include <QAudioFormat>  
#include <QAudioOutput> 
#include <QAudioDeviceInfo>
#include <QMediaPlayer>
#include <QToolTip>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QFont>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QEvent>
#include <QMenu>
#include <QMessageBox>
#include <QPalette>
#include <QPushButton>
#include <QtCore/QUrl>
#include <QUrl>
#include <QWidget>

//extern "C"
//{
//#include <libavcodec\avcodec.h>
//#include <libavformat\avformat.h>
//#include <libswscale\swscale.h>
//#include <libswresample\swresample.h>
//}

AudioWall::AudioWall(QWidget *parent) : BaseWidget(parent)
{
	setWindowTitle(QString::fromLocal8Bit("音频输出窗口"));
	setFixedSize(650, 500);
	nth = 0;
	layout = new QVBoxLayout(this);
	layout->setAlignment(Qt::AlignTop);
	layout->setMargin(5);
	font = QFont("Cambria", 12, 28, false);
	font.setBold(true);
	font_pe.setColor(QPalette::WindowText, Qt::black);
	pe.setColor(QPalette::ButtonText, Qt::white);
	installEventFilter(this);
}

AudioWall::~AudioWall()
{
	for each (auto task in tasks) delete task;
}

bool AudioWall::eventFilter(QObject *watched, QEvent *event)
{
	if (event->type() == QEvent::ContextMenu)
	{
		QContextMenuEvent *e = static_cast<QContextMenuEvent*>(event);
		QMenu *menu = new QMenu();
		menu->addAction(tr("Add Task"), this, SLOT(addAudioView()));
		menu->addAction(tr("Start All"), this, SLOT(playAll()));
		menu->addAction(tr("Pause All"), this, SLOT(pauseAll()));
		menu->addAction(tr("Exit"), this, SLOT(close()));
		menu->popup(e->globalPos());
		menu->exec();
		return true;
	}
	return false;
}

void AudioWall::addAudioView()
{
	AddView = new BaseWidget();
	QVBoxLayout *vLayout = new QVBoxLayout(AddView);
	vLayout->setAlignment(Qt::AlignHCenter);
	vLayout->setSpacing(45);

	//设置采样率
	sampleRateComboBox = new QComboBox(AddView);
	sampleRateComboBox->setFixedSize(QSize(150, 45));
	sampleRateComboBox->insertItems(0, QStringList() << "48000" << "44100" << "32000" << "24000" << "22050" << "16000" << "12050" << "8000");
	sampleRateComboBox->setStatusTip(tr("SAMPLERATE"));
	sampleRateComboBox->setFont(font);
	sampleRateComboBox->setPalette(pe);
	sampleRateComboBox->setStyleSheet("QComboBox{ background - color: #454545; }");
	vLayout->addWidget(sampleRateComboBox);

	//设置通道
	m_deviceBox = new QComboBox(AddView);
	m_deviceBox->setFixedSize(QSize(150, 45));
	qDebug() << "l:" << QAudioDeviceInfo::defaultOutputDevice().deviceName();
	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
	{
		m_deviceBox->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
	}
	m_deviceBox->setStatusTip(tr("CHANNEL"));
	m_deviceBox->setFont(font);
	m_deviceBox->setPalette(pe);
	m_deviceBox->setStyleSheet("QComboBox{ background - color: #454545; }");
	vLayout->addWidget(m_deviceBox);

	//设置文件
	QPushButton *selectFileButton;
	selectFileButton = new QPushButton(QIcon("./Resources/file.png"), tr(""), AddView);
	selectFileButton->setStatusTip(tr("Choose a File"));
	selectFileButton->setFixedSize(QSize(45, 45));
	selectFileButton->setIconSize(QSize(45, 45));
	vLayout->addWidget(selectFileButton);

	QPushButton *BackButton;//取消
	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr(""), AddView);
	BackButton->setStatusTip(tr("返回"));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));
	QPushButton *addOKButton;//添加
	addOKButton = new QPushButton(QIcon("./Resources/ok.png"), tr(""), AddView);
	addOKButton->setStatusTip(tr("返回"));
	addOKButton->setFixedSize(QSize(45, 45));
	addOKButton->setIconSize(QSize(45, 45));

	QHBoxLayout *hLayout = new QHBoxLayout();
	hLayout->addWidget(BackButton);
	hLayout->addWidget(addOKButton);
	vLayout->addLayout(hLayout);

	connect(sampleRateComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setSampleRate(int)));
	connect(m_deviceBox, SIGNAL(activated(int)), this, SLOT(deviceChanged(int)));
	connect(selectFileButton, SIGNAL(clicked()), this, SLOT(openLocalFile()));
	connect(BackButton, SIGNAL(clicked()), AddView, SLOT(close()));
	connect(addOKButton, SIGNAL(clicked()), this, SLOT(AudioOK()));

	AddView->show();
}

void AudioWall::playAll()
{
	for each (auto task in tasks)
	{
		task->start();
	}
}

void AudioWall::pauseAll()
{
	for each (auto task in tasks)
	{
		task->pause();
	}
}

void AudioWall::setSampleRate(const int &index)
{
	sampleRate = sampleRateComboBox->itemText(index);
}

void AudioWall::deviceChanged(const int &index)
{
	taskName = QString::fromLocal8Bit("通道") + QString::number(index + 1);
	//taskName = m_deviceBox->itemText(index);
	m_device = m_deviceBox->itemData(index).value<QAudioDeviceInfo>();
}

void AudioWall::openLocalFile()
{
	/*
	aif        : AIFF (Apple/SGI)
	wav        : WAV (Microsoft)
	au         : AU (Sun/NeXT)
	caf        : CAF (Apple Core Audio File)
	flac       : FLAC (FLAC Lossless Audio Codec)
	snd        : AU (Sun/NeXT)
	svx        : IFF (Amiga IFF/SVX8/SV16)
	paf        : PAF (Ensoniq PARIS)
	fap        : PAF (Ensoniq PARIS)
	gsm        : RAW (header-less)
	nist       : WAV (NIST Sphere)
	htk        : HTK (HMM Tool Kit)
	ircam      : SF (Berkeley/IRCAM/CARL)
	sf         : SF (Berkeley/IRCAM/CARL)
	voc        : VOC (Creative Labs)
	w64        : W64 (SoundFoundry WAVE 64)
	raw        : RAW (header-less)
	mat4       : MAT4 (GNU Octave 2.0 / Matlab 4.2)
	mat5       : MAT5 (GNU Octave 2.1 / Matlab 5.0)
	mat        : MAT4 (GNU Octave 2.0 / Matlab 4.2)
	pvf        : PVF (Portable Voice Format)
	sds        : SDS (Midi Sample Dump Standard)
	sd2        : SD2 (Sound Designer II)
	vox        : RAW (header-less)
	xi         : XI (FastTracker 2)
	wve        : WVE (Psion Series 3)
	oga        : OGG (OGG Container format)
	mpc        : MPC (Akai MPC 2k)
	rf64       : RF64 (RIFF 64)
	*/
	file_path = QFileDialog::getOpenFileName(0, tr("Open an Audio File"), "./", "Vedio(*.mpc *.mp3 *.flac *.aiff *.ogg *.wav)");
	taskInfo = QFileInfo(file_path);
}

void AudioWall::AudioOK()
{
	QWidget *row = new QWidget(this);
	row->setFixedHeight(50);
	rows.append(row);
	layout->addWidget(row);

	QHBoxLayout *hLayout = new QHBoxLayout(row);
	hLayout->setSpacing(5);

	QLabel* taskNameLable = new QLabel(row);
	taskNameLable->setText(taskName);
	taskNameLable->setFixedHeight(45);
	taskNameLable->setFont(font);
	taskNameLable->setPalette(font_pe);

	QLabel* fileNameLable = new QLabel(row);
	fileNameLable->setText(taskInfo.fileName());
	fileNameLable->setFixedHeight(45);
	fileNameLable->setFont(font);
	fileNameLable->setPalette(font_pe);

	QSlider *TimeSlider = new QSlider(row);
	TimeSlider->setOrientation(Qt::Horizontal);
	TimeSlider->setTickPosition(QSlider::TicksAbove);//设置刻度
	TimeSlider->setTickInterval(5);
	TimeSlider->setMinimum(0);
	TimeSlider->setMaximum(100);
	TimeSlider->setValue(0);
	timeSliders.append(TimeSlider);

	QSlider *VolumeSlider = new QSlider(row);
	VolumeSlider->setOrientation(Qt::Vertical);
	VolumeSlider->setTracking(true);//实时改变
	VolumeSlider->setMinimum(0);
	VolumeSlider->setMaximum(100);
	VolumeSlider->setValue(100);
	volumeSliders.append(VolumeSlider);

	QPushButton* start = new QPushButton(QIcon("./Resources/start.png"), tr(""), row);
	start->setFixedSize(QSize(45, 45));
	start->setIconSize(QSize(45, 45));

	QPushButton* remove = new QPushButton(QIcon("./Resources/remove.png"), tr(""), row);
	remove->setFixedSize(QSize(45, 45));
	remove->setIconSize(QSize(45, 45));

	hLayout->addWidget(taskNameLable, 2);
	hLayout->addWidget(fileNameLable, 4);
	hLayout->addWidget(TimeSlider, 4);
	hLayout->addWidget(VolumeSlider, 1);
	hLayout->addWidget(start, 1);
	hLayout->addWidget(remove, 1);

	WAVFile *inputFile = new WAVFile();
	inputFile->open(file_path, QIODevice::ReadOnly);
	m_format.setSampleRate(sampleRate.toInt());//设置采样率  
	m_format.setChannelCount(1);//设置通道数  
	m_format.setSampleSize(16);//设置采样大小，一般为8位或16位  
	m_format.setCodec("audio/pcm");//设置编码方式  
	m_format.setByteOrder(QAudioFormat::LittleEndian);//设置字节序  
	m_format.setSampleType(QAudioFormat::UnSignedInt);//设置样本数据类型
	m_format = m_device.preferredFormat();
	QAudioOutput *m_audioOutput = new QAudioOutput(m_device, m_format);
	m_audioOutput->setNotifyInterval(1000);
	if (inputFile->isReadable())
	{
		AudioTask* task = new AudioTask(tr("Audio"), taskName, taskInfo, m_audioOutput, sampleRate);
		m_audioOutput->start(inputFile);
		players.append(m_audioOutput);
		task->file = inputFile;
		task->size = inputFile->size();
		task->timeSlider = TimeSlider;
		task->taskTime = QDateTime::currentDateTime();
		task->isFinished = false;
		inputFile->seek(0);
		emit updateAudioList(task);
		tasks.append(task);
		AddView->close();
		row->show();
	}
	else
	{
		QMessageBox::warning(0, tr("warnning"), tr("The file is unreadable!\n"));
		return;
	}

	connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(updateAudioState(QAudio::State)));
	connect(m_audioOutput, SIGNAL(notify()), this, SLOT(setSliderPosition()));
	connect(TimeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setPlayerPosition(int)));
	connect(VolumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));
	connect(start, SIGNAL(clicked()), this, SLOT(setState()));
	connect(remove, SIGNAL(clicked()), this, SLOT(Remove()));
}

void AudioWall::setAudioState(QAudio::State state)
{
	emit updateAudioState(nth);
}

void AudioWall::setState()
{
	QPushButton *test = qobject_cast<QPushButton *>(sender());
	QWidget *w = test->parentWidget();
	nth = w->y() / 50;
	if (tasks[nth]->getState() == QAudio::ActiveState)
	{
		tasks[nth]->stop();
		test->setIcon(QIcon("./Resources/start.png"));
	}
	else
	{
		tasks[nth]->start();
		test->setIcon(QIcon("./Resources/stop.png"));
	}
	emit updateAudioState(nth);
}

void AudioWall::Remove()
{
	QPushButton *test = qobject_cast<QPushButton *>(sender());
	QWidget *w = test->parentWidget();
	nth = w->y() / 50;
	if (!tasks[nth]->audio->signalsBlocked())
	{
		tasks[nth]->audio->blockSignals(true);
		tasks[nth]->audio->deleteLater();
		tasks[nth]->isFinished = true;
	}
	w->close();
	emit updateAudioList(tasks[nth]);
	tasks[nth]->record();
	tasks.removeAt(nth);
	players.removeAt(nth);
	timeSliders.removeAt(nth);
	volumeSliders.removeAt(nth);
	rows.removeAt(nth);

}

void AudioWall::setPlayerPosition(int value)
{
	QSlider *test = qobject_cast<QSlider *>(sender());
	QWidget *w = test->parentWidget();
	nth = w->y() / 50;
	qDebug() << nth << ": " << value;
	tasks[nth]->file->seek(tasks[nth]->size / 100 * value);
	test->setToolTip(QString::number(value));

	emit updateAudioProgress(nth, value);
}

void AudioWall::setSliderPosition()
{
	static int processed = 0;
	static int position = 0;
	QAudioOutput *audio = qobject_cast<QAudioOutput *>(sender());
	nth = players.indexOf(audio);
	QSlider *slider = tasks[nth]->timeSlider;
	int n = audio->bufferSize();
	if (position != slider->sliderPosition()) //如果进度条被拖放，则将该位置置为起点
		processed += slider->sliderPosition() * tasks[nth]->size / 100 + n;
	else
		processed += n;
	qDebug() << "size: " << tasks[nth]->size << " \t " << processed;
	int value = processed * 100 / tasks[nth]->size;
	slider->setValue(value);
	position = slider->sliderPosition();

	emit updateAudioProgress(nth, value);
}

void AudioWall::setVolume(int value)
{
	QSlider *test = qobject_cast<QSlider *>(sender());
	QWidget *w = test->parentWidget();
	nth = w->y() / 50;
	qDebug() << nth << "\n" << value;
	QAudioOutput *temp = tasks[nth]->audio;
	if (temp) temp->setVolume(value*1.0 / 100.0);
	test->setToolTip(QString::number(value));
}
