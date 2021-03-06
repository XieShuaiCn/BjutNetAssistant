#ifndef BNA_GUI_HLABEL_H
#define BNA_GUI_HLABEL_H

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

private:
    Q_DISABLE_COPY(HLabel)
};
}
}
#endif // BNA_GUI_HLABEL_H
