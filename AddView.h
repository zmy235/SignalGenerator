//ͷ�ļ���AddView.h
#ifndef AddView_H
#define AddView_H

#include "BaseWidget.h"

//���õ������ǰ������
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

//�̳���QWidget���Զ��崰�ڣ��������ڲ��ƶ������϶�����
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
	QPushButton *BackButton;//ȡ��

	QWidget *AudioView;
	QPushButton *AudioButton;
	QComboBox *AudioComboBox;
	QPushButton *AudioFileButton;//���
	QPushButton *AudioOKButton;//���

	QWidget *VedioView;
	QPushButton *VedioButton;
	QComboBox *VedioComboBox;
	QPushButton *VedioFileButton;//���
	QPushButton *VedioOKButton;//���

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