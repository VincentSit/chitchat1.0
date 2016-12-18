#include "note.h"
/**
 * @brief Note::Note
 * @param parent
 */
Note::Note(QWidget *parent) :
    QMainWindow(parent)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    setWindowIcon(QIcon(":/images/note.png"));
    setWindowTitle(tr("���±�"));

    QPalette pal;
    QPixmap pixmap(":/images/background.jpg");
    pal.setBrush(QPalette::Window,QBrush(pixmap));
    setPalette(pal);


    textEdit = new QTextEdit();
    setCentralWidget(textEdit);
    createAction();//��������
    createMenu();//�����˵�
    createToolBar();//����������

    //�ļ��˵��Ķ�����۵�����
    connect(newFile,SIGNAL(triggered()),this,SLOT(NewFile()));
    connect(openFile,SIGNAL(triggered()),this,SLOT(OpenFile()));
    connect(save,SIGNAL(triggered()),this,SLOT(Save()));
    connect(saveAs,SIGNAL(triggered()),this,SLOT(SaveAs()));
    connect(exit,SIGNAL(triggered()),this,SLOT(close()));
    //�༭�˵��Ķ�����۵�����
    connect(cut,SIGNAL(triggered()),textEdit,SLOT(cut()));
    connect(copy,SIGNAL(triggered()),textEdit,SLOT(copy()));
    connect(paste,SIGNAL(triggered()),textEdit,SLOT(paste()));
    connect(undoAction,SIGNAL(triggered()),textEdit,SLOT(undo()));
    connect(redoAction,SIGNAL(triggered()),textEdit,SLOT(redo()));
    //�������ö�����۵�����
    connect(fontCom,SIGNAL(activated(QString)),this,
            SLOT(FontComBox(QString)));
    //�ֺ����ö�����۵�����
    connect(fontSizeCom,SIGNAL(activated(QString)),this,SLOT(FontSizeCom(QString)));

    connect(textEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat&)),
            this,SLOT(showCurrentCharFormatChanged(QTextCharFormat&)));


    //��ɫ�༭������۵�����
    connect(color,SIGNAL(triggered()),this,SLOT(Color()));
    //��ʽ�˵�������۵�����
    connect(bold,SIGNAL(triggered()),this,SLOT(Bold()));
    connect(italic,SIGNAL(triggered()),this,SLOT(Italic()));
    connect(underLine,SIGNAL(triggered()),this,SLOT(UnderLine()));

    connect(group,SIGNAL(triggered(QAction*)),this,SLOT(ALignment(QAction *)));
    //����˵�������۵�����
    connect(insertImg,SIGNAL(triggered()),this,SLOT(InsertImage()));
    connect(insertTable,SIGNAL(triggered()),this,SLOT(InsertTable()));
    //��ӡ������۵�����
    connect(printText,SIGNAL(triggered()),this,SLOT(PrintText()));
     //���Ҳ˵�������۵�����
    connect(find,SIGNAL(triggered()),this,SLOT(Find()));

    connect(about,SIGNAL(triggered()),this,SLOT(About()));
    connect(aboutQt,SIGNAL(triggered()),this,SLOT(AboutQt()));
    resize(600,600);
}


