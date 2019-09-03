#include "WndMain.h"
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include "HLabel.h"
#include "HPanel.h"
#include "HPanel.h"


void bna::gui::WndMain::layoutUI()
{
    QFont font_s100;
    QFont font_s105;
    QFont font_s115;
    QFont font_s120;
    QFont font_s130;
    font_s100.setPointSize(10);
    font_s105.setPointSizeF(10.5);
    font_s115.setPointSizeF(11.5);
    font_s120.setPointSize(12);
    font_s130.setPointSize(13);
    //窗口
    this->setObjectName(QStringLiteral("WndMain"));
    this->setWindowIcon(QIcon(":/icon/logo.ico"));
    //设置固定窗口大小
    this->setFixedSize(m_szFrameSimple);
    //窗口控件

    //状态图
    m_frmGraph->setGeometry(QRect(10, 10, 280, 160));
    m_frmGraph->setFrameShape(QFrame::NoFrame);
    m_frmGraph->setFrameShadow(QFrame::Plain);
    m_lblService->setGeometry(QRect(0, 0, 150, 20));
    m_lblService->setFont(font_s115);
    m_lblFlowUsed->setGeometry(QRect(0, 20, 100, 20));
    m_lblFlowUsed->setFont(font_s120);
    m_lblStatus->setGeometry(QRect(0, 40, 50, 20));
    m_lblStatus->setFont(font_s120);
    m_lblStatusFlag->setGeometry(QRect(35, 40, 20, 20));
    m_lblStatusFlag->setFont(font_s120);
    m_btnRefresh->setGeometry(QRect(55, 40, 20, 20));
    m_btnRefresh->setFlat(true);
    m_btnRefresh->setFont(font_s120);
    m_btnSetting->setGeometry(QRect(0, 60, 20, 20));
    m_btnSetting->setFlat(true);
    m_btnSetting->setFont(font_s120);
    m_btnHelp->setGeometry(QRect(20, 60, 20, 20));
    m_btnHelp->setFlat(true);
    m_btnHelp->setFont(font_s120);
    m_frmFlowGraph->setGeometry(QRect(115,0,161,161));
    m_frmFlowGraph->setFrameShape(QFrame::NoFrame);
    m_frmFlowGraph->setFrameShadow(QFrame::Plain);
    m_rectFlowGraphTop = QRect(m_frmGraph->pos()+m_frmFlowGraph->pos(),
                            m_frmFlowGraph->size()-QSize(0, m_nFlowGraphThickness));
    m_rectFlowGraphBottom = QRect(m_rectFlowGraphTop.topLeft()+QPoint(0, m_nFlowGraphThickness),
                            m_rectFlowGraphTop.size());
    //账户状态
    m_frmInfo->setGeometry(QRect(310, 10, 250, 120));
    m_frmInfo->setFrameShape(QFrame::NoFrame);
    m_frmInfo->setFrameShadow(QFrame::Plain);
    //状态label
    m_lblInfoTime->setGeometry(QRect(20, 10, 100, 20));
    m_lblInfoTime->setFont(font_s130);
    m_lblInfoFlow->setGeometry(QRect(20, 50, 100, 20));
    m_lblInfoFlow->setFont(font_s130);
    m_lblInfoFee->setGeometry(QRect(20, 90, 100, 20));
    m_lblInfoFee->setFont(font_s130);
    //状态value
    m_lcdNumTime->setGeometry(QRect(110, 10, 81, 23));
    m_lcdNumTime->setFrameShadow(QFrame::Sunken);
    m_lcdNumTime->setLineWidth(1);
    m_lcdNumTime->setDigitCount(7);
    m_lcdNumTime->setSegmentStyle(QLCDNumber::Flat);
    m_lcdNumFlow->setGeometry(QRect(110, 50, 81, 23));
    m_lcdNumFlow->setFrameShadow(QFrame::Sunken);
    m_lcdNumFlow->setLineWidth(1);
    m_lcdNumFlow->setDigitCount(7);
    m_lcdNumFlow->setSegmentStyle(QLCDNumber::Flat);
    m_lcdNumFee->setGeometry(QRect(110, 90, 81, 23));
    m_lcdNumFee->setFrameShadow(QFrame::Sunken);
    m_lcdNumFee->setLineWidth(1);
    m_lcdNumFee->setDigitCount(7);
    m_lcdNumFee->setSegmentStyle(QLCDNumber::Flat);
    //状态unit
    m_lblTimeUnit->setGeometry(QRect(200, 10, 40, 20));
    m_lblTimeUnit->setFont(font_s130);
    m_lblFlowUnit->setGeometry(QRect(200, 50, 40, 20));
    m_lblFlowUnit->setFont(font_s130);
    m_lblFeeUnit->setGeometry(QRect(200, 90, 40, 20));
    m_lblFeeUnit->setFont(font_s130);
    //上线按钮
    m_btnLogin->setGeometry(QRect(310, 140, 85, 30));
    //下线按钮
    m_btnLogout->setGeometry(QRect(410, 140, 85, 30));
    //展开按钮
    m_btnDetail->setGeometry(QRect(510, 140, 50, 30));
    m_btnDetail->setFlat(true);
    //在线列表
    m_frmOnline->setGeometry(QRect(15, 180, 540, 140));
    m_frmOnline->setFrameShape(QFrame::NoFrame);
    m_frmOnline->setFrameShadow(QFrame::Plain);
    m_lblClent1_ip4->setGeometry(QRect(10, 22, 40, 20));
    m_lblClent1_ip4->setFont(font_s120);
    m_lblClent1_ip6->setGeometry(QRect(10, 52, 40, 20));
    m_lblClent1_ip6->setFont(font_s120);
    m_lblClent1_addr4->setGeometry(QRect(55, 22, 250, 20));
    m_lblClent1_addr4->setFont(font_s120);
    m_lblClent1_addr6->setGeometry(QRect(55, 52, m_frmOnline->width()-20, 20));
    m_lblClent1_addr6->setFont(font_s120);
    m_btnOnline1->setGeometry(QRect(m_frmOnline->width()-80, 20, 70, 25));
    m_btnOffline1->setGeometry(QRect(m_btnOnline1->pos().x()-80, 20, 70, 25));
    m_btnForceOffline1->setGeometry(QRect(m_btnOffline1->pos().x()-80, 20, 70, 25));
    m_lblClent2_ip4->setGeometry(QRect(10, 82, 40, 20));
    m_lblClent2_ip4->setFont(font_s120);
    m_lblClent2_ip6->setGeometry(QRect(10, 112, 40, 20));
    m_lblClent2_ip6->setFont(font_s120);
    m_lblClent2_addr4->setGeometry(QRect(55, 82, 250, 20));
    m_lblClent2_addr4->setFont(font_s120);
    m_lblClent2_addr6->setGeometry(QRect(55, 112, m_frmOnline->width()-20, 20));
    m_lblClent2_addr6->setFont(font_s120);
    m_btnOnline2->setGeometry(QRect(m_frmOnline->width()-80, 80, 70, 25));
    m_btnOffline2->setGeometry(QRect(m_btnOnline2->pos().x()-80, 80, 70, 25));
    m_btnForceOffline2->setGeometry(QRect(m_btnOffline2->pos().x()-80, 80, 70, 25));

    //预约套餐
    m_frmOperation->setGeometry(QRect(15, 325, 180, 120));
    m_frmOperation->setFrameShape(QFrame::NoFrame);
    m_frmOperation->setFrameShadow(QFrame::Plain);
    m_lblCurrentBook->setGeometry(QRect(12, 23, 160, 20));
    m_lblCurrentBook->setFont(font_s115);
    m_cmbListBook->setGeometry(QRect(12, 48, 150, 25));
    m_cmbListBook->setFont(font_s115);
    m_btnRefreshBook->setGeometry(QRect(12, 80, 73, 25));
    m_btnRefreshBook->setFont(font_s115);
    m_btnSubmitBook->setGeometry(QRect(90, 80, 73, 25));
    m_btnSubmitBook->setFont(font_s115);
    // entrance
    m_frmBjutWeb->setGeometry(QRect(210, 325, 345, 120));
    m_frmBjutWeb->setFrameShape(QFrame::NoFrame);
    m_frmBjutWeb->setFrameShadow(QFrame::Plain);
    m_btnBjutWeb1->setGeometry(QRect(20, 23, 130, 25));
    m_btnBjutWeb1->setFont(font_s105);
    m_btnBjutWeb2->setGeometry(QRect(20, 53, 130, 25));
    m_btnBjutWeb2->setFont(font_s105);
    m_btnBjutWeb3->setGeometry(QRect(20, 83, 130, 25));
    m_btnBjutWeb3->setFont(font_s105);
    m_btnBjutWeb4->setGeometry(QRect(180, 23, 130, 25));
    m_btnBjutWeb4->setFont(font_s105);
    m_btnBjutWeb5->setGeometry(QRect(180, 53, 130, 25));
    m_btnBjutWeb5->setFont(font_s105);
    m_btnBjutWebMore->setGeometry(QRect(180, 83, 130, 25));
    m_btnBjutWebMore->setFont(font_s105);

    //消息日志框
    m_lblShowMsg->setGeometry(QRect(15, 450, 540, 20));
    m_txtMsg->setGeometry(QRect(15, 475, 540, 115));
    //版本号
    m_lblVersion->setFont(font_s100);
    // feedback
    m_lblFeedback->setFont(font_s100);
    m_lblFeedback->setStyleSheet("color:#303030;");

}
