#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlDatabase>

#include "chitchat.h"
#include "ui_chitchat.h"
/**
 * @brief Chitchat::Chitchat
 * @param nickName
 * @param parent
 */
Chitchat::Chitchat(QString nickName,QWidget *parent) :
    QMainWindow(parent),name(nickName),
    ui(new Ui::Chitchat)
{
    ui->setupUi(this);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    setWindowIcon(QIcon(":/images/dove.png"));
    setWindowTitle(tr("�׸������������"));

    //���ñ���
    QPalette pal;
    QPixmap pixmap(":/images/background.jpg");
    pal.setBrush(QPalette::Window,QBrush(pixmap));
    setPalette(pal);

    createAction();
    createToolBar();


    connect(ui->boldBtn,SIGNAL(clicked()),this,SLOT(Bold()));
    connect(ui->ItalicBtn,SIGNAL(clicked()),this,SLOT(Italic()));
    connect(ui->underLineBtn,SIGNAL(clicked()),this,SLOT(UnderLine()));
    connect(ui->textEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentFormatChanged(const QTextCharFormat)));
    connect(ui->closeBtn,SIGNAL(clicked()),this,SLOT(Close()));

    /**
     *
     *���ý�����ԣ������ı��򲻿��Ծۼ�����
     */
    ui->textEdit->setFocusPolicy(Qt::StrongFocus);
    ui->textBrowser->setFocusPolicy(Qt::NoFocus);

    ui->textEdit->setFocus();
    ui->textEdit->installEventFilter(this);//�����¼��˹���
    privatechat=NULL;//˽��
    privatechat1=NULL;

    /**
     *udpͨ��,�˿�Ϊ45455
     */
    udpSocket = new QUdpSocket(this);
    port = 45455;
    bb=0;
    udpSocket->bind(port,QUdpSocket::ShareAddress
                                    | QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
    sendMessage(NewParticipant);

    server = new TcpServer(this);
    connect(server,SIGNAL(sendFileName(QString)),this,SLOT(sentFileName(QString)));
    //connect(ui->textEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentFormatChanged(const QTextCharFormat)));
    setFixedSize(926,578);
}

void Chitchat::init(){

}
/**
 *����Action
 * @brief Chitchat::createAction
 */
void Chitchat::createAction(){

  sendFile=new QAction(QIcon(":/images/send.png"),tr("�����ļ�"),this);
  music=new QAction(QIcon(":/images/Music.png"),tr("����"),this);
  save=new QAction(QIcon(":/images/save.png"),tr("���������¼"),this);
  clear=new QAction(QIcon(":/images/clear.png"),tr("����"),this);
  note=new QAction(QIcon(":/images/note.png"),tr("���±�"),this);
  explore=new QAction(QIcon(":/images/explore.png"),tr("�����"),this);
  exitLogin=new QAction(QIcon(":/images/exitLogin.png"),tr("�˳���¼"),this);
  grapWindowScreen=new QAction(QIcon(":/images/img.png"),tr("��ͼ����"),this);

  connect(sendFile,SIGNAL(triggered()),this,SLOT(SendFile()));
  connect(music,SIGNAL(triggered()),this,SLOT(showPlayer()));
  connect(save,SIGNAL(triggered()),this,SLOT(Save()));
  connect(clear,SIGNAL(triggered()),this,SLOT(Clear()));
  connect(note,SIGNAL(triggered()),this,SLOT(showNote()));
  connect(exitLogin,SIGNAL(triggered()),this,SLOT(ExitLogin()));
  connect(grapWindowScreen,SIGNAL(triggered()),this,SLOT(showGrapScreenTool()));
  connect(explore,SIGNAL(triggered()),this,SLOT(showExplore()));

}
/**
 *������
 * @brief Chitchat::createToolBar
 */
void Chitchat::createToolBar(){
    toolBar=addToolBar(tr("������"));
    toolBar->addAction(sendFile);
    toolBar->addAction(music);
    toolBar->addAction(save);
    toolBar->addAction(clear);
    toolBar->addAction(note);
    toolBar->addAction(exitLogin);
    toolBar->addAction(explore);
    toolBar->addAction(grapWindowScreen);

}
/**
 *
 *��������UDP
 * @brief Chitchat::processPendingDatagrams
 */
