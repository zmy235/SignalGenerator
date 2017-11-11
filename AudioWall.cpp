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
	setFixedSize(650, 500);
	layout = new QVBoxLayout(this);
	layout->setAlignment(Qt::AlignTop);
	layout->setMargin(5);
	font = QFont("Cambria", 12, 28, false);
	font.setBold(true);
	font_pe.setColor(QPalette::WindowText, Qt::white);
	pe.setColor(QPalette::ButtonText, Qt::white);
	nth = 0;
	addButton = new QPushButton(QIcon("./Resources/add.png"), tr(""), this);
	addButton->setStatusTip(tr("���"));
	addButton->setFixedSize(QSize(45, 45));
	addButton->setIconSize(QSize(45, 45));
	layout->addWidget(addButton);
	connect(addButton, SIGNAL(clicked()), this, SLOT(addAudioView()));
}

AudioWall::~AudioWall()
{

}

void AudioWall::addAudioView()
{
	AddView = new BaseWidget();
	QVBoxLayout *vLayout = new QVBoxLayout(AddView);
	vLayout->setAlignment(Qt::AlignHCenter);
	vLayout->setSpacing(45);

	//���ò�����
	sampleRateComboBox = new QComboBox(AddView);
	sampleRateComboBox->setFixedSize(QSize(150, 45));
	sampleRateComboBox->insertItems(0, QStringList() << "48000" << "44100" << "32000" << "24000" << "22050" << "16000" << "12050" << "8000");
	sampleRateComboBox->setStatusTip(tr("SAMPLERATE"));
	sampleRateComboBox->setFont(font);
	sampleRateComboBox->setPalette(pe);
	sampleRateComboBox->setStyleSheet("QComboBox{ background - color: #454545; }");
	vLayout->addWidget(sampleRateComboBox);

	//����ͨ��
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

	//�����ļ�
	QPushButton *VedioFileButton;
	VedioFileButton = new QPushButton(QIcon("./Resources/file.png"), tr(""), AddView);
	VedioFileButton->setStatusTip(tr("Choose a File"));
	VedioFileButton->setFixedSize(QSize(45, 45));
	VedioFileButton->setIconSize(QSize(45, 45));
	vLayout->addWidget(VedioFileButton);

	QPushButton *BackButton;//ȡ��
	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr(""), AddView);
	BackButton->setStatusTip(tr("����"));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));
	QPushButton *addOKButton;//���
	addOKButton = new QPushButton(QIcon("./Resources/ok.png"), tr(""), AddView);
	addOKButton->setStatusTip(tr("����"));
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
	sampleRate = sampleRateComboBox->itemText(index);
}

