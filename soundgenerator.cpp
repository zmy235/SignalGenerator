#include "soundgenerator.h"
#include "WAVFile.h"

SoundGenerator::SoundGenerator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	m_deviceBox = new QComboBox(this);
	foreach(const QAudioDeviceInfo &deviceInfo, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
		m_deviceBox->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
	connect(m_deviceBox, SIGNAL(activated(int)), SLOT(deviceChanged(int)));
}

SoundGenerator::~SoundGenerator()
{

}

void SoundGenerator::deviceChanged(int index)
{
	m_device = m_deviceBox->itemData(index).value<QAudioDeviceInfo>();
	createAudioOutput();
}



void SoundGenerator::createAudioOutput()
{
	QString fileName = "./xianliang.wav";
	WAVFile *inputFile = new WAVFile;
	inputFile->open(fileName, QIODevice::ReadOnly);

	QAudioOutput *audio = new QAudioOutput(m_device, inputFile->format());
	connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audio0(QAudio::State)));

	if (inputFile->isReadable())
	{
		audio->start(inputFile);
	}
}
