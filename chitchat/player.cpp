#include "player.h"
#include "ui_player.h"
/**
 * @brief Player::Player
 * @param parent
 */
Player::Player(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Player)
{
    ui->setupUi(this);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    setWindowTitle(tr("���ֲ�����"));
    setWindowIcon(QIcon(":/images/Music.png"));

    /**
     *���ñ���ͼƬ
     * @brief pal
     */
    QPalette pal;
    QPixmap pixmap(":/images/background.jpg");
    pal.setBrush(QPalette::Window,QBrush(pixmap));
    setPalette(pal);

    // ����������Ŀ
   ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(3);
    // ���ñ�ͷ��ǩ
    QStringList list;
    list << tr("����") << tr("������") << tr("����");
    ui->tableWidget->setHorizontalHeaderLabels(list);
    // ����ֻ��ѡ����
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    // ���ò���ʾ����
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget->setPalette(Qt::blue);

    playerInit();

    setFixedSize(700,430);
}

void Player::playerInit(){

    /**
     *����ý��
     */
    music = new Phonon::MediaObject(this);
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::createPath(music, audioOutput);
    connect(music,SIGNAL(tick(qint64)),this,SLOT(updateTime(qint64)));//���½���

    volumeSlider=new Phonon::VolumeSlider(audioOutput,this);//��������
    seekSlider=new Phonon::SeekSlider(music,this);
    seekSlider->hide();


    ui->timtLabel->setText(tr("00:00/00:00"));

    play=new QAction(QIcon(":/images/playerIcon/play.png"),tr("����"),this);
    noPlay=new QAction(QIcon(":/images/playerIcon/noplay.png"),tr("������"),this);
    skipForWard=new QAction(QIcon(":/images/playerIcon/skipForward.png"),tr("ǰһ��"),this);
    skipBackWard=new QAction(QIcon(":/images/playerIcon/skipBackward.png"),tr("��һ��"),this);
    open=new QAction(QIcon(":/images/Music.png"),tr("������"),this);
    lrc=new QAction(QIcon(":/images/playerIcon/lrc.png"),tr("��ʾ���"),this);

    connect(open,SIGNAL(triggered()),this,SLOT(Open()));
    connect(play,SIGNAL(triggered()),this,SLOT(Play()));
    connect(noPlay,SIGNAL(triggered()),this,SLOT(Noplay()));
    connect(skipForWard,SIGNAL(triggered()),this,SLOT(SkipForWard()));
    connect(skipBackWard,SIGNAL(triggered()),this,SLOT(SkipBackWard()));
    connect(lrc,SIGNAL(triggered()),this,SLOT(showLRC()));

    toolBar=addToolBar(tr("������"));
    toolBar->addAction(skipBackWard);
    toolBar->addAction(play);
    toolBar->addAction(noPlay);
    toolBar->addAction(skipForWard);
    toolBar->addAction(open);
    toolBar->addAction(lrc);
    toolBar->addWidget(volumeSlider);
    //music->setCurrentSource(Phonon::MediaSource("../myPlayer/music.mp3"));
    connect(music,SIGNAL(stateChanged(Phonon::State,Phonon::State)),this,SLOT(stateChanged(Phonon::State,Phonon::State)));

    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(tableChilked(int)));

    listMusic=new Phonon::MediaObject(this);
    Phonon::AudioOutput *output=new Phonon::AudioOutput(Phonon::MusicCategory,this);
    Phonon::createPath(listMusic,output);
    connect(listMusic,SIGNAL(stateChanged(Phonon::State,Phonon::State)),this,SLOT(metaStateChanged(Phonon::State,Phonon::State)));

    connect(music,SIGNAL(currentSourceChanged(Phonon::MediaSource)),this,SLOT(sourceChanged(Phonon::MediaSource)));

    connect(music,SIGNAL(aboutToFinish()),this,SLOT(aboutToFinsh()));

}

Player::~Player()
{
    delete ui;
}
/**
 *�˳����ֲ�����
 * @brief Player::on_exitButton_clicked
 */
void Player::on_exitButton_clicked()
{
    this->close();
}
/**
 *���½���
 * @brief Player::updateTime
 * @param time
 */
void Player::updateTime(qint64 time){

    qint64 totalTimeValue = music->totalTime();//��ȡʱ��
    QTime totalTime(0, (totalTimeValue / 60000) % 60, (totalTimeValue / 1000) % 60);
    QTime currentTime(0, (time / 60000) % 60, (time / 1000) % 60);
    QString str = currentTime.toString("mm:ss") + " / " + totalTime.toString("mm:ss");
    ui->timtLabel->setText(str);
}
/**
 *���ֲ���״̬�ı�
 * @brief Player::stateChanged
 * @param s1
 * @param s2
 */
void Player::stateChanged(Phonon::State s1, Phonon::State s2){

    switch(s1){
    case Phonon::ErrorState:
        if(music->errorType()==Phonon::FatalError){
            QMessageBox::warning(this,tr("�����"),music->errorString());

        }else{
            QMessageBox::warning(this,tr("����"),music->errorString());

        }
        break;
    case Phonon::PlayingState:
        noPlay->setEnabled(true);
    case Phonon::StoppedState:
        noPlay->setEnabled(false);
    case Phonon::PausedState:
        noPlay->setEnabled(true);
    case Phonon::BufferingState:
        break;
    default:
        ;
    }
}
/**
 *������Դ�ı�
 * @brief Player::sourceChanged
 * @param sou
 */
void Player::sourceChanged(const Phonon::MediaSource &sou){

    int index=sources.indexOf(sou);
    ui->tableWidget->selectRow(index);
    changeActionState();

}
/**
 * @brief Player::aboutToFinsh
 */
