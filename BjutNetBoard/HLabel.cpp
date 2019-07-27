#include "HLabel.h"
#include <QMouseEvent>

namespace bna {
namespace gui {

HLabel::HLabel(QWidget *parent, Qt::WindowFlags f):
    QLabel(parent, f)
{ }

HLabel::HLabel(const QString &text, QWidget *parent, Qt::WindowFlags f):
    QLabel(text, parent, f)
{ }

void HLabel::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    emit clicked(event->button());
}

void HLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    QLabel::mouseDoubleClickEvent(event);
    emit doubleClicked(event->button());
}
}}
