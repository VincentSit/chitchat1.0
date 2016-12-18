#ifndef NOTE_H
#define NOTE_H



#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFont>
#include <QToolBar>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPrinter>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QActionGroup>
#include <QPrintDialog>
#include <QPushButton>
#include <QToolButton>
#include <QTextStream>
#include <QMessageBox>
#include <QTextCodec>
#include <QMainWindow>
#include <QFileDialog>
#include <QFontDialog>
#include <QApplication>
#include <QInputDialog>
#include <QFontDatabase>
#include <QFontComboBox>
#include <QColorDialog>
/**
 *���±����ܵ�ͷ�ļ�
 * @brief The Note class
 */

class Note : public QMainWindow
{
    Q_OBJECT

public:
    explicit Note(QWidget *parent = 0);
    void createAction();
    void createMenu();
    void createToolBar();
    void fileRead();//���ļ�
    void fileWrite();//д�ļ�


public:

    QMenu *file;
    QMenu *edit;
    QMenu *style;
    QMenu *insert;//����˵�
    QMenu *search;//���Ҳ˵�
    QMenu *help;//�����˵�

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *styleToolBar;
    QToolBar *insertToolBar;
    QToolBar *searchToolBar;
    QToolBar *helpToolBar;

    QTextEdit *textEdit;

    QAction *newFile;
    QAction *openFile;
    QAction *save;
    QAction *saveAs;
    QAction *printText;
    QAction *exit;

    QAction *undoAction;//����
    QAction *redoAction;//�ָ�
    QAction *copy;
    QAction *cut;
    QAction *paste;

    QAction *insertImg;
    QAction *insertTable;

    QAction *find;

    QAction *about;
    QAction *aboutQt;

    QAction *bold;
    QAction *italic;
    QAction *underLine;
    QAction *color;

    QAction *left;
    QAction *center;
    QAction *right;
    QAction *justify;
    QActionGroup *group;


    QLabel *fontLabel;
    QLabel *fontSizeLabel;
    QFontComboBox *fontCom;
    QComboBox *fontSizeCom;

    QString fileName;

    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *btn;
    QVBoxLayout *layout;

    QLineEdit *column;
    QLineEdit *row;



private slots:
    void findNext();

    void NewFile();
    void OpenFile();
    void Save();
    void SaveAs();
    void Color();
    void Bold();
    void Italic();
    void UnderLine();
    void ALignment(QAction *);
    void PrintText();
    void InsertImage();
    void InsertTable();
    void mergeFormat(QTextCharFormat);
    void FontComBox(QString fontComStr);
    void FontSizeCom(QString fontSizeStr);
    void Find();
    void About();
    void AboutQt();
    void showCurrentCharFormatChanged(const QTextCharFormat &form);

};


#endif // NOTE_H
