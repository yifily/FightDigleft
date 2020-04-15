#ifndef WIDGET_H
#define WIDGET_H

#include <QPaintEvent>
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void paintEvent(QPaintEvent *);
    // 鼠标点击事件
    void mousePressEvent(QMouseEvent *e);
    // 鼠标移动事件
    void mouseMoveEvent(QMouseEvent *e);


    int  m_score;
    bool m_isStart;

    // 开始位置
    int m_StartX;
    int m_StartY;

    // 图片宽度
    int m_MapW ;
    int m_MapH ;

    // 锤子位置
    int m_hammerX;
    int m_hammerY;
    // 是否画锤子
    bool m_isDrawHammer;
    // 用定时器定时画锤子
    QTimer m_timerClearn;

    // 定时出现老鼠
    QTimer m_timerShowMouse;
    // 老鼠出现的位置
    int m_mouseX;
    int m_mouseY;

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