void Chitchat::processPendingDatagrams()   //��������UDP
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;//�ֽ�����
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);//������
        int messageType;
        in >> messageType;
        QString userName,localHostName,ipAddress,message;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        switch(messageType)
        {
        //������յ�����Ϣ
            case Message:
                {
                    in >>userName >>localHostName >>ipAddress >>message;//���նԷ����û�����ip������Ϣ
                    ui->textBrowser->setTextColor(Qt::blue);
                    ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
                    ui->textBrowser->append("[ " +userName+" ] "+ time);//���Է��û����ӵ��ı������
                    ui->textBrowser->append(message);//���յ�����Ϣ�ӵ��ı������
                    break;
                }
            //���û�����
            case NewParticipant:
                {
                    in >>userName >>localHostName >>ipAddress;//���û����û�������������ip
                    newParticipant(userName,localHostName,ipAddress);

                    break;
                }
            //�û��뿪
            case ParticipantLeft:
                {
                    in >>userName >>localHostName;
                    participantLeft(userName,localHostName,time);
                    break;
                }
            //�����ļ�
        case FileName:
            {
                in >>userName >>localHostName >> ipAddress;
                QString clientAddress,fileName;
                in >> clientAddress >> fileName;
                hasPendingFile(userName,ipAddress,clientAddress,fileName);
                break;
            }
            //�ܾ�
        case Refuse:
            {
                in >> userName >> localHostName;
                QString serverAddress;
                in >> serverAddress;
                QString ipAddress = getIP();

                if(ipAddress == serverAddress)
                {
                    server->refused();
                }
                break;
            }
        case Xchat:
           {
                in >>userName >>localHostName >>ipAddress;
                /****/
                 // showxchat(userName,ipAddress);
               showxchat(localHostName,ipAddress);//��ʾ�������������У������û���
                break;
           }
        }
    }
}
/**
 *�������û�����
 * @brief Chitchat::newParticipant
 * @param userName
 * @param localHostName
 * @param ipAddress
 */
void Chitchat::newParticipant(QString userName,QString localHostName,QString ipAddress)
{
    bool bb = ui->tableWidget->findItems(localHostName,Qt::MatchExactly).isEmpty();
    if(bb)
    {
        QTableWidgetItem *user = new QTableWidgetItem(userName);//�û���
        QTableWidgetItem *host = new QTableWidgetItem(localHostName);//������
        QTableWidgetItem *ip = new QTableWidgetItem(ipAddress);//ip
        ui->tableWidget->insertRow(0);
        //����һ��ͼƬ,��������ʵ���û��ϴ��Լ���ͼƬ
        QLabel *imgLabel=new QLabel;
        imgLabel->setPixmap(QPixmap(":/images/usericon/yy.png"));
        ui->tableWidget->setCellWidget(0,0,imgLabel);

        ui->tableWidget->setItem(0,1,user);
        ui->tableWidget->setItem(0,2,host);
        ui->tableWidget->setItem(0,3,ip);


        ui->textBrowser->setTextColor(Qt::gray);
        ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
        ui->textBrowser->append(tr("%1 ���ߣ�").arg(userName));
        ui->onLineUsersNum->setText(tr("����������%1").arg(ui->tableWidget->rowCount())+tr(",��¼��:%1").arg(name));
        sendMessage(NewParticipant);//������Ϣ
    }
}

/**
 *�����û��뿪
 * @brief Chitchat::participantLeft
 * @param userName
 * @param localHostName
 * @param time
 */
void Chitchat::participantLeft(QString userName,QString localHostName,QString time)
{
    int rowNum = ui->tableWidget->findItems(localHostName,Qt::MatchExactly).first()->row();
    ui->tableWidget->removeRow(rowNum);
    ui->textBrowser->setTextColor(Qt::gray);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->textBrowser->append(tr("%1 �� %2 �뿪��").arg(userName).arg(time));
    ui->onLineUsersNum->setText(tr("����������%1").arg(ui->tableWidget->rowCount()));
}

/**
 *�ı��¼�
 * @brief Chitchat::changeEvent
 * @param e
 */
void Chitchat::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
/**
 *��ȡip
 * @brief Chitchat::getIP
 * @return
 */
QString Chitchat::getIP()  //��ȡip��ַ
{
    //��ȡ����ip,�ŵ��б���
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    /**
     *�����б����ipЭ����IPve4�ģ����ȡ
     */
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol) //����ʹ��IPv4��ַ
            return address.toString();//��QHostAddress���͵�ipת��Ϊ�ַ�����ʽ
    }
    //�񣬷���0
       return 0;
}
/**
 * ������Ϣ
 *@brief Chitchat::sendMessage
 * @param type
 * @param serverAddress
 */
void Chitchat::sendMessage(MessageType type, QString serverAddress)  //������Ϣ
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString localHostName = QHostInfo::localHostName();
    QString address = getIP();
    out << type << getUserName() << localHostName;

    switch(type)
    {
        case ParticipantLeft:
            {
                break;
            }
        case NewParticipant:
            {
                out << address;
                break;
            }

        case Message :
            {
                if(ui->textEdit->toPlainText() == "")
                {
                    QMessageBox::warning(0,tr("����"),tr("�������ݲ���Ϊ��"),QMessageBox::Ok);
                    return;
                }
               out << address << getMessage();
               ui->textEdit->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
               break;

            }
        case FileName:
            {
                int row = ui->tableWidget->currentRow();//��ȡQTableWidget����
                QString clientAddress = ui->tableWidget->item(row,3)->text();//�ͻ���ip
                out << address << clientAddress << fileName;
                break;
            }
        case Refuse:
            {
                out << serverAddress;
                break;
            }
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast, port);
}
/**
 *��ȡ�û���
 * @brief Chitchat::getUserName
 * @return
 */
