#ifndef BNA_GUI_HPANEL_H
#define BNA_GUI_HPANEL_H

#include "../BjutNetService/common.h"
#include <QFrame>
#include <QColor>
#include <QLabel>
#include <QMutex>

class QGraphicsOpacityEffect;

namespace bna{
namespace gui{
class HPanel : public QFrame
{
    Q_OBJECT
public:
    explicit HPanel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    PROPERTY_READ_WRITE(bool, ShowBorder, m_bShowBorder)
    PROPERTY_READ_WRITE(double, BorderWidth, m_dBorderWidth)
    PROPERTY_READ_WRITE(double, BorderRound, m_dBorderRound)
    PROPERTY_READ_WRITE(QColor, BorderColor, m_colorBorder)
    void setBorderStyle(double width, double round, QColor color);
    void setText(const QString &text);
public slots:
    virtual void setVisible(bool visible);
protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    QLabel *m_lblText;
    bool m_bShowText;
    bool m_bShowBorder;
    QRect m_rectText;
    double m_dBorderWidth;
    double m_dBorderRound;
    QColor m_colorBorder;
    QGraphicsOpacityEffect *m_effectOpacity;
    int m_nOpacityEffectTimer;
    qreal m_fOpacityStart;
    qreal m_fOpacityStop;
    qreal m_fOpacityStep;
    QMutex m_mtxToVisible;
    bool m_bToVisible;
    bool m_bToInvisible;
};

inline void HPanel::setBorderStyle(double width, double round, QColor color)
{
    m_dBorderWidth = width;
    m_dBorderRound = round;
    m_colorBorder = color;
}
}}
#endif // BNA_GUI_HPANEL_H
