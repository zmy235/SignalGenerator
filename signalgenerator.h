#ifndef MainWindow_H
#define MainWindow_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "Task.h"
#include "AudioTask.h"
#include "VideoTask.h"

class    Action;
class    QToolBar;
class    QWidget;
class    BaseWidget;
class    QRect;
class    QPoint;
class    QMouseEvent;
class    QProgressBar;
class    QListView;
class    QPushButton;
class    QLabel;
class    QDateTime;
class    TaskRow;
class    AddView;
class    SetView;
class    AboutView;
class    HistoryView;
class    AudioWall;
class    VideoWall;
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
	BaseWidget *videoView;
	VideoWall *wall;

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

	QVector<Task*> taskList;//ȫ����Ƶ�����б�
	QList<AudioTask*> AudioTaskList;//ȫ����Ƶ�����б�
	QList<VideoTask*> VideoTaskList;//ȫ����Ƶ�����б�
	QList<TaskRow*> rows;//�пؼ�

	int ListNum;
	int nth;//��ǰ����λ��
	int audioNth[10];//��Ƶ�����Ӧλ��
	int videoNth[9];//��Ƶ�����Ӧλ��

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
	void updateVideoList(VideoTask*);
	void updateAudioState(int);
	void updateVideoState(int);
	void updateVH(bool);
	void updateOpacity(int);
	void updateFramless(bool);

	void AudioView();
	void VideoView();
	void History();
	void Setting();
	void About();
	void Info();
};

#endif // MainWindow_H
