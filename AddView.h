//头文件：AddView.h
#ifndef AddView_H
#define AddView_H

#include "BaseWidget.h"

//所用到的类的前置声明
class    QWidget;
class    QFont;
class    QPalette;
class    QRect;
class    QPoint;
class    QHBoxLayout;
class    QPushButton;
class    QComboBox;
class    QDoubleSpinBox;
class    Task;

//继承自QWidget的自定义窗口，单击窗口并移动鼠标可拖动窗口
class AddView : public BaseWidget
{
	Q_OBJECT

public:
	AddView(QWidget *parent = 0);
	~AddView();

private:

	Task *task;
	QFont font;
	QPalette pe;
	QPalette palette;

	QVector<QString> audioTypes;
	QVector<QString> vedioTypes;

	QHBoxLayout *horizontalLayout;
	QWidget *BaseView;
	QPushButton *BackButton;//取消

	QWidget *AudioView;
	QPushButton *AudioButton;
	QComboBox *AudioComboBox;
	QPushButton *AudioFileButton;//添加
	QPushButton *AudioOKButton;//添加

	QWidget *VedioView;
	QPushButton *VedioButton;
	QComboBox *VedioComboBox;
	QPushButton *VedioFileButton;//添加
	QPushButton *VedioOKButton;//添加

signals:

	void updateList(Task*);

private slots:

	void addAudio();
	void AudioFile();
	void on_sel_audio(const int &);
	void AudioOK();
	void AudioBack();

	void addVedio();
	void VedioFile();
	void on_sel_vedio(const int &);
	void VedioOK();
	void VedioBack();

};

#endif // AddView_H