#pragma once

#ifndef TASK_H
#define TASK_H

#include<QString>
#include<QList>
#include<QDateTime>
#include<QFileInfo>
#include <QAudioOutput>
#include <QtAV/AVPlayer.h>

class Task
{
public:
	Task();
	~Task();
	void start();
	void pause();
	QtAV::AVPlayer* video;
	QAudioOutput* audio;
	QString type;
	QString name;
	QString filePath;
	QDateTime time;
	QString info;
	int size;
	int progress;
	bool playing;
	bool finished;
};

#endif