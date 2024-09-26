#include "Client.h"

Client::Client(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_pProtocol = new Protocol("ws://localhost:1234", this);
    movie = new QMovie(":/Client/login_a");
    movie_b = new QMovie(":/Client/login_b");

    ui.lineEdit_pwd->installEventFilter(this);
    ui.label_link_reg->installEventFilter(this);
}

Client::~Client()
{
}

void Client::on_pushButton_clicked()
{
    m_pProtocol->test(ui.lineEdit_id->text());
}

bool Client::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui.lineEdit_pwd)
    {
        if (event->type() == QEvent::FocusIn)
        {
            // 当 lineEdit_pwd 获得焦点时启动动画
            ui.gif_a->setMovie(movie);
            ui.gif_b->setMovie(movie_b);
            movie->setScaledSize(ui.gif_a->size());
            movie_b->setScaledSize(ui.gif_b->size());
            movie->start();
            movie_b->start();
        }
        else if (event->type() == QEvent::FocusOut)
        {
            // 当 lineEdit_pwd 失去焦点时停止动画
            // 不显示label
            ui.gif_a->clear();
            ui.gif_b->clear();
            movie->stop();
            movie_b->stop();
        }
    }
    if (watched == ui.label_link_reg)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                // 当 label_link_reg 被点击时切换页面
                if (isLoginPage)
                {
                    ui.stackedWidget->setCurrentWidget(ui.page_signin);
                }
                else
                {
                    ui.stackedWidget->setCurrentWidget(ui.page_login);
                }
                isLoginPage = !isLoginPage; // 切换标记
            }
        }
    }

    return QMainWindow::eventFilter(watched, event);
}