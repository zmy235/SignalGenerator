#ifndef AudioWALL_H
#define AudioWALL_H

#include <QtCore/QList>
#include <QFile>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QComboBox>
#include <QToolBox>

#include "BaseWidget.h"
#include "Slider.h"
#include "AudioTask.h"
#include "WAVFile.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QPushButton;
class QVBoxLayout;
QT_END_NAMESPACE

class AudioWall : public BaseWidget
{
	Q_OBJECT

public:
	explicit AudioWall(QWidget *parent = 0);
	~AudioWall();

public:
	int nth;
	QVBoxLayout *layout;
	QWidget *row;
	QPushButton* addButton;

	BaseWidget *AddView;
	QList<AudioTask*> tasks;
	QString taskName;
	QString file_path;
	QFileInfo taskInfo;
	QComboBox *sampleRateComboBox;
	QString sampleRate;
	WAVFile *inputFile;
	qint64 fileSize;
	QComboBox *m_deviceBox;
	QAudioDeviceInfo m_device;
	QIODevice *m_output;
	QAudioOutput *m_audioOutput;
	QAudioFormat m_format;
	QSlider* TimeSlider;
	QSlider* VolumeSlider;

	QFont font;
	QPalette pe;
	QPalette font_pe;

signals:
	void updateAudioList(AudioTask*);
	void updateAudioState(int);
	void updateAudioProgress(int);

public slots:
	void addAudioView();
	void setSampleRate(const int &);
	void deviceChanged(const int &);
	void openLocalFile();
	void AudioOK();
	void Remove(); 
	void updateAudioState(QAudio::State);
	void setState();
	void setSliderPosition();
	void setPlayerPosition(int);
	void setVolume(int);

};

#endif