void Player::aboutToFinsh(){
    int index=sources.indexOf(music->currentSource())+1;
    if(sources.size()>index){
        music->enqueue(sources.at(index));//���
        music->seek(music->totalTime());
    }else{
        music->stop();
    }

}
/**
 * @brief Player::metaStateChanged
 * @param s1
 * @param s2
 */
void Player::metaStateChanged(Phonon::State s1, Phonon::State s2){

    if(s1==Phonon::ErrorState){
        QMessageBox::warning(this,tr("���ļ�ʱ����!"),listMusic->errorString());
        while(!sources.isEmpty()&&!(sources.takeLast()==listMusic->currentSource())){

        }
        return;
    }

    if(s1!=Phonon::StoppedState&&s1!=Phonon::PausedState)
        return;
    if(listMusic->currentSource().type()==Phonon::MediaSource::Invalid)
        return;

    QMap<QString,QString> metaData=listMusic->metaData();

    QString title=metaData.value("TITLE");

    if(title==""){
        QString str=listMusic->currentSource().fileName();
        title=QFileInfo(str).baseName();
    }
    /**
     *����
     * @brief titleItem
     */
    QTableWidgetItem *titleItem=new QTableWidgetItem(title);
    titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);

    titleItem->setFlags(titleItem->flags()^Qt::ItemIsEditable);
    /**
     *������
     * @brief artistItem
     */
    QTableWidgetItem *artistItem=new QTableWidgetItem(metaData.value("ARTIST"));

    artistItem->setFlags(titleItem->flags()^Qt::ItemIsEditable);

    qint64 totalTime=listMusic->totalTime();

    QTime time(0,(totalTime/60000)%60,(totalTime/1000)%60);
    /**
     *����ʱ��
     * @brief timeItem
     */
    QTableWidgetItem *timeItem=new QTableWidgetItem(time.toString("mm::ss"));

    int row=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row,0,titleItem);
    ui->tableWidget->setItem(row,1,artistItem);
    ui->tableWidget->setItem(row,2,timeItem);

    int index=sources.indexOf(listMusic->currentSource())+1;

    if(sources.size()>index){
        listMusic->setCurrentSource(sources.at(index));//���õ�ǰ���ŵ�����
    }else{
        if(ui->tableWidget->selectedItems().isEmpty()){

            if(music->state()!=Phonon::PlayingState&&music->state()!=Phonon::PausedState){
                music->setCurrentSource(sources.at(0));
            }else{
                ui->tableWidget->selectRow(0);
                changeActionState();
            }

        }else{
            changeActionState();
        }
    }
}
/**
 *
 * @brief Player::changeActionState
 */
void Player::changeActionState(){
    if(sources.count()==0){
        if(music->state()!=Phonon::PlayingState&&music->state()!=Phonon::PausedState){
            play->setEnabled(false);
            noPlay->setEnabled(false);
        }
        skipBackWard->setEnabled(false);
        skipForWard->setEnabled(false);
    }else{
        skipBackWard->setEnabled(true);
        skipForWard->setEnabled(true);
        int index=ui->tableWidget->currentRow();
        if(index==0)
            skipBackWard->setEnabled(false);
        if(index+1==sources.count())
           skipForWard->setEnabled(false);
    }
}
/**
 *����tableWidgetʱ����ѡ�е�����
 * @brief Player::tableChilked
 * @param row
 */
void Player::tableChilked(int row){

    bool wasPlaying=music->state()==Phonon::PlayingState;
    music->stop();
    music->clearQueue();

    if(row>=sources.size())
        return;
    music->setCurrentSource(sources.at(row));
    if(wasPlaying)
        music->play();
}
/**
 *
 * @brief Player::clearSources
 */
void Player::clearSources(){
    sources.clear();
    changeActionState();
}
/**
 *�������ļ�
 * @brief Player::Open
 */
void Player::Open(){
    // ��ϵͳ����Ŀ¼�򿪶�������ļ�
    QStringList list = QFileDialog::getOpenFileNames(this, tr("�������ļ�"),
                         QDesktopServices::storageLocation(QDesktopServices::MusicLocation));

    if(list.isEmpty())
        return;
    int index=sources.size();
    /**
     *����������Դ�б������ּ���
     */
    foreach(QString string,list){
        Phonon::MediaSource source(string);
        sources.append(source);
    }
    if(!sources.isEmpty()){
         listMusic->setCurrentSource(sources.at(index));
    }
}
/**
 *����
 * @brief Player::Play
 */
void Player::Play(){
    music->play();
}
/**
 *������
 * @brief Player::Noplay
 */
void Player::Noplay(){
    music->pause();
}
/**
 *��һ��
 * @brief Player::SkipBackWard
 */
void Player::SkipBackWard(){

    int index=sources.indexOf(music->currentSource());
    music->setCurrentSource(sources.at(index-1));
    music->play();
}

/**
 *ǰһ��
 * @brief Player::SkipForWard
 */
void Player::SkipForWard(){

    int index=sources.indexOf(music->currentSource());
    music->setCurrentSource(sources.at(index+1));
    music->play();
}
/**
 *��ʾ��ʣ���ûʵ��
 * @brief Player::showLRC
 */
void Player::showLRC(){



}

// ��ղ����б�
void Player::clearPlaylist()
{
    while (ui->tableWidget->rowCount())
        ui->tableWidget->removeRow(0);
    // ���䲥���б�������ź�
    //emit playlistClean();
}
/**
 *
 * @brief Player::contextMenuEvent
 * @param event
 */
void Player::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu;
    menu.addAction(tr("����б�"),this,SLOT(clearPlaylist()));
    menu.exec(event->globalPos());
}

