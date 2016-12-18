#ifndef DOODLEBOARD_H
#define DOODLEBOARD_H

#include "drawwidget.h"


#include <QMenu>
#include <QLabel>
#include <QPixmap>
#include <QMenuBar>
#include <QToolBar>
#include <QSpinBox>
#include <QComboBox>
#include <QTextCursor>
#include <QTextStream>
#include <QPrintDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <QMainWindow>
#include <QColorDialog>
/**
 *����ʵ��Ϳѻ�幦�ܵ�ͷ�ļ�
 * @brief The DoodleBoard class
 */
class DoodleBoard : public QMainWindow
{
    Q_OBJECT
public:
    explicit DoodleBoard(QWidget *parent = 0);
    void fileInit();//�ļ�������������ʼ
    void editInit();//�༭������������ʼ
    void createToolBar();//����������
private:

    DrawWidget *draw;//����

    QToolBar *fileTool;//�ļ�������
    QToolBar *editTool;//�༭������
    QLabel *styleLabel;
    QComboBox *styleCom;//���ʷ�������˵�
    QLabel *widthLabel;
    QSpinBox *widthSpain;//������������

    //�ļ������ද���¼�
    QAction *exit;
    QAction *load;
    QAction *save;

    //�༭�����ද���¼�
    QAction *color;
    QAction *clear;
    QAction *send;

private slots:

    void savePic();//����ͼƬ
    void loadPic();//����ͼƬ��Ϳѻ��
    void showStyle();//�ı仭�ʷ��
    void showColor();//�ı仭����ɫ
    
};

#endif // DOODLEBOARD_H
