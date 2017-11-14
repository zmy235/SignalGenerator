#ifndef AUDIOTASK_H
#define AUDIOTASK_H

#include "Task.h"
#include "WAVFile.h"
#include <QAudioOutput>
#include <QSlider>

class AudioTask : public Task
{
public:
	AudioTask(QString a, QString b, QFileInfo c, QAudioOutput* d, QString e) : Task(a, b, c), audio(d), sampleRate(e)
	{
		taskType = a;
		taskName = b;
		taskInfo = c;
		taskProgress = 0;
	}
	~AudioTask()
	{
		delete audio;
		delete file;
		delete timeSlider;
	}
	QAudio::State getState(){ return audio ? audio->state() : QAudio::StoppedState; }
	void start(){ if (getState() != QAudio::ActiveState) audio->start(); }
	void pause() { if (getState() == QAudio::ActiveState) audio->blockSignals(true); }
	void stop() { if (getState() != QAudio::StoppedState) audio->stop(); }
	void setState()
	{
		if (getState() == QAudio::ActiveState)audio->stop();
		else audio->start();
	}

public:
	QString sampleRate;
	qint64 size;
	WAVFile *file;
	QAudioOutput* audio;
	QSlider *timeSlider;
};

#endif