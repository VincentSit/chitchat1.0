#ifndef CHITCHAT_H
#define CHITCHAT_H

#include "note.h"
#include "player.h"
#include "login.h"
#include "grapscreen.h"
#include "privatechat.h"
#include "doodleboard.h"
#include "tcpclient.h"
#include "tcpserver.h"

#include <QtGui>
#include <QMenu>
#include <QColor>
#include <QAction>
#include <QToolBar>
#include <QPalette>
#include <QtNetwork>
#include <QListWidget>
#include <QTableWidget>
#include <QTextCodec>
#include <QMainWindow>
#include <QColorDialog>

/**
 *ʵ��Ⱥ�Ĺ��ܵ�ͷ�ļ�,ͨ�����캯���ӵ�½���ﴫ��¼����Ϣ��Ⱥ�Ľ���
 */
namespace Ui {
class Chitchat;
}

class Chitchat : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Chitchat
     * @param nickName//��¼��
     * @param parent
     */
    explicit Chitchat(QString nickName,QWidget *parent = 0);
    void init();
    void createAction();//�����˵���
    void createToolBar();//������
    QString getUserName();//��ȡ��½��
    QString getMessage();//��ȡ��Ϣ

    PrivateChat *privatechat;//˽�������
    PrivateChat *privatechat1;

       QListWidget *list;//���ڷ��ͱ���,��ûʵ�ַ��ͱ���

    ~Chitchat();
protected:
    void changeEvent(QEvent *e);//�ı��¼�
    void sendMessage(MessageType type,QString serverAddress="");//�����û�״̬��������Ϣ
    void newParticipant(QString userName,QString localHostName,QString ipAddress);//���û�����
    void participantLeft(QString userName,QString localHostName,QString time);//�û��뿪
    void closeEvent(QCloseEvent *);//�ر��¼�
    void hasPendingFile(QString userName,QString serverAddress,
                        QString clientAddress,QString fileName);

     bool eventFilter(QObject *target, QEvent *event);//�¼�������



private:
    Ui::Chitchat *ui;
    qint32 bb;

    QString name;

    QToolBar *toolBar;
    QAction *sendFile;
    QAction *music;
    QAction *save;
    QAction *clear;
    QAction *note;
    QAction *explore;
    QAction *grapWindowScreen;
    QAction *exitLogin;
    QAction *privateChichat;

    QUdpSocket *udpSocket;
    qint32 port;
    QString fileName;
    TcpServer *server;

    DoodleBoard *doodle;

    QString getIP();

    QColor color;//��ɫ



    void showxchat(QString name, QString ip);


    bool saveFile(const QString& fileName);//���������¼

private slots:

    void Clear();
    void SendFile();
    void showPlayer();
    void showExplore();
    void showNote();
    void ExitLogin();
    void showGrapScreenTool();

    void Save();


    void processPendingDatagrams();
    void sentFileName(QString);
//    void currentFormatChanged(const QTextCharFormat &format);

//    void currentFormatChanged(const QTextCharFormat &format);

    void Close();
    void on_sentBtn_clicked();

    void on_colorBtn_clicked();
    void on_picBtn_clicked();
    void on_expression_clicked();
    void on_showPanelBtn_clicked();

    void on_fontComboBox_currentFontChanged(const QFont font);
    void on_fontSizeComboBox_currentIndexChanged(const QString size);

//    void on_boldBtn_clicked(bool checked);
//    void on_ItalicBtn_clicked(bool checked);
//    void on_underLineBtn_clicked(bool checked);
    void Bold();
    void Italic();

    void UnderLine();


    void on_tableWidget_clicked(QModelIndex index);
};

#endif // CHITCHAT_H
