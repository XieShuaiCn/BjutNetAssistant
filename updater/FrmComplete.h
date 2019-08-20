#ifndef FRMCOMPLETE_H
#define FRMCOMPLETE_H

#include "FrmStep.h"

class QLabel;

class FrmComplete : public FrmStep
{
public:
    FrmComplete(QWidget* parent = Q_NULLPTR);

    virtual bool canCancel();

    virtual bool canGoLast();

    virtual bool canGoNext();

private:
    QLabel *m_lblInfo;
};

#endif // FRMCOMPLETE_H
