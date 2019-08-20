#include "FrmWelcome.h"
#include <QLabel>

FrmWelcome::FrmWelcome(QWidget* parent)
    : FrmStep(parent)
{
    m_lblInfo = new QLabel(this);
    m_lblInfo->setGeometry(30, 20, this->width()-40, 50);
    m_lblInfo->setText("Welcome to update <BjutNetLogin>!\r\nPress <Next> to continue.");
}

