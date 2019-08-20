#include "FrmStep.h"

FrmStep::FrmStep(QWidget* parent)
    : QFrame(parent)
{
    if(parent)
    {
        this->setGeometry(parent->geometry());
    }
    this->setVisible(false);
}

bool FrmStep::canGoLast()
{
    return true;
}

bool FrmStep::canGoNext()
{
    return true;
}

bool FrmStep::canCancel()
{
    return true;
}
bool FrmStep::canProcess()
{
    return true;
}

bool FrmStep::doBefore()
{
    return true;
}

bool FrmStep::doProcess()
{
    return true;
}

bool FrmStep::doAfter()
{
    return true;
}

bool FrmStep::doRollBack()
{
    return true;
}
