#ifndef VIDEOTASK_H
#define VIDEOTASK_H

#include "Task.h"
#include "Slider.h"
#include <QtAV/AVPlayer.h>
#include <QSlider>
#include <QFileInfo>
using namespace QtAV;

class VideoTask : public Task
{
public:
	VideoTask(QString a, QString b, QFileInfo c, AVPlayer* d) :
		Task(a, b, c),
		video(d)
	{
		taskType = a;
		taskName = b;
		taskInfo = c;
		taskProgress = 0;
		isPlaying = false;
		isFinished = false;
	}

	~VideoTask()
	{
		delete video;
		delete timeSlider;
		delete volumeSlider;
	}

	void start()
	{
		if (!video->isPlaying()) 
			video->play();
		isPlaying = true;
	}

	void pause()
	{
		video->pause();
		isPlaying = false;
	}

	void cancel()
	{
		video->pause();
		isPlaying = false;
	}

public:
	AVPlayer* video;
	QSlider* timeSlider;
	QSlider* volumeSlider;
};

#endif