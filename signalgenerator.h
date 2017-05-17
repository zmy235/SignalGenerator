#ifndef MainWindow_H
#define MainWindow_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

//所用到的类的前置声明
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
//PlayerWindow

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow ui;

private:
	void createActions();
	void createToolBars();

	AddView *addView;
	SetView *setView;
	AboutView *aboutView;
	HistoryView *historyView;
	FindView *findView;
	VideoWall *wall;
	//PlayerWindow* player;

	QFont font;
	QPalette font_pe;
	QPalette palette; 

	QToolBar *MainToolBar;
	QAction *add;//添加
	QAction *history;//历史记录
	QAction *lookfor;//查找
	QAction *setting;//设置
	QAction *about;//关于
	QAction *shutdown;//关闭

	QList<Task*>* taskList;//全局
	QList<Task*>::iterator TaskListHead;
	int TaskListSize;
	int ListNum;
	QList<QWidget*>* WigetList;
	QWidget *Base;
	QLabel *taskName;
	QProgressBar *progressBar;
	QPushButton *start;
	QPushButton *stop;
	QPushButton *remove;
	QPushButton *info;

	QRect m_areaMovable;//可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
	QPoint m_ptPress;//鼠标按下的初始位置
	bool m_bPressed;//鼠标按下标志（不分左右键）
	void setAreaMovable(const QRect rt);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

	private slots:

	void updateVH(bool);
	void updateOpacity(int);
	void updateList(Task*);

	void Add();
	void History();
	void Setting();
	void About();
	void Find();

	void Start();
	void Stop();
	void Remove();
	void Info();

};

#endif // MainWindow_H
