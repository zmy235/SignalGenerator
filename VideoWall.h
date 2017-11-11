#ifndef VIDEOWALL_H
#define VIDEOWALL_H

#include "BaseWidget.h"
#include "Slider.h"
#include "VideoTask.h"
#include <QComboBox>
#include <QtAVWidgets> 
#include <QtAV/AVPlayer.h>
using namespace QtAV;

QT_BEGIN_NAMESPACE
class QMenu;
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
	QComboBox *VedioComboBox;
	QMenu *menu;

	int nth;
	QMap<int, VideoTask*> taskMap;
	QList<VideoTask*> tasks;
	QList<AVPlayer*> players;
	VideoPreviewWidget *m_preview;
	QList<QWidget*> rendererWidgets;
	QList<QSlider*> timeSliders;
	QList<QSlider*> volumeSliders;
	QString taskName;
	QString taskPath;
	QFileInfo taskInfo;

signals:
	void updateVideoList(VideoTask*);
	void updateVideoState(int n);

	public slots:
	void playAll();
	void stopAll();
	void help();
	void addVideoView();
	void setVideoType(const int &text);
	void openLocalFile();
	void VedioOK();
	void seek();
	void seek(int);
	void onTimeSliderLeave();
	void onTimeSliderHover(int pos, int value);
	void onPositionChange(qint64 pos);
};

#endif