void Note::createAction(){
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    //�½��ļ�
    newFile = new QAction(QIcon(":/images/noteicon/new.png"),tr("�½��ļ�"),this);
    newFile->setShortcut(tr("Ctrl+N"));
    newFile->setStatusTip(tr("�½�һ���ļ�"));
    //���ļ�
    openFile = new QAction(QIcon(":/images/noteicon/open.png"),tr("���ļ�"),this);
    openFile->setShortcut(tr("Ctrl+O"));
    openFile->setStatusTip(tr("��һ���ļ�"));
    //�����ļ�
    save = new QAction(QIcon(":/images/noteicon/save.png"),tr("����"),this);
    save->setShortcut(tr("Ctrl+S"));
    save->setStatusTip(tr("�����ļ�"));
    //���Ϊ
    saveAs = new QAction(QIcon(":/images/noteicon/saveAs.png"),tr("���Ϊ"),this);
    saveAs->setShortcut(tr("Ctrl+A"));
    saveAs->setStatusTip(tr("�ļ����Ϊ"));
    //��ӡ
    printText = new QAction(QIcon(":/images/noteicon/printText.png"),tr("��ӡ"),this);
    printText->setShortcut(tr("Ctrl+P"));
    printText->setStatusTip(tr("��ӡ"));
   //�˳�
    exit = new QAction(QIcon(":/images/noteicon/exit.png"),tr("�˳�"),this);
    exit->setShortcut(tr("Ctrl+E"));
    exit->setStatusTip(tr("�˳�"));
    //�ָ�
    redoAction = new QAction(QIcon(":/images/noteicon/redo.png"),tr("�ָ�"),this);
    redoAction->setShortcut(tr("Ctrl+R"));
    redoAction->setStatusTip(tr("�ָ�"));
    //����
    undoAction = new QAction(QIcon(":/images/noteicon/undo.png"),tr("����"),this);
    undoAction->setShortcut(tr("Ctrl+U"));
    undoAction->setStatusTip(tr("����"));
    //����
    cut = new QAction(QIcon(":/images/noteicon/cut.png"),tr("����"),this);
    cut->setShortcut(tr("Ctrl+X"));
    cut->setStatusTip(tr("����"));
    //����
    copy = new QAction(QIcon(":/images/noteicon/copy.png"),tr("����"),this);
    copy->setShortcut(tr("Ctrl+C"));
    copy->setStatusTip(tr("����"));
   //ճ��
    paste = new QAction(QIcon(":/images/noteicon/paste.png"),tr("���"),this);
    paste->setShortcut(tr("Ctrl+P"));
    paste->setStatusTip(tr("ճ��"));

     //����
    bold = new QAction(QIcon(":/images/noteicon/bold.png"),tr("����"),this);
    bold->setStatusTip(tr("������"));
    bold->setCheckable(true);
    //б��
    italic = new QAction(QIcon(":/images/noteicon/italic.png"),tr("б��"),this);
    italic->setStatusTip(tr("б����"));
    italic->setCheckable(true);
    //�»���
    underLine = new QAction(QIcon(":/images/noteicon/underline.png"),tr("�»���"),this);
    underLine->setStatusTip(tr("�»���"));
    underLine->setCheckable(true);
    //��ɫ
    color = new QAction(QIcon(":/images/noteicon/color.png"),tr("��ɫ"),this);
    color->setShortcut(tr("Ctrl+C"));
    color->setStatusTip(tr("��ɫ�༭"));

    group = new QActionGroup(this);
    //�����
    left = new QAction(QIcon(":/images/noteicon/left.png"),tr("�����"),group);
    left->setStatusTip(tr("�����"));
    left->setCheckable(true);
    //����
    center = new QAction(QIcon(":/images/noteicon/center.png"),tr("���ж���"),group);
    center->setStatusTip(tr("���ж���"));
    center->setCheckable(true);
    //�Ҷ���
    right = new QAction(QIcon(":/images/noteicon/right.png"),tr("�Ҷ���"),group);
    right->setStatusTip(tr("�Ҷ���"));
    right->setCheckable(true);
    //���˶���
    justify = new QAction(QIcon(":/images/noteicon/justify.png"),tr("���˶���"),group);
    justify->setStatusTip(tr("���˶���"));
    justify->setCheckable(true);

    group->addAction(left);
    group->addAction(center);
    group->addAction(right);
    group->addAction(justify);
    //����ͼƬ
    insertImg = new QAction(QIcon(":/images/noteicon/image.png"),tr("����ͼƬ"),this);
    insertImg->setStatusTip(tr("����ͼƬ"));
    //������
    insertTable = new QAction(QIcon(":/images/noteicon/table.png"),tr("������"),this);
    insertTable->setStatusTip(tr("������"));
    //����
    find = new QAction(QIcon(":/images/noteicon/find.png"),tr("����"),this);
    find->setShortcut(tr("Ctrl+F"));
    find->setStatusTip(tr("����"));
    //����
    about = new QAction(QIcon(":/images/noteicon/about.png"),tr("����"),this);
    about->setStatusTip(tr("����"));

    //����
    fontLabel = new QLabel(tr("����"));
    fontCom = new QFontComboBox();
    fontCom->setFontFilters(QFontComboBox::ScalableFonts);
    fontCom->setStatusTip(tr("��������"));
    //�ֺ�
    fontSizeLabel = new QLabel(tr("�ֺ�"));
    QFontDatabase db;
    fontSizeCom = new QComboBox();
    foreach(int size,db.standardSizes())
        fontSizeCom->addItem(QString::number(size));
    fontSizeCom->setStatusTip(tr("�ֺ�����"));
    //����
    aboutQt = new QAction(tr("����QT"),this);
    aboutQt->setStatusTip(tr("����Qt"));

}

