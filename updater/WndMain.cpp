#include "WndMain.h"
#include <QtGui/QCloseEvent>
#include <QtWidgets/QFrame>
#include <QtWidgets/QPushButton>

WndMain::WndMain(QWidget *parent)
    : QWidget(parent)
{
    initUI();

    m_btnLast->setEnabled(false);
    m_btnNext->setEnabled(true);
    m_btnCancel->setEnabled(true);

    connect(m_btnNext, &QPushButton::clicked, this, &WndMain::btnNextClicked);
    connect(m_btnLast, &QPushButton::clicked, this, &WndMain::btnLastClicked);
    connect(m_btnCancel, &QPushButton::clicked, this, &WndMain::btnCancelClicked);
    connect(&m_tmMoveFrame, &QTimer::timeout, this, &WndMain::MoveFrame);
}

WndMain::~WndMain()
{

}

void WndMain::doSwitchFrame()
{
    if (m_nDirection > 0)
    {
        if(m_nCurrentStep >= 0 && m_nCurrentStep < int(m_vecFrameStep.size())-1)
        {
            m_vecFrameStep[m_nCurrentStep+1]->setGeometry(m_frmShowStep->width(), 0, m_frmShowStep->width(), m_frmShowStep->height());
            m_vecFrameStep[m_nCurrentStep+1]->setVisible(true);
            m_tmMoveFrame.setSingleShot(false);
            m_tmMoveFrame.start(1);
        }
        else if(m_nCurrentStep == int(m_vecFrameStep.size())-1)
        {
            m_vecFrameStep[m_nCurrentStep]->doAfter();
            this->close();
        }
    }
    else if (m_nDirection < 0)
    {
        if(m_nCurrentStep > 0 && m_nCurrentStep < int(m_vecFrameStep.size()))
        {
            m_vecFrameStep[m_nCurrentStep-1]->setGeometry(-m_frmShowStep->width(), 0, m_frmShowStep->width(), m_frmShowStep->height());
            m_vecFrameStep[m_nCurrentStep-1]->setVisible(true);
            m_tmMoveFrame.setSingleShot(false);
            m_tmMoveFrame.start(1);
        }
    }
}

void WndMain::finishSwitchFrame()
{
    if(m_nDirection > 0)
    {
        m_tmMoveFrame.stop();
        ++m_nCurrentStep;
        m_btnLast->setEnabled(false);
        m_btnNext->setEnabled(false);
        m_btnCancel->setEnabled(false);
        if(m_nCurrentStep == int(m_vecFrameStep.size())-1){
            m_btnCancel->setText("Finish");
        }
        else {
            m_btnCancel->setText("Cancel");
        }
        m_vecFrameStep[m_nCurrentStep]->doBefore();
        m_vecFrameStep[m_nCurrentStep]->doProcess();
        m_btnLast->setEnabled(m_nCurrentStep > 0
                              && m_vecFrameStep[m_nCurrentStep]->canGoLast());
        m_btnNext->setEnabled(m_nCurrentStep < int(m_vecFrameStep.size())
                              && m_vecFrameStep[m_nCurrentStep]->canGoNext());
        m_btnCancel->setEnabled(m_vecFrameStep[m_nCurrentStep]->canCancel());
    }
    else if(m_nDirection < 0)
    {
        m_tmMoveFrame.stop();
        --m_nCurrentStep;
        m_btnLast->setEnabled(false);
        m_btnNext->setEnabled(false);
        m_btnCancel->setEnabled(false);
        m_vecFrameStep[m_nCurrentStep]->doBefore();
        m_vecFrameStep[m_nCurrentStep]->doProcess();
        m_btnLast->setEnabled(m_nCurrentStep > 0
                              && m_vecFrameStep[m_nCurrentStep]->canGoLast());
        m_btnNext->setEnabled(m_nCurrentStep < int(m_vecFrameStep.size())
                              && m_vecFrameStep[m_nCurrentStep]->canGoNext());
        m_btnCancel->setEnabled(m_vecFrameStep[m_nCurrentStep]->canCancel());
    }
}

void WndMain::MoveFrame()
{
    if(m_nDirection > 0)
    {
        QRect r1 = m_vecFrameStep[m_nCurrentStep]->geometry();
        QRect r2 = m_vecFrameStep[m_nCurrentStep+1]->geometry();
        if(r2.left() > 0){
            r1.moveLeft(r1.left() - m_nDirection);
            auto nl = r2.left() - m_nDirection;
            nl = (nl > 0 ? nl : 0);
            r2.moveLeft(nl);
            m_vecFrameStep[m_nCurrentStep]->setGeometry(r1);
            m_vecFrameStep[m_nCurrentStep+1]->setGeometry(r2);
        }
        else
        {
            finishSwitchFrame();
        }
    }
    else if(m_nDirection < 0)
    {
        QRect r1 = m_vecFrameStep[m_nCurrentStep]->geometry();
        QRect r2 = m_vecFrameStep[m_nCurrentStep-1]->geometry();
        if(r2.left() < 0){
            r1.moveLeft(r1.left() - m_nDirection);
            auto nl = r2.left() - m_nDirection;
            nl = (nl < 0 ? nl : 0);
            r2.moveLeft(nl);
            m_vecFrameStep[m_nCurrentStep]->setGeometry(r1);
            m_vecFrameStep[m_nCurrentStep-1]->setGeometry(r2);
        }
        else
        {
            finishSwitchFrame();
        }
    }
}

void WndMain::btnNextClicked()
{
    if(m_nCurrentStep >= 0 && m_nCurrentStep < int(m_vecFrameStep.size()))
    {
        if(m_vecFrameStep[m_nCurrentStep]->canGoNext()
                && m_vecFrameStep[m_nCurrentStep]->canProcess())
        {
            m_vecFrameStep[m_nCurrentStep]->doAfter();
            m_nDirection = 2;
            doSwitchFrame();
        }
    }
}

void WndMain::btnLastClicked()
{
    if(m_nCurrentStep >= 0 && m_nCurrentStep < int(m_vecFrameStep.size()))
    {
        if(m_vecFrameStep[m_nCurrentStep]->canGoLast())
        {
            m_vecFrameStep[m_nCurrentStep]->doRollBack();
            m_nDirection = -2;
            doSwitchFrame();
        }
    }
}

void WndMain::btnCancelClicked()
{
    if(m_nCurrentStep >= 0 && m_nCurrentStep < int(m_vecFrameStep.size()))
    {
        for(int i = m_nCurrentStep; i >= 0; --i)
        {
            m_vecFrameStep[i]->doRollBack();
        }
    }
    this->close();
}

void WndMain::closeEvent(QCloseEvent *event)
{
    //取消关闭命令
    if(m_vecFrameStep[m_nCurrentStep]->canCancel()){
        btnCancelClicked();
    }
    else{
        event->setAccepted(false);
    }
}

