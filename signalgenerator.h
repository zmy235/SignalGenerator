#ifndef MainWindow_H
#define MainWindow_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "BaseWidget.h"
#include "Task.h"
#include "AudioTask.h"
#include "VideoTask.h"
#include "TaskRow.h"
#include "HistoryView.h"
#include "SetView.h"
#include "AboutView.h"
#include "AudioWall.h"
#include "VideoWall.h"

class    QAction;
class    QToolBar;
class    QWidget;
class    QRect;
class    QPoint;
class    QMouseEvent;
class    QProgressBar;
class    QPushButton;
class    QLabel;
class    QDateTime;
class    QVBoxLayout;
class    QScrollArea;
class    QScrollBar;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow ui;
	SetView *setView;
	AboutView *aboutView;
	HistoryView *historyView;
	AudioWall *audioView;
	VideoWall *videoView;

private:

	QFont font;
	QPalette font_pe;
	QPalette palette;

	QWidget *Base;
	QScrollArea *scrollArea;
	QVBoxLayout *vLayout;

	QToolBar *MainToolBar;
	QAction *audio;//音频
	QAction *video;//视频
	QAction *history;//历史记录
	QAction *setting;//设置
	QAction *about;//关于
	QAction *shutdown;//关闭

	QVector<Task*> tasks;//全局任务列表
	QList<AudioTask*> audioTasks;//全局音频任务列表
	QList<VideoTask*> videoTasks;//全局视频任务列表
	QList<TaskRow*> rows;//行控件

	QRect m_areaMovable;//可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
	QPoint m_ptPress;//鼠标按下的初始位置
	bool m_bPressed;//鼠标按下标志(不分左右键)

	void createActions();
	void createToolBars();//拖动控制
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void setAreaMovable(const QRect rt);
	void deleteRow(Task*);

	private slots:

	void updateAudioList(AudioTask*);
	void updateAudioState(int);
	void updateAudioProgress(int, int);

	void updateVideoList(VideoTask*);
	void updateVideoState(int);
	void updateVideoProgress(int, int);
	
	void updateVH(bool);
	void updateOpacity(int);
	void updateFramless(bool);

	void AudioView();
	void VideoView();
	void Setting();
	void History();
	void About();
	void Info();
};

#endif // MainWindow_H
