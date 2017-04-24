#ifndef MainWindow_H
#define MainWindow_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

//所用到的类的前置声明
class    Action;
class    QToolBar;
class    QWidget;
class    QRect;
class    QPoint;
class    QMouseEvent;
class    QProgressBar;
class    QListView; 
class    QDoubleSpinBox;


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
	QListView *listView;

private:
	Ui::MainWindow ui;

private:
	void createActions();
	void createToolBars();
	void createList();

	QWidget *centralwidget;
	QTabWidget *tabWidget;
	QWidget *tab;
	QWidget *tab_2;
	QProgressBar *progressBar;
	QToolBar *MainToolBar;
	QToolBar *AddToolBar;
	QDoubleSpinBox *doubleSpinBox;


	QAction *list;//列表
	QAction *add;//添加
	QAction *history;//历史记录
	QAction *lookfor;//查找
	QAction *setting;//设置
	QAction *about;//关于
	QAction *shutdown;//关闭

	QAction *back;//返回
	QAction *audio;//音频
	QAction *vedio;//视频
	QAction *start;//开始
	QAction *stop;//停止
	QAction *remove;//移除

	QRect m_areaMovable;//可移动窗口的区域，鼠标只有在该区域按下才能移动窗口
	bool m_bPressed;//鼠标按下标志（不分左右键）
	QPoint m_ptPress;//鼠标按下的初始位置

	void setAreaMovable(const QRect rt);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

	private slots:
	void List();
	void Add();
	void History();
	void Lookfor();
	void Setting();
	void About();

	void Back();
	void AddAudioWin();
	void AddVedioWin();
	void Start();
	void Stop();
	void Remove();

};

#endif // MainWindow_H
