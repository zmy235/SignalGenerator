#ifndef VIDEOWALL_H
#define VIDEOWALL_H

#include "BaseWidget.h"
#include "VideoTask.h"
#include <QComboBox>
#include <QtAVWidgets> 
#include <QtAV/AVPlayer.h>
using namespace QtAV;

QT_BEGIN_NAMESPACE
class QMenu;
class QLabel;
QT_END_NAMESPACE

class VideoWall : public QWidget
{
	Q_OBJECT
public:
	explicit VideoWall(QWidget *parent = 0);
	~VideoWall();
	void setRows(int n);
	void setCols(int n);
	int getRows() const;
	int getCols() const;

protected:
	virtual bool eventFilter(QObject *, QEvent *);

private:
	int r, c;
	QFont font;
	QPalette pe;
	BaseWidget *AddView;
	QLabel *FrameNumView;
	QComboBox *VedioComboBox;
	QMenu *menu;

	int nth;
	QMap<int, VideoTask*> taskMap;
	QList<VideoTask*> tasks;
	QList<AVPlayer*> players;
	QList<QWidget*> rendererWidgets;
	QList<QSlider*> timeSliders;
	QList<QSlider*> volumeSliders;
	QString taskName;
	QString taskPath;
	QFileInfo taskInfo;

signals:
	void updateVideoList(VideoTask*);
	void updateVideoState(int);
	void updateVideoProgress(int,int);

	public slots:
	void playAll();
	void stopAll(); 
	void help();
	void addVideoView();
	void setVideoType(const int &);
	void openLocalFile();
	void addFramNum(int); 
	void framCount(int);
	void VedioOK();

	void setSliderPosition(qint64);
	void setPlayerPosition(int);
	void setVolume(int);
};

#endif
