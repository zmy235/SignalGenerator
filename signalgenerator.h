#ifndef MainWindow_H
#define MainWindow_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

//���õ������ǰ������
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
class    Task;
class    AddView;
class    SetView;
class    AboutView;
class    HistoryView;
class    FindView;
class    VideoWall;
class    QVBoxLayout;
class    QScrollArea;
class    QScrollBar;

//PlayerWindow
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	void close();

private:
	Ui::MainWindow ui;

private:
	void createActions();
	void createToolBars();

	SetView *setView;
	AboutView *aboutView;
	HistoryView *historyView;
	FindView *findView;
	VideoWall *wall;

	QFont font;
	QPalette font_pe;
	QPalette palette; 

	QToolBar *MainToolBar;
	QAction *audio;//��Ƶ
	QAction *video;//��Ƶ
	QAction *history;//��ʷ��¼
	QAction *lookfor;//����
	QAction *setting;//����
	QAction *about;//����
	QAction *shutdown;//�ر�

	QList<Task*>* taskList;//ȫ��
	QList<Task*>::iterator TaskListHead;
	QList<QWidget*>* WigetList; 
	QVBoxLayout *vLayout;
	QScrollArea *scrollArea;

	int TaskListSize;
	int ListNum;

	BaseWidget *Base,*audioView,*videoView;
	QLabel *taskName;
	QProgressBar *progressBar;
	QPushButton *start;
	QPushButton *stop;
	QPushButton *remove;
	QPushButton *info;

	QRect m_areaMovable;//���ƶ����ڵ��������ֻ���ڸ������²����ƶ�����
	QPoint m_ptPress;//��갴�µĳ�ʼλ��
	bool m_bPressed;//��갴�±�־���������Ҽ���
	void setAreaMovable(const QRect rt);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

	private slots:

	void updateVH(bool);
	void updateOpacity(int);
	void updateList(Task*);

	void AudioView();
	void VideoView();
	void History();
	void Setting();
	void About();
	void Find();

	void Start_Stop();
	void Remove();
	void Info();

};

#endif // MainWindow_H
