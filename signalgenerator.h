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
	QAction *audio;//��Ƶ
	QAction *video;//��Ƶ
	QAction *history;//��ʷ��¼
	QAction *setting;//����
	QAction *about;//����
	QAction *shutdown;//�ر�

	QVector<Task*> tasks;//ȫ�������б�
	QList<AudioTask*> audioTasks;//ȫ����Ƶ�����б�
	QList<VideoTask*> videoTasks;//ȫ����Ƶ�����б�
	QList<TaskRow*> rows;//�пؼ�

	QRect m_areaMovable;//���ƶ����ڵ��������ֻ���ڸ������²����ƶ�����
	QPoint m_ptPress;//��갴�µĳ�ʼλ��
	bool m_bPressed;//��갴�±�־(�������Ҽ�)

	void createActions();
	void createToolBars();//�϶�����
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
