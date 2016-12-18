#ifndef GRAPSCREEN_H
#define GRAPSCREEN_H

#include <QtGui>
#include <QAction>
#include <QLabel>
#include <QPoint>
#include <QPixmap>
#include <QToolBar>
#include <QTextCodec>
#include <QMouseEvent>
#include <QRubberBand>
#include <QApplication>
#include <QMainWindow>


namespace Ui {
class GrapScreen;
}
/**
 *��Ļ�������ܵ�ͷ�ļ�
 * @brief The GrapScreen class
 */
class GrapScreen : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GrapScreen(QWidget *parent = 0);
    void createEventFilter();
    void createWindow();
     bool eventFilter(QObject *, QEvent *);
    ~GrapScreen();
    
private:
    Ui::GrapScreen *ui;
    QPoint point1;//��꿪ʼ����ĵ�
    QPoint point2;

    QAction *save;//�����ͼ
    QAction *grapWindowGreen;//����
    QToolBar *toolBar;

    QLabel *saveLabel;
    QLabel *fullScreenLabel;

    QPixmap fullScreenPixmap;

    QRubberBand *rubberBand;//��Ƥ��

     bool leftMousePress;//����Ƿ����

private slots:

    void savePic();


    void GrapWindowScreen();

};

#endif // GRAPSCREEN_H