QString Chitchat::getUserName()  //��ȡ�û���
{

    return name;
}
/**
 *��ȡҪ���͵���Ϣ
 * @brief Chitchat::getMessage
 * @return
 */
QString Chitchat::getMessage()  //���Ҫ���͵���Ϣ
{
    QString msg = ui->textEdit->toHtml();//ת����ʽ

    ui->textEdit->clear();
    ui->textEdit->setFocus();
    return msg;
}
/**
 *�ر��¼�
 * @brief Chitchat::closeEvent
 */
void Chitchat::closeEvent(QCloseEvent *)
{
    sendMessage(ParticipantLeft);
}

/**
 *�����ļ���
 * @brief Chitchat::sentFileName
 * @param fileName
 */
void Chitchat::sentFileName(QString fileName)
{
    this->fileName = fileName;
    sendMessage(FileName);
}
/**
 *�����ļ�
 * @brief Chitchat::hasPendingFile
 * @param userName
 * @param serverAddress
 * @param clientAddress
 * @param fileName
 */
void Chitchat::hasPendingFile(QString userName,QString serverAddress,  //�����ļ�
                            QString clientAddress,QString fileName)
{
    QString ipAddress = getIP();
    if(ipAddress == clientAddress)
    {
        int btn = QMessageBox::information(this,tr("�����ļ�"),
                                 tr("����%1(%2)���ļ���%3,�Ƿ���գ�")
                                 .arg(userName).arg(serverAddress).arg(fileName),
                                 QMessageBox::Yes,QMessageBox::No);
        if(btn == QMessageBox::Yes)
        {
            QString name = QFileDialog::getSaveFileName(0,tr("�����ļ�"),fileName);
            if(!name.isEmpty())
            {
                TcpClient *client = new TcpClient(this);
                client->setFileName(name);
                client->setHostAddress(QHostAddress(serverAddress));
                client->show();

            }

        }
        else{
            sendMessage(Refuse,serverAddress);
        }
    }
}

/**
 *�رմ���
 * @brief Chitchat::Close
 */
void Chitchat::Close(){
    QMessageBox::StandardButton bt=QMessageBox::question(this,tr("�رմ���"),
    tr("�Ƿ�رմ���"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);

    if(bt==QMessageBox::Cancel){
        return;
    }else if(bt==QMessageBox::Yes){
        this->close();
    }
}

/**
 *���������¼
 * @brief Chitchat::Save
 */
void Chitchat::Save(){
    QString path=QFileDialog::getSaveFileName(this,tr("�����ļ�"),"records.txt","(*.*)");
    QFile file(path);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("�����ļ�"),
                                 tr("�޷������ļ� %1:\n %2").arg(path)
                                 .arg(file.errorString()));

        }
        QTextStream out(&file);

        QString r= ui->textBrowser->toPlainText();

        out<<r;

        QSqlDatabase db=QSqlDatabase::addDatabase("SQLITE");

        db.setDatabaseName("chatingRecords.db");


        QSqlQuery query;

        QVariantList rec;

        rec<<r;

        query.addBindValue(rec);

        if(!query.execBatch())//�������������������������qDebug()<<query.lastError();

        QMessageBox::warning(this,tr("���������¼!"),tr("����ɹ�!"));

}
/**
 *������Ϣ
 * @brief Chitchat::on_sentBtn_clicked
 */

void Chitchat::on_sentBtn_clicked()
{
       sendMessage(Message);
}
/**
 *�¼��˹���
 * @brief Chitchat::eventFilter
 * @param target
 * @param event
 * @return
 */
bool Chitchat::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return)
             {
                 on_sentBtn_clicked();
                 return true;
             }
        }
    }
    return QWidget::eventFilter(target,event);
}
/**
 *�����ļ�
 * @brief Chitchat::SendFile
 */
void Chitchat::SendFile(){
    if(ui->tableWidget->selectedItems().isEmpty()){
          QMessageBox::warning(0,tr("ѡ���û�"),tr("���ȴ��û��б�ѡ��Ҫ���͵��û���"),QMessageBox::Ok);
          return;
    }
      server->show();
      server->initServer();
}
/**
 *��ʾ���ֲ�����
 * @brief Chitchat::showPlayer
 */
