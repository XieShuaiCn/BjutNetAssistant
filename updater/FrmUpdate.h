#ifndef FRMUPDATE_H
#define FRMUPDATE_H

#include "FrmStep.h"

class QLabel;
class QProgressBar;

class FrmUpdate : public FrmStep
{
public:
    FrmUpdate(QWidget* parent = Q_NULLPTR);

    virtual bool canCancel();

    virtual bool canGoLast();

    virtual bool canGoNext();

    virtual bool doProcess();

private:
    QLabel *m_lblInfo;
    QProgressBar * m_barUpdate;
    bool m_bCanCancel;
    bool m_bCanNext;
    bool m_bCanLast;
    bool m_bCanProcess;
};

#endif // FRMUPDATE_H
