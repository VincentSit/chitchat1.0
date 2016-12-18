#include "doodleboard.h"
/**
 * @brief DoodleBoard::DoodleBoard
 * @param parent
 */
DoodleBoard::DoodleBoard(QWidget *parent) :
    QMainWindow(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale()); //���ñ����ʽ

    setWindowTitle(tr("Ϳѻ��")); //���ô��ڱ���

    setWindowIcon(QIcon(":/images/gyachi.png"));//���ñ�����ͼ��

    QPalette pal;
    QPixmap pixmap(":/images/background.jpg");
    pal.setBrush(QPalette::Window,QBrush(pixmap));
    setPalette(pal);



    draw=new DrawWidget; //�����������

    setCentralWidget(draw);//���û���Ϊ���Ĳ���

    fileInit();
    editInit();

    createToolBar();//����������

    showStyle();//���ø�ʽ

    draw->setWidth(widthSpain->value());  //�����߿�

    draw->setColor(Qt::black);//������ɫ

    resize(500,400);

}

void DoodleBoard::fileInit(){
    //����

      save = new QAction(QIcon(":/images/save.png"),tr("����"),this);
      save->setStatusTip(tr("����"));
      connect(save,SIGNAL(triggered()),this,SLOT(savePic()));

      exit = new QAction(QIcon(":/images/exit.png"),tr("�˳�"),this);
      exit->setStatusTip(tr("�˳�"));
      connect(exit,SIGNAL(triggered()),this,SLOT(close()));
  //����ͼƬ
      load=new QAction(QIcon(":/images/load.png"),tr("����"),this);
      load->setStatusTip(tr("����ͼƬ"));
      connect(load,SIGNAL(triggered()),this,SLOT(loadPic()));



}

void DoodleBoard::editInit(){
    //��ʽ

    styleLabel = new QLabel(tr("���Է��:"));
    styleCom = new QComboBox;
    styleCom->setStatusTip(tr("�ı仭�ʷ��"));
    styleCom->addItem(tr("DashDotLine"),static_cast<int>(Qt::DashDotDotLine));
    styleCom->addItem(tr("DashDotDotLine"),static_cast<int>(Qt::DashDotDotLine));
    styleCom->addItem(tr("DashLine"),static_cast<int>(Qt::DashLine));
    styleCom->addItem(tr("DotLine"),static_cast<int>(Qt::DotLine));
    styleCom->addItem(tr("SolidLine"),static_cast<int>(Qt::SolidLine));
    connect(styleCom,SIGNAL(activated(int)),this,SLOT(showStyle()));
//�߿�
    widthLabel = new QLabel(tr("�߿�:"));
    widthSpain = new QSpinBox;
    widthSpain->setStatusTip(tr("�ı仭���߿�"));
    widthSpain->setValue(1);
    connect(widthSpain,SIGNAL(valueChanged(int)),draw,SLOT(setWidth(int)));
//��ɫ
    color = new QAction(QIcon(":/images/color.png"),tr("��ɫ"),this);
    color->setStatusTip(tr("�ı仭����ɫ"));
    QPixmap pix(30,30);
    pix.fill(Qt::green);
    connect(color,SIGNAL(triggered()),this,SLOT(showColor()));
//���
    clear = new QAction(QIcon(":/images/clear.png"),tr("���"),this);
    clear->setText(tr("���"));
    clear->setStatusTip(tr("���"));
    connect(clear,SIGNAL(triggered()),draw,SLOT(Clear()));
}



//����������
void DoodleBoard::createToolBar(){


//��������ӵ�������

    editTool=addToolBar(tr("�༭"));

    fileTool=addToolBar(tr("������"));

    editTool->addWidget(styleLabel);
    editTool->addWidget(styleCom);
    editTool->addWidget(widthLabel);
    editTool->addWidget(widthSpain);
    editTool->addAction(color);
    editTool->addAction(clear);

    fileTool->addAction(save);
    fileTool->addAction(load);
    fileTool->addAction(exit);

}
//��ʽ
void DoodleBoard::showStyle(){

    draw->setStyle(styleCom->itemData(styleCom->currentIndex(),Qt::UserRole).toInt());

}
//��ɫ
void DoodleBoard::showColor(){

    QColor col=QColorDialog::getColor(Qt::white,this);

    if(col.isValid()){
        draw->setColor(col);
        QPixmap pix(30,30);//����
        pix.fill(col);//���ѡ�����ɫ
        //color->setIcon(QIcon(pix));//���ð�ť��ɫ
    }
}


void DoodleBoard::savePic(){

    draw->pix->save("E://myDraw.png");
    QMessageBox::about(this,tr("ͼƬ����"),tr("ͼƬ����ɹ�!"));

}

void DoodleBoard::loadPic(){
    QString pic=QFileDialog::getOpenFileName(this,tr("���ļ�"),
                                                  "/","picture(*.png)");
    draw->pix->load(pic);
}
