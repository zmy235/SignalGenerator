#include "AudioWall.h"
#include "BaseWidget.h"
#include "Task.h"

#include <QFile>  
#include <QAudioFormat>  
#include <QAudioOutput>
#include <QComboBox>
#include <QDesktopWidget>
#include <QDebug>
#include <QEvent>
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
#include <QPoint>
#include <QRect>
#include <QTextEdit>
#include <QtCore/QUrl>
#include <QUrl>
#include <QWidget>

const qreal kVolumeInterval = 0.04;

AudioWall::AudioWall(QWidget *parent) : BaseWidget(parent)
{
	setWindowTitle(tr("Audio Wall"));

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

	QVBoxLayout *vLayout = new QVBoxLayout(AddView);
	vLayout->setAlignment(Qt::AlignHCenter);
	vLayout->setSpacing(45);

	//设置采样率
	QComboBox *VedioComboBox;
	VedioComboBox = new QComboBox(AddView);
	VedioComboBox->setFixedSize(QSize(150, 45));
	VedioComboBox->insertItems(0, QStringList() << "48000" << "44100" << "32000" << "24000" << "22050" << "16000" << "12050" << "8000");
	VedioComboBox->setStatusTip(tr("返回"));
	VedioComboBox->setFont(font);
	VedioComboBox->setPalette(pe);
	VedioComboBox->setStyleSheet("QComboBox{ background - color: #454545; }");
	vLayout->addWidget(VedioComboBox);

	//VedioFileButton
	QPushButton *VedioFileButton;
	VedioFileButton = new QPushButton(QIcon("./Resources/file.png"), tr(""), AddView);
	VedioFileButton->setStatusTip(tr("Choose a File"));
	VedioFileButton->setFixedSize(QSize(45, 45));
	VedioFileButton->setIconSize(QSize(45, 45));
	vLayout->addWidget(VedioFileButton);

	QHBoxLayout *hLayout = new QHBoxLayout();
	QPushButton *BackButton;//取消
	BackButton = new QPushButton(QIcon("./Resources/cancel.png"), tr(""), AddView);
	BackButton->setStatusTip(tr("返回"));
	BackButton->setFixedSize(QSize(45, 45));
	BackButton->setIconSize(QSize(45, 45));
	hLayout->addWidget(BackButton);
	QPushButton *addOKButton;//添加
	addOKButton = new QPushButton(QIcon("./Resources/ok.png"), tr(""), AddView);
	addOKButton->setStatusTip(tr("返回"));
	addOKButton->setFixedSize(QSize(45, 45));
	addOKButton->setIconSize(QSize(45, 45));
	hLayout->addWidget(addOKButton);
	vLayout->addLayout(hLayout);

	connect(VedioComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectAudioType(int)));
	connect(VedioFileButton, SIGNAL(clicked()), this, SLOT(openLocalFile()));
	connect(BackButton, SIGNAL(clicked()), AddView, SLOT(close()));
	connect(addOKButton, SIGNAL(clicked()), this, SLOT(AudioOK()));

	AddView->setLayout(vLayout);
	AddView->show();
}

void AudioWall::selectAudioType(const int &text)
{
	int SampleRate[] = { 48000, 44100, 32000, 24000, 22050, 16000, 12050, 8000 };
	SampleRates[nth] = SampleRate[text];
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
	QString file_path = QFileDialog::getOpenFileName(0, tr("Open an Audio File"), "./", "Vedio(*.wav, *.flac, *.aiff, *.ogg, *.mp3)");
	QFileInfo file_info = QFileInfo(file_path);
	int file_size = file_info.size();
	QString file_name = file_info.fileName();
	QString ab_path = file_info.absolutePath();
	tasks[nth].filePath = file_path;
	tasks[nth].name = file_name;
	tasks[nth].size = file_size;
}

