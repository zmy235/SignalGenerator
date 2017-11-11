#include "BaseWidget.h"
#include "AudioWall.h"
#include "AudioTask.h"

#include <QFile>  
#include <QAudioFormat>  
#include <QAudioOutput> 
#include <QAudioDeviceInfo>
#include <QSoundEffect>
#include <QComboBox>
#include <QDebug>
#include <QFileDialog>
#include <QFont>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QPalette>
#include <QPushButton>
#include <QtCore/QUrl>
#include <QUrl>
#include <QWidget>

const qreal kVolumeInterval = 0.04;

AudioWall::AudioWall(QWidget *parent) : BaseWidget(parent)
{
	setWindowTitle(tr("Audio Wall"));
	setFixedSize(650,500);
	font = QFont("Cambria", 12, 28, false);
	font.setBold(true);
	font_pe.setColor(QPalette::WindowText, Qt::white);
	pe.setColor(QPalette::ButtonText, Qt::white);
	for (int i = 0; i < 10; ++i)
	{
		QPushButton *addButton = new QPushButton(QIcon("./Resources/add.png"), tr(""), this);
		addButton->setObjectName(QString::fromUtf8("addButton") + QString::number(i));
		addButton->setStatusTip(tr("添加"));
		addButton->setFixedSize(QSize(45, 45));
		addButton->setIconSize(QSize(45, 45));
		addButton->setGeometry(5, 50 * i, 45, 45);
		addButtons.append(addButton);
		connect(addButton, SIGNAL(clicked()), this, SLOT(addAudioView()));
	}
}

AudioWall::~AudioWall()
{

}

void AudioWall::addAudioView()
{
	QPushButton *test = qobject_cast<QPushButton *>(sender());
	nth = test->y() / 50;

	AddView = new BaseWidget();
	QVBoxLayout *vLayout = new QVBoxLayout();
	vLayout->setAlignment(Qt::AlignHCenter);
	vLayout->setSpacing(45);
	AddView->setLayout(vLayout);

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
	QPushButton *VedioFileButton;
	VedioFileButton = new QPushButton(QIcon("./Resources/file.png"), tr(""), AddView);
	VedioFileButton->setStatusTip(tr("Choose a File"));
	VedioFileButton->setFixedSize(QSize(45, 45));
	VedioFileButton->setIconSize(QSize(45, 45));
	vLayout->addWidget(VedioFileButton);

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
	connect(VedioFileButton, SIGNAL(clicked()), this, SLOT(openLocalFile()));
	connect(BackButton, SIGNAL(clicked()), AddView, SLOT(close()));
	connect(addOKButton, SIGNAL(clicked()), this, SLOT(AudioOK()));

	AddView->show();
}

void AudioWall::setSampleRate(const int &index)
{
	sampleRate = sampleRateComboBox->itemText(index);;
}

void AudioWall::deviceChanged(const int &index)
{
	taskName = QString::fromLocal8Bit("通道") + QString::number(index+1);
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
	file_path = QFileDialog::getOpenFileName(0, tr("Open an Audio File"), "./", "Vedio(*.mp3, *.flac, *.aiff, *.ogg, *.wav)");
	taskInfo = QFileInfo(file_path);
}

