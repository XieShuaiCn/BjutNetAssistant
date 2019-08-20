#ifndef FRMWELCOME_H
#define FRMWELCOME_H

#include "FrmStep.h"

class QLabel;

class FrmWelcome : public FrmStep
{
public:
    FrmWelcome(QWidget* parent = Q_NULLPTR);

private:
    QLabel *m_lblInfo;
};

#endif // FRMWELCOME_H
