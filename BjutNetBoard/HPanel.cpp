#include "HPanel.h"
#include <QPainter>
#include <QFrame>
#include <QPaintEvent>
#include <QPen>

namespace bna {
namespace gui {

HPanel::HPanel(QWidget* parent, Qt::WindowFlags f) :
    QFrame(parent, f),
    m_bShowText(false),
    m_bShowBorder(true),
    m_dBorderWidth(2.),
    m_dBorderRound(5.),
    m_colorBorder(QColor::fromRgb(180,180,180))
{
    m_lblText = new QLabel(this);
    m_lblText->setGeometry(QRect(25, 0, 100, 20));
}

void HPanel::setText(const QString &text)
{
    if(text.size())
    {
        const auto fm = m_lblText->fontMetrics();
        m_rectText = fm.boundingRect(text);
        m_lblText->setGeometry(QRect(25, 0, m_rectText.width()+10, 20));
        m_bShowText = true;
    }
    else {
        m_bShowText = false;
    }
    m_lblText->setText(text);
}

void HPanel::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);
    if(m_bShowBorder){
        QPainter painter(this);
        // 设置反锯齿
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(Qt::NoBrush);
        {
            QPen pen(m_colorBorder);
            pen.setWidthF(m_dBorderWidth);
            painter.setPen(pen);
        }
        if(m_bShowText){
            double width = this->width();
            double height = this->height();
            double t_height_2 = 3+0.5*m_rectText.height();
            double t_width = m_rectText.width()+2;
            double left = 0.5*m_dBorderWidth;
            double right = width-0.5*m_dBorderWidth;
            double top = t_height_2;
            double bottom = height-0.5*m_dBorderWidth;

            painter.drawArc(QRectF(right-m_dBorderRound*2., t_height_2, m_dBorderRound*2., m_dBorderRound*2.), 0.*16., 90.*16.);
            painter.drawArc(QRectF(left, top, m_dBorderRound*2., m_dBorderRound*2.), 90.*16., 90.*16.);
            painter.drawArc(QRectF(left, bottom-m_dBorderRound*2., m_dBorderRound*2., m_dBorderRound*2), 180.*16., 90.*16.);
            painter.drawArc(QRectF(right-m_dBorderRound*2., bottom-m_dBorderRound*2., m_dBorderRound*2., m_dBorderRound*2), 270.*16., 90.*16.);

            painter.drawLine(QPointF(left+m_dBorderRound, top), QPointF(20., top));
            painter.drawLine(QPointF(30.+t_width, top), QPointF(right-m_dBorderRound, top));
            painter.drawLine(QPointF(right, top+m_dBorderRound), QPointF(right, bottom-m_dBorderRound));
            painter.drawLine(QPointF(left+m_dBorderRound, bottom), QPointF(right-m_dBorderRound, bottom));
            painter.drawLine(QPointF(left, top+m_dBorderRound), QPointF(left, bottom-m_dBorderRound));
        }
        else {
            double left = 0.5*m_dBorderWidth;
            painter.drawRoundedRect(QRectF(left, left, this->width()-m_dBorderWidth, this->height()-m_dBorderWidth), m_dBorderRound, m_dBorderRound);
        }
    }
}
}}