//�����˵�
void Note::createMenu(){
    //�ļ��˵�
    file = menuBar()->addMenu(tr("�ļ�(&F)"));
    file->addAction(newFile);
    file->addAction(openFile);
    file->addSeparator();
    file->addAction(save);
    file->addAction(saveAs);
    file->addSeparator();
    file->addAction(printText);
    file->addSeparator();
    file->addAction(exit);
    //�༭�˵�
    edit = menuBar()->addMenu(tr("�༭(&E)"));
    edit->addAction(undoAction);
    edit->addAction(redoAction);
    edit->addSeparator();
    edit->addAction(cut);
    edit->addAction(copy);
    edit->addAction(paste);
    //����˵�
    insert = menuBar()->addMenu(tr("����(&I)"));
    insert->addAction(insertImg);
    insert->addAction(insertTable);
    //���Ҳ˵�
    search = menuBar()->addMenu(tr("����(&F)"));
    search->addAction(find);
    //��ʽ�˵�

    style = menuBar()->addMenu(tr("��ʽ(&S)"));
    style->addAction(bold);
    style->addAction(italic);
    style->addAction(underLine);
    style->addSeparator();
    style->addAction(color);
    style->addSeparator();
    style->addAction(left);
    style->addAction(center);
    style->addAction(right);
    style->addAction(justify);
    //�����˵�
    help = menuBar()->addMenu(tr("����(&H)"));
    help->addAction(about);
    help->addAction(aboutQt);

}
//����������
void Note::createToolBar(){
    //�ļ�
    fileToolBar = addToolBar(tr("�ļ�(&N)"));
    fileToolBar->addAction(newFile);
    fileToolBar->addAction(openFile);
    fileToolBar->addSeparator();

    fileToolBar->addAction(save);
    fileToolBar->addAction(saveAs);
    fileToolBar->addSeparator();

    fileToolBar->addAction(printText);
    fileToolBar->addSeparator();
    //�༭
    editToolBar = addToolBar(tr("�༭(&E)"));
    editToolBar->addAction(undoAction);
    editToolBar->addAction(redoAction);
    editToolBar->addSeparator();
    editToolBar->addAction(cut);
    editToolBar->addAction(copy);
    editToolBar->addAction(paste);
    //��ʽ
    styleToolBar = addToolBar(tr("��ʽ(&S)"));
    styleToolBar->addAction(bold);
    styleToolBar->addAction(italic);
    styleToolBar->addAction(underLine);
    styleToolBar->addSeparator();

    styleToolBar->addAction(left);
    styleToolBar->addAction(center);
    styleToolBar->addAction(right);
    styleToolBar->addAction(justify);
    styleToolBar->addSeparator();

    styleToolBar->addWidget(fontLabel);
    styleToolBar->addWidget(fontCom);
    styleToolBar->addWidget(fontSizeLabel);
    styleToolBar->addWidget(fontSizeCom);
    //����
    insertToolBar = addToolBar(tr("����(&I)"));
    insertToolBar->addAction(insertImg);
    insertToolBar->addAction(insertTable);
    insertToolBar->addSeparator();
    //����
    searchToolBar = addToolBar(tr("����(&F)"));
    searchToolBar->addAction(find);
    search->addSeparator();
}



