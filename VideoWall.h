#ifndef VIDEOWALL_H
#define VIDEOWALL_H

#include "BaseWidget.h"
#include "VideoTask.h"
#include <QtCore/QList>
#include <QtAV/AVPlayer.h>
using namespace QtAV;

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

class VideoWall : public QObject
{
    Q_OBJECT
public:
    explicit VideoWall(QObject *parent = 0);
    ~VideoWall();
    void setRows(int n);
    void setCols(int n);
    int rows() const;
	int cols() const;
	void setView(BaseWidget *base);
	void show();
    void playAll();

private:
    int r, c;
	int nth;
	BaseWidget *view; 
	BaseWidget *AddView;
    QMenu *menu;
    QString vid;

	QList<VideoTask*> tasks;
	QString taskName;
	QString taskPath;
	QDateTime taskTime;
	QFileInfo taskInfo;
	AVPlayer* player;
	QSlider* timeSlider;
	QSlider* volumeSlider;

protected:
    virtual bool eventFilter(QObject *, QEvent *);

signals:
	void updateVideoList(VideoTask*);
	void updateVideoState(int n);

public slots:
	void stop();
	void close();
	void help();
	void addVideoView();
	void selectVideoType(const int &text);
	void openLocalFile();
	void VedioOK();

};

#endif
