#ifndef PLAYER_H
#define PLAYER_H



#include <QAction>
#include <QToolBar>
#include <Phonon>
#include <QTime>
#include <QMessageBox>
#include <QCloseEvent>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTextCodec>
#include <QMainWindow>

namespace Ui {
class Player;
}
/**
 *���������ܵ�ͷ�ļ�
 * @brief The Player class
 */
class Player : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Player(QWidget *parent = 0);
    void playerInit();
    ~Player();

protected:
     void contextMenuEvent(QContextMenuEvent *event);//���ݲ˵��¼�

 private:
        Ui::Player *ui;
        QAction *play;
        QAction *noPlay;
        QAction *skipBackWard;//��һ��
        QAction *skipForWard;//ǰһ��
        QAction *open;
        QAction *lrc;

        QToolBar *toolBar;

        Phonon::MediaObject *music;//ý�����
        Phonon::AudioOutput *audioOutput;
        Phonon::VolumeSlider *volumeSlider;//����������
        Phonon::SeekSlider *seekSlider;//

        QList<Phonon::MediaSource> sources;//�б����ڴ洢���������ļ�
        Phonon::MediaObject *listMusic;
         void changeActionState();
signals:
       void playlistClean();

private slots:
    void updateTime(qint64 time);//����ʱ��
    void clearPlaylist();

    void stateChanged(Phonon::State s1,Phonon::State s2);

    void sourceChanged(const Phonon::MediaSource &sou);
    void aboutToFinsh();
    void metaStateChanged(Phonon::State s1,Phonon::State s2);
    void tableChilked(int row);//����tableWidget
    void clearSources();

    void Open();
    void Play();
    void Noplay();
    void SkipBackWard();
    void SkipForWard();
    void showLRC();
    void on_exitButton_clicked();

};

#endif // PLAYER_H