void AudioWall::AudioOK()
{
	WAVFile *inputFile = new WAVFile();
	inputFile->open(tasks[nth].filePath, QIODevice::ReadOnly);
	if (!inputFile->isReadable())
	{
		QMessageBox::warning(0, tr("warnning"), tr("The file is unreadable!\n"));
		return;
	}

	//QAudioFormat audioFormat;//设置采样格式 
	//audioFormat.setSampleRate(SampleRates[nth]);//设置采样率  
	//audioFormat.setChannelCount(1);//设置通道数  
	//audioFormat.setSampleSize(16);//设置采样大小，一般为8位或16位  
	//audioFormat.setCodec("audio/pcm");//设置编码方式  
	//audioFormat.setByteOrder(QAudioFormat::LittleEndian);//设置字节序  
	//audioFormat.setSampleType(QAudioFormat::UnSignedInt);//设置样本数据类型

	QAudioOutput *audio = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), inputFile->format());

	QWidget *row = new QWidget(this);
	row->setFixedHeight(45);
	row->setWindowFlags(Qt::FramelessWindowHint);
	row->setGeometry(5, nth * 50, 800, 45);
	QHBoxLayout *hLayout = new QHBoxLayout(row);
	hLayout->setSpacing(5);

	QLabel* taskName = new QLabel(row);
	taskName->setText(tasks[nth].name);
	taskName->setFixedHeight(45);
	taskName->setFont(font);
	taskName->setPalette(font_pe);
	hLayout->addWidget(taskName, 20);

	Slider* mpTimeSlider = new Slider(row);
	mpTimeSlider->setDisabled(true);
	mpTimeSlider->setTracking(true);
	mpTimeSlider->setOrientation(Qt::Horizontal);
	mpTimeSlider->setMinimum(0);
	hLayout->addWidget(mpTimeSlider, 30);
	mpTimeSliders.append(mpTimeSlider);

	Slider* mpVolumeSlider = new Slider(row);
	mpVolumeSlider->setMinimum(0);
	//mpVolumeSlider->setInvertedAppearance(false);
	hLayout->addWidget(mpVolumeSlider, 10);
	mpVolumeSliders.append(mpVolumeSlider);

	QPushButton* start = new QPushButton(QIcon("./Resources/start.png"), tr(""), row);
	start->setObjectName(QString::fromUtf8("start"));
	start->setFixedSize(QSize(45, 45));
	start->setIconSize(QSize(45, 45));
	hLayout->addWidget(start, 10);

	QPushButton* stop = new QPushButton(QIcon("./Resources/stop.png"), tr(""), row);
	stop->setObjectName(QString::fromUtf8("stop"));
	stop->setFixedSize(QSize(45, 45));
	stop->setIconSize(QSize(45, 45));
	hLayout->addWidget(stop, 10);

	QPushButton* remove = new QPushButton(QIcon("./Resources/remove.png"), tr(""), row);
	remove->setObjectName(QString::number(nth));
	remove->setFixedSize(QSize(45, 45));
	remove->setIconSize(QSize(45, 45));
	hLayout->addWidget(remove, 10);

	connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audio0(QAudio::State)));
	connect(mpTimeSlider, SIGNAL(sliderPressed()), SLOT(seek()));
	connect(mpTimeSlider, SIGNAL(sliderReleased()), SLOT(seek()));
	connect(mpTimeSlider, SIGNAL(onLeave()), SLOT(onTimeSliderLeave()));
	connect(mpTimeSlider, SIGNAL(onHover(int, int)), SLOT(onTimeSliderHover(int, int)));

	connect(mpVolumeSlider, SIGNAL(sliderPressed()), SLOT(setVolume()));
	connect(mpVolumeSlider, SIGNAL(valueChanged(int)), SLOT(setVolume()));

	connect(start, SIGNAL(clicked()), this, SLOT(Start()));
	connect(stop, SIGNAL(clicked()), this, SLOT(Stop()));
	connect(remove, SIGNAL(clicked()), this, SLOT(Remove()));

	addButtons[nth]->hide();
	row->show();

	//更新
	tasks[nth].type = "Audio";
	tasks[nth].time = QDateTime::currentDateTime();
	tasks[nth].progress = 0;
	tasks[nth].playing = true;
	tasks[nth].audio = audio;
	players.append(audio);

	emit updateList(&tasks[nth]);
	audio->start();
	AddView->close();
}

void AudioWall::Start()
{
	QPushButton *test = qobject_cast<QPushButton *>(sender());
	nth = test->y() / 50;
	players[nth]->start();
}

void AudioWall::Stop()
{
	QPushButton *test = qobject_cast<QPushButton *>(sender());
	nth = test->y() / 50;
	players[nth]->stop();
}

void AudioWall::Remove()
{
	QPushButton *test = qobject_cast<QPushButton *>(sender());
	QWidget *w = test->parentWidget();
	nth = w->y() / 50;
	w->close();
	addButtons[nth]->show();
}

void AudioWall::seek()
{
	//players[nth]->setSeekType(AccurateSeek);
	//players[nth]->((qint64)mpTimeSlider->value());
}

void AudioWall::onTimeSliderLeave()
{
	//if (m_preview && m_preview->isVisible())
	//	m_preview->hide();
}

void AudioWall::onTimeSliderHover(int pos, int value)
{
	//QPoint gpos = mapToGlobal(mpTimeSlider->pos() + QPoint(pos, 0));
	//QToolTip::showText(gpos, QTime(0, 0, 0).addMSecs(value).toString(QString::fromLatin1("HH:mm:ss")));
	//if (!m_preview)
	//	m_preview = new VideoPreviewWidget();
	//m_preview->setFile(mpPlayer->file());
	//m_preview->setTimestamp(value);
	//m_preview->setWindowFlags(m_preview->windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

void AudioWall::setVolume()
{
	QAudioOutput *ao = players[nth];
	qreal v = qreal(mpVolumeSliders[nth]->value())*kVolumeInterval;
	if (ao)
	{
		if (qAbs(int(ao->volume() / kVolumeInterval) - mpVolumeSliders[nth]->value()) >= int(0.1 / kVolumeInterval))
		{
			ao->setVolume(v);
		}
	}
	mpVolumeSliders[nth]->setToolTip(QString::number(v));
}