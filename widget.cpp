#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QPainter>
#include <QCursor>
#include <QMediaPlayer>
#include <QSound>
#include <QMediaPlaylist>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("FightMouse");
    QMediaPlaylist *playlist = new QMediaPlaylist;

    playlist->addMedia(QUrl("qrc:/music/music/back.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    QMediaPlayer * player = new QMediaPlayer;
    player->setPlaylist(playlist);

    player->setVolume(50);
    player->play();


    ui->page_2->setMouseTracking(true);
    ui->stackedWidget->setMouseTracking(true);
    setMouseTracking(true);

    m_score =0;
    m_isStart =false;
    m_isDrawHammer = false;

    //
    connect(&m_timerClearn,&QTimer::timeout,
            [=](){
        m_isDrawHammer = false;
        m_timerClearn.stop();
        update();
    });

    m_timerShowMouse.start(1000);
    connect(&m_timerShowMouse,&QTimer::timeout,
            [=](){

        //老鼠的下标
        m_mouseX = rand()%4;
        m_mouseY = rand()%4;
        update();

    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)
{

    m_StartX = width()/10;
    m_StartY = height()/10;

    m_MapW =width()/5;
    m_MapH = height()/5;


    QPainter painter(this);

    if(m_isStart)
    {

         painter.drawPixmap(rect(),QPixmap(":/img/image/350.png"));
        //划线 - 绘制图片
        for(int i =0;i<4;i++)
        {
            for(int j =0;j<4;j++)
                {
                    painter.drawPixmap(
                            m_StartX +  j*m_MapW,
                            m_StartY +  i*m_MapH,
                            m_MapW,
                            m_MapH,
                             QPixmap(":/img/image/bk.png")
                            );
            }

        }

        // 绘制老鼠
        painter.drawPixmap(
                    m_StartX+m_MapW*m_mouseX,
                    m_StartY+m_MapH*m_mouseY,
                    m_MapW,
                    m_MapH,
                    QPixmap(":/img/image/shu.png")
                    );
        // 绘制锤子
        if(m_isDrawHammer)
        {
            // 打到老鼠
            if(m_hammerX == m_mouseX && m_hammerY==m_mouseY)
            {
                QSound::play(":/music/music/ping.wav");
                m_score +=100;
                painter.drawPixmap(
                            m_hammerX*m_MapW+m_StartX,
                            m_hammerY*m_MapH+m_StartY,
                            m_MapW,
                            m_MapH,
                            QPixmap(":/img/image/shu2.png"));

             // 没有达到老鼠正常画锤子
            }else{
                painter.drawPixmap(
                            m_hammerX*m_MapW+m_StartX,
                            m_hammerY*m_MapH+m_StartY,
                            m_MapW,
                            m_MapH,
                            QPixmap(":/img/image/5.png"));
            }

        }

        // 绘制分数
        QString sql = QString("当前分数 %1").arg(m_score);
        QFont font("微软雅黑",20,QFont::Bold,false);
        painter.setFont(font);
        painter.drawText(width()/2-sql.size()*10,30,sql);


    }else
    {
        painter.drawPixmap(rect(),QPixmap(":/img/image/bgk.jpg"));
    }

    painter.end();
}

void Widget::on_pushButton_clicked()
{
    // 开始游戏，切换到第二张界面
    ui->stackedWidget->setCurrentIndex(1);
    m_isStart = true;
}


void Widget::mousePressEvent(QMouseEvent *e)
{
    //鼠标按下
    // 判断落在什么地方
    // 鼠标点击时获取坐标
    int x = e->x();
    int y = e->y();
    // 落在棋盘上
    if(x > m_StartX && x<m_StartX+m_MapW*4  &&
        y>m_StartY && y<m_StartY+m_MapH*4)
    {
            // 转换成数组下标
            m_hammerX = (x-m_StartX)/m_MapW;
            m_hammerY = (y-m_StartY)/m_MapH;
            // 绘制锤子
             m_isDrawHammer = true;
            // 更新窗口，重绘
            update();
            qDebug()<< m_hammerX <<" "<<m_hammerY;
            // 5秒后删除锤子
            m_timerClearn.start(500);

    }


}

void Widget::mouseMoveEvent(QMouseEvent *e)
{

    int x = e->x();
    int y = e->y();
    // 落在棋盘上
    if(x > m_StartX && x<m_StartX+m_MapW*4  &&
        y>m_StartY && y<m_StartY+m_MapH*4)
    {

         QCursor my(QPixmap(":/img/image/5.png"));
         QApplication::setOverrideCursor(my);//用成员函数设置图标
    }

    else{
         QApplication::restoreOverrideCursor();//删除载入的图

    }

}
