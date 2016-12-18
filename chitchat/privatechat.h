#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include "note.h"
#include "grapscreen.h"
#include "player.h"
#include "tcpclient.h"
#include "tcpserver.h"
#include "doodleboard.h"


#include <QtNetwork>
#include <QtGui>
#include <QMenu>
#include <QColor>
#include <QPalette>
#include <QListWidget>
#include <QAction>
#include <QToolBar>
#include <QMainWindow>
#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QTextCharFormat>

enum MessageType
{
    Message,
    NewParticipant,
    ParticipantLeft,
    FileName,
    Refuse,
    Xchat
};
//ö�ٱ�����־��Ϣ�����ͣ��ֱ�Ϊ��Ϣ�����û����룬���û��˳�

namespace Ui {
class PrivateChat;
}
/**
 *
 *ʵ��˽�Ĺ��ܵ�ͷ�ļ�
 * @brief The PrivateChat class
 */

class PrivateChat : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrivateChat(QString pasvusername, QString pasvuserip,QWidget *parent = 0);
    QString xpasvuserip;//�û�ip
    QString xpasvusername;//��¼��

    void actionInit();
    void createToolBar();

    QUdpSocket *xchat;
       qint32 xport;//�˿�
       void sendMessage(MessageType type,QString serverAddress="");//������Ϣ
       quint16 a;
   //    static  qint32 is_opened = 0;
       bool is_opened;

    ~PrivateChat();
protected:
    void hasPendingFile(QString userName,QString serverAddress,  //�����ļ�
                                    QString clientAddress,QString fileName);
        void participantLeft(QString userName,QString localHostName,QString time);
        bool eventFilter(QObject *target, QEvent *event); //�¼�������

private:
    Ui::PrivateChat *ui;

    QAction *send;
    QAction *music;
    QAction *SaveRe;
    QAction *clear;
    QAction *note;
    QAction *explore;
    QAction *grapScreen;//����
    QToolBar *toolBar;

    TcpServer *server;
    QColor color;//��ɫ
    bool saveFile(const QString& fileName);//���������¼
    QString getMessage();
    QString getIP();
    QString getUserName();//����û���
    QString message;
    QString fileName;

private slots:
    void sentFileName(QString);

    void processPendingDatagrams();

    void Close();
    void save();
    void SendFile();
    void Clear();
    void showPlayer();
    void showNote();
    void showExplore();
    void showGrapScreenTool();

    void on_fontComboBox_currentFontChanged(const QFont font);
    void on_comboBox_currentIndexChanged(const QString size);
    void on_BoldButton_clicked();
    void on_ItalicButton_clicked();
    void on_UnderlineButton_clicked();
    void on_ColorButton_clicked();
    void on_expressionButton_clicked();//����
    void on_showPanelButton_clicked();//Ϳѻ��
    void on_insertPicButton_clicked();//����ͼƬ
    void on_sendButton_clicked();

};

#endif // PRVATECHAT_H