void Chitchat::showPlayer(){
    Player *player=new Player;
    player->show();
}
/**
 *����
 * @brief Chitchat::Clear
 */
void Chitchat::Clear(){
    ui->textBrowser->clear();
}
/**
 *��ʾ���±�
 * @brief Chitchat::showNote
 */
void Chitchat::showNote(){

    Note *n=new Note;
    n->show();
}
/**
 *
 *��Ĭ�����������ҳ
 * @brief Chitchat::showExplore
 */

void Chitchat::showExplore(){

    const QUrl AuthorUrl("http://hao.qq.com/");
    QDesktopServices::openUrl(AuthorUrl);
}

/**
 *��ʾ��ͼ����
 * @brief Chitchat::showGrapScreenTool
 */
void Chitchat::showGrapScreenTool(){
    GrapScreen *g=new GrapScreen;
    g->show();
}

/**
 *�˳���¼�������ص�¼����
 * @brief Chitchat::ExitLogin
 */
void Chitchat::ExitLogin(){
    this->close();
    Login *l=new Login;
    l->show();
}

Chitchat::~Chitchat()
{
    delete ui;
}

/**
 *�رմ���
 * @brief Chitchat::on_colorBtn_clicked
 */
void Chitchat::on_colorBtn_clicked()
{
    QColor color=QColorDialog::getColor(Qt::white,this);
    if(color.isValid()){
        ui->textEdit->setTextColor(color);
        ui->textEdit->setFocus();
    }
}
/**
 *���ͱ��飬��ûʵ��
 * @brief Chitchat::on_expression_clicked
 */
void Chitchat::on_expression_clicked()
{
    list=new QListWidget;

}
/**
 *����ͼƬ
 * @brief Chitchat::on_picBtn_clicked
 */
void Chitchat::on_picBtn_clicked()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextImageFormat format;
    QString image = QFileDialog::getOpenFileName(this,"insert image","/","png(*.png)");
    format.setName(image);
    cursor.insertImage(format);
}
/**
 *��ʾͿѻ��
 * @brief Chitchat::on_showPanelBtn_clicked
 */
void Chitchat::on_showPanelBtn_clicked()
{
    doodle=new DoodleBoard;
    doodle->show();
}


/**
 *���Ϳ���������
 * @brief Chitchat::on_fontComboBox_currentFontChanged
 * @param font
 */
void Chitchat::on_fontComboBox_currentFontChanged(const QFont font)
{
    ui->textEdit->setFont(font);
    ui->textEdit->setFocus();
}

/**
 *���Ϳ������С����
 * @brief Chitchat::on_fontSizeComboBox_currentIndexChanged
 * @param size
 */
void Chitchat::on_fontSizeComboBox_currentIndexChanged(const QString size)
{
    ui->textEdit->setFontPointSize(size.toDouble());
    ui->textEdit->setFocus();
}

/**
 *�����ı�������Ӵ�
 * @brief Chitchat::Bold
 */
void Chitchat::Bold()
{
    QTextCharFormat format;
    format.setFontWeight(ui->boldBtn->isChecked()?QFont::Bold:QFont::Normal);
    ui->textEdit->mergeCurrentCharFormat(format);
}
/**
 *�ı�������б������
 * @brief Chitchat::Italic
 */
void Chitchat::Italic()
{
    QTextCharFormat format;
    format.setFontItalic(ui->ItalicBtn->isChecked());
    ui->textEdit->mergeCurrentCharFormat(format);
}
/**
 *�»���
 * @brief Chitchat::UnderLine
 */
void Chitchat::UnderLine()
{
    QTextCharFormat format;
    format.setFontUnderline(ui->underLineBtn->isChecked());
    ui->textEdit->mergeCurrentCharFormat(format);
}
/**
 *QTableWidget����
 * @brief Chitchat::on_tableWidget_clicked
 * @param index
 */
void Chitchat::on_tableWidget_clicked(QModelIndex index)
{

            if(!privatechat){

            privatechat = new PrivateChat(ui->tableWidget->item(index.row(),1)->text(), //����������
                                   ui->tableWidget->item(index.row(),3)->text()) ;//�����û�IP
            }

            QByteArray data;
            QDataStream out(&data,QIODevice::WriteOnly);
            QString localHostName = QHostInfo::localHostName();
            QString address = getIP();
//            out << Xchat << getUserName() << localHostName << address;
              out<< getUserName() << localHostName << address;
            udpSocket->writeDatagram(data,data.length(),QHostAddress::QHostAddress(ui->tableWidget->item(index.row(),3)->text()), port);

                privatechat->show();
            privatechat->is_opened = true;

 }

void Chitchat::showxchat(QString name, QString ip)
{

    if(!privatechat1)
        /****/
    privatechat1 = new PrivateChat(getUserName(),ip);

    bb++;


}
