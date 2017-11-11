#pragma once

#ifndef TASK_H
#define TASK_H

#include<QString>
#include<QList>
#include<QDateTime>
#include<QFileInfo>

class Task
{
public:
	Task(QString a, QString b, QFileInfo c) :
		taskType(a),
		taskName(b),
		taskInfo(c){}
	~Task(){}
	virtual void start() = 0;
	virtual void pause() = 0;
	virtual void cancel() = 0;

public:
	QString taskType;
	QString taskName;
	QFileInfo taskInfo;
	QDateTime taskTime;
	int taskProgress;
	bool isPlaying;
	bool isFinished;
};

#endif