//��װ�Ķ��ļ�,���ھ������
void Note::fileRead(){
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly|QFile::Text)){
         QMessageBox::warning(this,tr("���ĵ��༭��"),
                tr("�����Դ��ļ�%1:\n%2").arg(fileName).arg(file.errorString()));
         return;
    }
        QTextStream in(&file);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        textEdit->setPlainText(in.readAll());
        QApplication::restoreOverrideCursor();
}
//��װ��д�ļ�,���ھ������
void Note::fileWrite(){
    QFile file(fileName);

    if(!file.open(QFile::WriteOnly|QFile::Text)){
        QMessageBox::warning(this,tr("���ĵ��༭��"),
                             tr("������д���ļ�%1:/n%2").arg(fileName).arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out<<textEdit->toPlainText();
    QApplication::restoreOverrideCursor();
    return;
}
//�½��ļ�
void Note::NewFile(){
    Note *n = new Note;
    n->show();
}
//���ļ�
void Note::OpenFile(){
    fileName = QFileDialog::getOpenFileName(this,tr("���ļ�"),"/","allFile(*.*)");
    if(!fileName.isEmpty()){

        if(textEdit->document()->isEmpty()){
            fileRead();
        }else{
            Note *n = new Note;
            n->show();
            QFile file(fileName);
            if(!file.open(QFile::ReadOnly|QFile::Text)){
                 QMessageBox::warning(this,tr("���ĵ��༭��"),
                        tr("�����Դ��ļ�%1:\n%2").arg(fileName).arg(file.errorString()));
                 return;
            }
                QTextStream in(&file);
                QApplication::setOverrideCursor(Qt::WaitCursor);
                n->textEdit->setPlainText(in.readAll());
                QApplication::restoreOverrideCursor();


        }

    }
}
//�����ļ�
void Note::Save(){

    if(fileName.isEmpty()){
       fileName = QFileDialog::getSaveFileName(this,tr("���Ϊ"),tr("δ����.txt"));
       fileWrite();
    }else{
        fileWrite();
    }

}
//���Ϊ
void Note::SaveAs(){

    fileName = QFileDialog::getSaveFileName(this,tr("���Ϊ"),tr("δ����.txt"));
    fileWrite();
}








//��������
void Note::FontComBox(QString fontComStr){
    QTextCharFormat format;
    format.setFontFamily(fontComStr);
    mergeFormat(format);
}
void Note::mergeFormat(QTextCharFormat format){
    QTextCursor cursor = textEdit->textCursor();
    if(!cursor.hasSelection())
       cursor.select(QTextCursor::WordUnderCursor);
       cursor.mergeCharFormat(format);
       textEdit->mergeCurrentCharFormat(format);

}
//�ֺ�����
void Note::FontSizeCom(QString fontSizeStr){
    QTextCharFormat format;
    format.setFontPointSize(fontSizeStr.toFloat());
    textEdit->mergeCurrentCharFormat(format);
}





//��ɫ����
void Note::Color(){
    QColor color = QColorDialog::getColor(Qt::blue,this);
    if(color.isValid()){
        QTextCharFormat format;
        format.setForeground(color);
        textEdit->mergeCurrentCharFormat(format);
    }
}
//����
void Note::Bold(){
    QTextCharFormat format;
    format.setFontWeight(bold->isChecked()?QFont::Bold:QFont::Normal);
    textEdit->mergeCurrentCharFormat(format);
}
//б��
void Note::Italic(){
    QTextCharFormat format;
    format.setFontItalic(italic->isChecked());
    textEdit->mergeCurrentCharFormat(format);

}
//�»���
void Note::UnderLine(){

    QTextCharFormat format;
    format.setFontUnderline(underLine->isChecked());
    textEdit->mergeCurrentCharFormat(format);
}
//����
void Note::ALignment(QAction *act){
    if(act == left){
        textEdit->setAlignment(Qt::AlignLeft);
    }else if(act == center){
        textEdit->setAlignment(Qt::AlignCenter);
    }else if(act == right){
        textEdit->setAlignment(Qt::AlignRight);
    }else if(act == justify){
        textEdit->setAlignment(Qt::AlignJustify);
    }

}





//����ͼƬ
void Note::InsertImage(){

    QTextCursor cursor = textEdit->textCursor();
    QTextImageFormat format;
    QString image = QFileDialog::getOpenFileName(this,"insert image","/","png(*.png)");
    format.setName(image);
    cursor.insertImage(format);

}

//������,�õ�����Ի���
void Note::InsertTable(){
    bool ok;
    int a = QInputDialog::getInt(this,tr("������"),tr("��������:"),0,0,300,1,&ok);
    int b = QInputDialog::getInt(this,tr("������"),tr("��������:"),0,0,300,1,&ok);
    if(ok){

        QTextCursor cursor = textEdit->textCursor();

        QTextTableFormat format;
        format.setCellSpacing(0);
        format.setCellPadding(30);

        cursor.insertTable(a,b,format);

    }
}


//����
void Note::Find(){
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("����"));
    lineEdit = new QLineEdit(dialog);
    label = new QLabel(this);
    btn = new QPushButton(dialog);
    btn->setText(tr("��һ��"));
    connect(btn,SIGNAL(clicked()),this,SLOT(findNext()));

    layout = new QVBoxLayout();
    layout->addWidget(lineEdit);
    layout->addWidget(btn);
    dialog->setLayout(layout);
    dialog->resize(60,60);
    dialog->show();

}

void Note::findNext(){

    QString str = lineEdit->text();

    bool isfind = textEdit->find(str,QTextDocument::FindBackward);

    if(isfind){
        label->setText(tr("����Ŀ����,�к�:%1 �к�:%2!").arg(textEdit->textCursor().blockNumber()).arg(textEdit->textCursor().columnNumber()));
        //statusBar()->addPermanentWidget(label);

    }

}
//��ӡ
void Note::PrintText(){
    QPrinter printer;
    QPrintDialog printDialog(&printer,this);
    if(printDialog.exec()){
        QTextDocument *textDocument = textEdit->document();
        textDocument->print(&printer);
    }
}




//����
void Note::About(){
    QMessageBox::about(this,tr("����"),tr("����һ�����±�"));

}

void Note::AboutQt(){
    QMessageBox::aboutQt(this,tr("Qt�༭��"));
}

void Note::showCurrentCharFormatChanged(const QTextCharFormat &form){

    fontCom->setCurrentIndex(fontCom->findText(form.fontFamily()));
    fontSizeCom->setCurrentIndex(fontSizeCom->findText(QString::number(form.fontPointSize())));
    bold->setChecked(form.font().bold());
    italic->setChecked(form.font().italic());
    underLine->setChecked(form.font().underline());
}

