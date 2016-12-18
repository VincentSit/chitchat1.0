
#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QPen>
#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>

class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWidget(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);//�滭�¼�
    void resizeEvent(QResizeEvent *);
    void mouseMoveEvent(QMouseEvent *);//����ƶ��¼�
    void mousePressEvent(QMouseEvent *);
public:
    int style;
    int width;
    QColor color;
    QPoint startPos;
    QPixmap *pix;


signals:

public slots:
    void setStyle(int);//���û��ʷ��
    void setWidth(int);
    void setColor(QColor);
    void Clear();

};

#endif // DRAWWIDGET_H
