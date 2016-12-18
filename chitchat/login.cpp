#include "login.h"
#include "ui_login.h"
/**
 * @brief Login::Login
 * @param parent
 */
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    setWindowTitle(tr("�׸������������"));
    /**
     *���ñ���
     * @brief pal
     */
    QPalette pal;
    QPixmap pixmap(":/images/background.jpg");
    pal.setBrush(QPalette::Window,QBrush(pixmap));
    setPalette(pal);

}

Login::~Login()
{
    delete ui;
}
/**
 *��¼����֤
 * @brief Login::on_loginButton_clicked
 */
void Login::on_loginButton_clicked()
{
    if(ui->lineEdit->text()==""){
        QMessageBox::warning(this,tr("��ܰ��ʾ"),tr("�������û��ǳ�"),QMessageBox::Ok,QMessageBox::No);
    }else{
        Chitchat *chi=new Chitchat(ui->lineEdit->text());
        chi->show();
        this->close();
    }
}

void Login::on_exitButton_clicked()
{
    this->close();
}


