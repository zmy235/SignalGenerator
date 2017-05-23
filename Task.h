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
	Task();
	~Task(); 
public:
	QString type;
	QString name;
	QString filePath;
	QDateTime time;
	int size;
	int progress;
	bool playing;
	bool finished;
	//QFileInfo info;//ÂëÂÊ¡¢·Ö±æÂÊ
};

#endif