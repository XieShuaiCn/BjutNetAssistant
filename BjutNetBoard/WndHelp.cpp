#include "WndHelp.h"

namespace bna{
namespace gui{
WndHelp::WndHelp(QWidget *parent) : QWidget(parent)
{
    m_lblImage = new HLabel(this);
    m_lblImage->setPixmap(QPixmap(":/png/HelpInfo"));
    m_lblImage->setGeometry(0, 0, 575, 495);
    connect(m_lblImage, &bna::gui::HLabel::clicked, this, &bna::gui::WndHelp::on_lblImage_clicked);
}

void WndHelp::on_lblImage_clicked()
{
    this->close();
}

}}
