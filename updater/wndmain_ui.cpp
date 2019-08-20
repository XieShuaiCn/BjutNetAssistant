#include "WndMain.h"
#include "FrmWelcome.h"
#include "FrmUpdate.h"
#include "FrmComplete.h"
#include <QtWidgets/QFrame>
#include <QtWidgets/QPushButton>

void WndMain::initUI()
{
    QFont font_s100;
    font_s100.setPointSize(10);
    QFont font_s105;
    font_s105.setPointSizeF(10.5);
    QFont font_s115;
    font_s115.setPointSizeF(11.5);
    QFont font_s120;
    font_s120.setPointSize(12);
    QFont font_s130;
    font_s130.setPointSize(13);

    this->setWindowTitle("网关登录器 更新");
    this->setObjectName(QStringLiteral("WndMain"));
    this->setWindowIcon(QIcon(":/icon/logo.ico"));
    this->setFixedSize(500, 150);
    this->setFont(font_s105);

    m_frmShowStep = new QFrame(this);
    m_frmShowStep->setGeometry(QRect(10, 10, this->width()-20, this->height()-70));
    m_frmShowStep->setFrameShape(QFrame::NoFrame);
    m_frmShowStep->setFrameShadow(QFrame::Plain);
    m_vecFrameStep.emplace_back(new FrmWelcome(m_frmShowStep));
    m_vecFrameStep.emplace_back(new FrmUpdate(m_frmShowStep));
    m_vecFrameStep.emplace_back(new FrmComplete(m_frmShowStep));
    m_vecFrameStep[0]->setVisible(true);
    m_vecFrameStep[0]->setGeometry(0, 0, m_frmShowStep->width(), m_frmShowStep->height());
    m_nCurrentStep = 0;

    // button group
    m_frmButton = new QFrame(this);
    m_frmButton->setGeometry(QRect(10, this->height()-55, this->width()-20, 55));
    m_frmButton->setFrameShape(QFrame::NoFrame);
    m_frmButton->setFrameShadow(QFrame::Plain);
    m_btnLast = new QPushButton("<< Last", m_frmButton);
    m_btnLast->setGeometry(m_frmButton->width()-270, 10, 80, 25);
    m_btnLast->setFont(font_s105);
    m_btnLast->setEnabled(false);
    m_btnNext = new QPushButton("Next >>", m_frmButton);
    m_btnNext->setGeometry(m_frmButton->width()-180, 10, 80, 25);
    m_btnNext->setFont(font_s105);
    m_btnNext->setEnabled(false);
    m_btnCancel = new QPushButton("Cancel", m_frmButton);
    m_btnCancel->setGeometry(m_frmButton->width()-90, 10, 80, 25);
    m_btnCancel->setFont(font_s105);
    m_btnCancel->setEnabled(false);
}
