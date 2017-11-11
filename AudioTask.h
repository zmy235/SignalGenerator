#ifndef AUDIOTASK_H
#define AUDIOTASK_H

#include "Task.h"
#include <QAudioOutput>
#include <QSlider>

class AudioTask : public Task
{

public:
	AudioTask(QString a, QString b, QFileInfo c, QAudioOutput* d, QString e) :
		Task(a, b, c),
		audio(d), 
		sampleRate(e)
	{
		taskType = a;
		taskName = b;
		taskInfo = c;
		taskProgress = 0;
		isPlaying = false;
		isFinished = false;
	}

	~AudioTask()
	{
		delete audio;
		delete timeSlider;
		delete volumeSlider;
	}

	void start()
	{
		if (audio->signalsBlocked())
			audio->start();
		isPlaying = true;
	}

	void pause()
	{
		audio->stop();
		isPlaying = false;
	}

	void cancel()
	{
		audio->stop();
		isPlaying = false;
	}

public:
	QString sampleRate;
	QAudioOutput* audio;
	QSlider* timeSlider;
	QSlider* volumeSlider;
};

#endif