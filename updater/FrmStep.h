#ifndef FRMSTEP_H
#define FRMSTEP_H

#include <QFrame>

class FrmStep : public QFrame
{
public:
    FrmStep(QWidget* parent = Q_NULLPTR);
    virtual bool canGoLast();
    virtual bool canGoNext();
    virtual bool canCancel();

    virtual bool canProcess();

    virtual bool doBefore();
    virtual bool doProcess();
    virtual bool doAfter();
    virtual bool doRollBack();
};

#endif // FRMSTEP_H