void AudioWall::AudioOK()
{
	QAudioFormat audioFormat;//设置采样格式 
	audioFormat.setSampleRate(sampleRate.toInt());//设置采样率  
	audioFormat.setChannelCount(1);//设置通道数  
	audioFormat.setSampleSize(16);//设置采样大小，一般为8位或16位  
	audioFormat.setCodec("audio/pcm");//设置编码方式  
	audioFormat.setByteOrder(QAudioFormat::LittleEndian);//设置字节序  
	audioFormat.setSampleType(QAudioFormat::UnSignedInt);//设置样本数据类型

	QWidget *row = new QWidget(this);
	row->setFixedHeight(45);
	row->setWindowFlags(Qt::FramelessWindowHint);
	row->setGeometry(5, nth * 50, 650, 45);
	QHBoxLayout *hLayout = new QHBoxLayout(row);
	hLayout->setSpacing(5);

	QLabel* taskNameLable = new QLabel(row);
	taskNameLable->setText(taskName);
	taskNameLable->setFixedHeight(45);
	taskNameLable->setFont(font);
	taskNameLable->setPalette(font_pe);

	TimeSlider = new Slider(row);
	TimeSlider->setTracking(true);
	TimeSlider->setOrientation(Qt::Horizontal);
	TimeSlider->setMinimum(0);

	VolumeSlider = new Slider(row);
	VolumeSlider->setOrientation(Qt::Vertical);
	VolumeSlider->setMinimum(0);

	QPushButton* start = new QPushButton(QIcon("./Resources/start.png"), tr(""), row);
	start->setObjectName(QString::fromUtf8("start"));
	start->setFixedSize(QSize(45, 45));
	start->setIconSize(QSize(45, 45));

	QPushButton* remove = new QPushButton(QIcon("./Resources/remove.png"), tr(""), row);
	remove->setObjectName(QString::number(nth));
	remove->setFixedSize(QSize(45, 45));
	remove->setIconSize(QSize(45, 45));

	hLayout->addWidget(taskNameLable, 5);
	hLayout->addWidget(TimeSlider, 4);
	hLayout->addWidget(VolumeSlider, 2);
	hLayout->addWidget(start, 2);
	hLayout->addWidget(remove, 2);


	QIODevice *inputFile = new QFile(file_path);
	inputFile->open(QIODevice::ReadOnly);
	m_audioOutput = new QAudioOutput(m_device,m_device.preferredFormat());
	if (inputFile->isReadable())
	{
		m_audioOutput->start(inputFile);
		AudioTask* task = new AudioTask(tr("Audio"), taskName, taskInfo, m_audioOutput, sampleRate);
		task->taskTime = QDateTime::currentDateTime();
		task->isFinished = false;
		task->isPlaying = true;
		emit updateAudioList(task);
		tasks.append(task);
	}
	else
	{
		QMessageBox::warning(0, tr("warnning"), tr("The file is unreadable!\n"));
		return;
	}

	connect(m_audioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audio0(QAudio::State)));
	connect(TimeSlider, SIGNAL(sliderPressed()), SLOT(seek()));
	connect(TimeSlider, SIGNAL(sliderReleased()), SLOT(seek()));
	connect(TimeSlider, SIGNAL(onLeave()), SLOT(onTimeSliderLeave()));
	connect(TimeSlider, SIGNAL(onHover(int, int)), SLOT(onTimeSliderHover(int, int)));
	connect(VolumeSlider, SIGNAL(sliderPressed()), SLOT(setVolume()));
	connect(VolumeSlider, SIGNAL(valueChanged(int)), SLOT(setVolume()));
	connect(start, SIGNAL(clicked()), this, SLOT(Start_Stop()));
	connect(remove, SIGNAL(clicked()), this, SLOT(Remove()));

	addButtons[nth]->hide();
	row->show();
	AddView->close();
}

void AudioWall::SetState()
{
	QPushButton *test = qobject_cast<QPushButton *>(sender());
	nth = test->y() / 50;
	if (nth < tasks.size())
		if (!tasks[nth]->audio->signalsBlocked())
		{
			tasks[nth]->audio->blockSignals(true);
			test->setIcon(QIcon("./Resources/stop.png"));
		}
		else
		{
			tasks[nth]->audio->start();
			test->setIcon(QIcon("./Resources/start.png"));
		}

	emit updateAudioState(nth);
}

void AudioWall::Remove()
{
	QPushButton *test = qobject_cast<QPushButton *>(sender());
	QWidget *w = test->parentWidget();
	nth = w->y() / 50;
	w->close();
	addButtons[nth]->show();
	tasks[nth]->isFinished = true;
	emit updateAudioList(tasks[nth]);
	tasks.removeAt(nth);
}

void AudioWall::seek()
{
	//players[nth]->setSeekType(AccurateSeek);
	//players[nth]->((qint64)TimeSlider->value());
}

void AudioWall::onTimeSliderLeave()
{
	//if (m_preview && m_preview->isVisible())
	//	m_preview->hide();
}

void AudioWall::onTimeSliderHover(int pos, int value)
{
	//QPoint gpos = mapToGlobal(TimeSlider->pos() + QPoint(pos, 0));
	//QToolTip::showText(gpos, QTime(0, 0, 0).addMSecs(value).toString(QString::fromLatin1("HH:mm:ss")));
	//if (!m_preview)
	//	m_preview = new VideoPreviewWidget();
	//m_preview->setFile(mpPlayer->file());
	//m_preview->setTimestamp(value);
	//m_preview->setWindowFlags(m_preview->windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	//emit updateProgress(nth,value);
}

void AudioWall::setVolume()
{
	QAudioOutput *temp = tasks[nth]->audio;
	qreal v = qreal(tasks[nth]->volumeSlider->value())*kVolumeInterval;
	if (temp)
	{
		if (qAbs(int(temp->volume() / kVolumeInterval) - tasks[nth]->volumeSlider->value()) >= int(0.1 / kVolumeInterval))
		{
			temp->setVolume(v);
		}
	}
	tasks[nth]->volumeSlider->setToolTip(QString::number(v));
}