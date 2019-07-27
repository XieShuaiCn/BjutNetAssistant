#ifndef HLABEL_H
#define HLABEL_H

#include <QtWidgets/QLabel>
class QMouseEvent;

namespace bna{
namespace gui{

class HLabel : public QLabel
{
    Q_OBJECT

public:
    explicit HLabel(QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags());
    explicit HLabel(const QString &text, QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags());

signals:
    void clicked(Qt::MouseButton button = Qt::MouseButton::NoButton);
    void doubleClicked(Qt::MouseButton button = Qt::MouseButton::NoButton);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
};
}
}
#endif // HLABEL_H
