#include "FrmComplete.h"
#include <QLabel>

FrmComplete::FrmComplete(QWidget* parent)
    : FrmStep(parent)
{
    m_lblInfo = new QLabel(this);
    m_lblInfo->setGeometry(30, 20, this->width()-40, 50);
    m_lblInfo->setText("Update completed!\r\nEnjoy yourself!");
}

bool FrmComplete::canCancel()
{
    return true;
}

bool FrmComplete::canGoLast()
{
    return false;
}

bool FrmComplete::canGoNext()
{
    return false;
}