void AudioWall::deviceChanged(const int &index)
{
	taskName = QString::fromLocal8Bit("ͨ��") + QString::number(index + 1);
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
	row = new QWidget(this);
	row->setFixedHeight(45);
	row->setWindowFlags(Qt::FramelessWindowHint);
	layout->insertWidget(0, row);

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

	TimeSlider = new QSlider(row);
	TimeSlider->setOrientation(Qt::Horizontal);
	TimeSlider->setTickPosition(QSlider::TicksAbove);//���ÿ̶�
	TimeSlider->setTickInterval(5);
	TimeSlider->setMinimum(0);
	TimeSlider->setMaximum(100);

	VolumeSlider = new QSlider(row);
	VolumeSlider->setTracking(true);//ʵʱ�ı�
	VolumeSlider->setOrientation(Qt::Vertical);
	VolumeSlider->setMinimum(0);
	VolumeSlider->setMaximum(100);

	QPushButton* state = new QPushButton(QIcon("./Resources/start.png"), tr(""), row);
	state->setFixedSize(QSize(45, 45));
	state->setIconSize(QSize(45, 45));
	QPushButton* remove = new QPushButton(QIcon("./Resources/remove.png"), tr(""), row);
	remove->setFixedSize(QSize(45, 45));
	remove->setIconSize(QSize(45, 45));

	hLayout->addWidget(taskNameLable);
	hLayout->addWidget(fileNameLable);
	hLayout->addWidget(TimeSlider);
	hLayout->addWidget(VolumeSlider);
	hLayout->addWidget(state);
	hLayout->addWidget(remove);

	hLayout->setStretch(0, 2);
	hLayout->setStretch(1, 4);
	hLayout->setStretch(2, 4);
	hLayout->setStretch(3, 1);
	hLayout->setStretch(4, 1);
	hLayout->setStretch(5, 1);

	m_format.setSampleRate(sampleRate.toInt());//���ò�����  
	m_format.setChannelCount(1);//����ͨ����  
	m_format.setSampleSize(16);//���ò�����С��һ��Ϊ8λ��16λ  
	m_format.setCodec("audio/pcm");//���ñ��뷽ʽ  
	m_format.setByteOrder(QAudioFormat::LittleEndian);//�����ֽ���  
	m_format.setSampleType(QAudioFormat::UnSignedInt);//����������������

	inputFile = new WAVFile;
	inputFile->open(file_path, QIODevice::ReadOnly);
	m_format = m_device.preferredFormat();
	m_audioOutput = new QAudioOutput(m_device, m_format);
	m_audioOutput->setNotifyInterval(1000);
	if (inputFile->isReadable())
	{
		AudioTask* task = new AudioTask(tr("Audio"), taskName, taskInfo, m_audioOutput, sampleRate);
		task->file = inputFile;
		task->size = inputFile->size();
		inputFile->seek(0);
		m_audioOutput->start(inputFile);
		task->timeSlider = TimeSlider;
		task->taskTime = QDateTime::currentDateTime();
		task->isFinished = false;
		task->isPlaying = true;
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
	connect(state, SIGNAL(clicked()), this, SLOT(setState()));
	connect(remove, SIGNAL(clicked()), this, SLOT(Remove()));
}

void AudioWall::updateAudioState(QAudio::State)
{
	emit updateAudioState(nth);
}

void AudioWall::setState()
{
	QPushButton *test = qobject_cast<QPushButton *>(sender());
	QWidget *w = test->parentWidget();
	nth = w->y() / 50;
	if (!tasks[nth]->audio->signalsBlocked())
	{
		tasks[nth]->audio->blockSignals(true);
		tasks[nth]->isPlaying = false;
		test->setIcon(QIcon("./Resources/start.png"));
	}
	else
	{
		tasks[nth]->audio->start();
		tasks[nth]->isPlaying = true;
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
	tasks.removeAt(nth);
}

void AudioWall::setPlayerPosition(int value)
{
	QSlider *test = qobject_cast<QSlider *>(sender());
	QWidget *w = test->parentWidget();
	nth = w->y() / 50;
	qDebug() << nth << ": " << value;
	qint64 allData = tasks[nth]->file->size();
	tasks[nth]->file->seek(allData / 100 * value);
	test->setToolTip(QString::number(value)); 
}

void AudioWall::setSliderPosition()
{
	QAudioOutput *test = qobject_cast<QAudioOutput *>(sender());
	qint64 bytesInBuffer = test->bufferSize() - test->bytesFree();
	qint64 usInBuffer = (qint64)(1000000) * bytesInBuffer / (m_format.channelCount() * m_format.sampleSize() / 8) / m_format.bytesPerFrame();
	qint64 usPlayed = test->processedUSecs() - usInBuffer;
	inputFile->seek(usPlayed);
	qint64 allData = tasks[nth]->size;
	qDebug() <<"size "<<tasks[nth]->size<< " played " << test->processedUSecs() << "  BUfferSize: " << test->bufferSize();
	tasks[nth]->timeSlider->setValue(usPlayed*100/allData);
}

void AudioWall::setVolume(int value)
{
	QSlider *test = qobject_cast<QSlider *>(sender());
	QWidget *w = test->parentWidget();
	nth = w->y() / 50;
	qDebug() << nth << "\n" << value;
	QAudioOutput *temp = tasks[nth]->audio;
	if (temp) temp->setVolume(value*1.0/100.0);
	test->setToolTip(QString::number(value));
}