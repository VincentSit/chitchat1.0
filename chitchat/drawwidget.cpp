#include "drawwidget.h"
/**
 * @brief DrawWidget::DrawWidget
 * @param parent
 */
DrawWidget::DrawWidget(QWidget *parent) :
    QWidget(parent)
{

    setAutoFillBackground(true);
    QPalette palette(QPalette(Qt::white));

    pix = new QPixmap(size());

    pix->fill(Qt::white);//�����ɫ

}
//setX��������������������
void DrawWidget::setStyle(int s){
    style=s;
}

void DrawWidget::setColor(QColor c){
    color=c;
}

void DrawWidget::setWidth(int w){
    width=w;
}

//����ƶ��¼�
void DrawWidget::mouseMoveEvent(QMouseEvent *e){

    QPainter *painter = new QPainter;//��ӡ��

    QPen pen;//����
    pen.setStyle((Qt::PenStyle)style);
    pen.setWidth(width);
    pen.setColor(color);

    painter->begin(pix);
    painter->setPen(pen);
    painter->drawLine(startPos,e->pos());//����
    painter->end();
    startPos=e->pos();
    update();

}
//��갴���¼�
void DrawWidget::mousePressEvent(QMouseEvent *e){

    startPos=e->pos();

}
//paint�¼�
void DrawWidget::paintEvent(QPaintEvent *e){

    QPainter painter(this);
    painter.drawPixmap(QPoint(0,0),*pix);
}
//�������ô�С�¼�
void DrawWidget::resizeEvent(QResizeEvent *e){

    if(height()>pix->height()){

        QPixmap *newPix=new QPixmap(size());
        newPix->fill(Qt::white);
        QPainter painter(newPix);
        painter.drawPixmap(QPoint(0,0),*pix);
        pix=newPix;

    }

    QWidget::resizeEvent(e);
}
//����¼�
void DrawWidget::Clear(){

    QPixmap *clearPix = new QPixmap(size());

    clearPix->fill(Qt::white);

    pix=clearPix;//�´����Ļ�������ԭ���Ļ���

    update();
}
