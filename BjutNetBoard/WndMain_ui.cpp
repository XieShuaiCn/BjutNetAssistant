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

namespace bna{

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
    //窗口
    this->setObjectName(QStringLiteral("WndMain"));
    this->setWindowIcon(QIcon(":/icon/logo.ico"));
    //设置固定窗口大小
    this->setFixedSize(575, 190);
    //窗口控件

    //状态图
    m_frmGraph = new QFrame(this);
    m_frmGraph->setGeometry(QRect(10, 10, 280, 160));
    m_frmGraph->setFrameShape(QFrame::NoFrame);
    m_frmGraph->setFrameShadow(QFrame::Plain);
    m_lblFlowUsed = new QLabel(m_frmGraph);
    m_lblFlowUsed->setGeometry(QRect(0, 20, 100, 20));
    m_lblFlowUsed->setFont(font_s120);
    m_lblService = new QLabel(m_frmGraph);
    m_lblService->setGeometry(QRect(0, 0, 150, 20));
    m_lblService->setFont(font_s115);
    m_lblStatus = new QLabel(m_frmGraph);
    m_lblStatus->setGeometry(QRect(0, 40, 50, 20));
    m_lblStatus->setFont(font_s120);
    m_lblStatusFlag = new QLabel(m_frmGraph);
    m_lblStatusFlag->setGeometry(QRect(35, 40, 20, 20));
    m_lblStatusFlag->setFont(font_s120);
    m_btnRefresh = new QPushButton(m_frmGraph);
    m_btnRefresh->setGeometry(QRect(55, 40, 20, 20));
    m_btnRefresh->setFlat(true);
    m_btnRefresh->setFont(font_s120);
    m_frmFlowGraph = new QFrame(m_frmGraph);
    m_frmFlowGraph->setGeometry(QRect(110,0,160,160));
    m_frmFlowGraph->setFrameShape(QFrame::NoFrame);
    m_frmFlowGraph->setFrameShadow(QFrame::Plain);
    m_rectFlowGraph = QRect(m_frmGraph->pos()+m_frmFlowGraph->pos(), m_frmFlowGraph->size());
    //账户状态
    m_frmInfo = new QFrame(this);
    m_frmInfo->setGeometry(QRect(310, 10, 250, 120));
    m_frmInfo->setFrameShape(QFrame::NoFrame);
    m_frmInfo->setFrameShadow(QFrame::Plain);
    //状态label
    m_lblInfoTime = new QLabel(m_frmInfo);
    m_lblInfoTime->setGeometry(QRect(20, 10, 100, 20));
    m_lblInfoTime->setFont(font_s130);
    m_lblInfoFlow = new QLabel(m_frmInfo);
    m_lblInfoFlow->setGeometry(QRect(20, 50, 100, 20));
    m_lblInfoFlow->setFont(font_s130);
    m_lblInfoFee = new QLabel(m_frmInfo);
    m_lblInfoFee->setGeometry(QRect(20, 90, 100, 20));
    m_lblInfoFee->setFont(font_s130);
    //状态value
    m_lcdNumTime = new QLCDNumber(m_frmInfo);
    m_lcdNumTime->setGeometry(QRect(110, 10, 81, 23));
    m_lcdNumTime->setFrameShadow(QFrame::Plain);
    m_lcdNumTime->setLineWidth(1);
    m_lcdNumTime->setDigitCount(7);
    m_lcdNumTime->setSegmentStyle(QLCDNumber::Flat);
    m_lcdNumFlow = new QLCDNumber(m_frmInfo);
    m_lcdNumFlow->setGeometry(QRect(110, 50, 81, 23));
    m_lcdNumFlow->setFrameShadow(QFrame::Plain);
    m_lcdNumFlow->setLineWidth(1);
    m_lcdNumFlow->setDigitCount(7);
    m_lcdNumFlow->setSegmentStyle(QLCDNumber::Flat);
    m_lcdNumFee = new QLCDNumber(m_frmInfo);
    m_lcdNumFee->setGeometry(QRect(110, 90, 81, 23));
    m_lcdNumFee->setFrameShadow(QFrame::Plain);
    m_lcdNumFee->setLineWidth(1);
    m_lcdNumFee->setDigitCount(7);
    m_lcdNumFee->setSegmentStyle(QLCDNumber::Flat);
    //状态unit
    m_lblTimeUnit = new QLabel(m_frmInfo);
    m_lblTimeUnit->setGeometry(QRect(200, 10, 40, 20));
    m_lblTimeUnit->setFont(font_s130);
    m_lblFlowUnit = new QLabel(m_frmInfo);
    m_lblFlowUnit->setGeometry(QRect(200, 50, 40, 20));
    m_lblFlowUnit->setFont(font_s130);
    m_lblFeeUnit = new QLabel(m_frmInfo);
    m_lblFeeUnit->setGeometry(QRect(200, 90, 40, 20));
    m_lblFeeUnit->setFont(font_s130);
    //上线按钮
    m_btnLogin = new QPushButton(this);
    m_btnLogin->setGeometry(QRect(310, 140, 85, 30));
    //下线按钮
    m_btnLogout = new QPushButton(this);
    m_btnLogout->setGeometry(QRect(410, 140, 85, 30));
    //展开按钮
    m_btnDetail = new QPushButton(this);
    m_btnDetail->setGeometry(QRect(510, 140, 50, 30));
    m_btnDetail->setFlat(true);
    //在线列表
    m_frmOnline = new QFrame(this);
    m_frmOnline->setGeometry(QRect(10, 190, 370, 130));
    m_frmOnline->setFrameShape(QFrame::NoFrame);
    m_frmOnline->setFrameShadow(QFrame::Plain);
    m_lblClent1_ip4 = new QLabel(m_frmOnline);
    m_lblClent1_ip4->setGeometry(QRect(10, 10, 40, 20));
    m_lblClent1_ip4->setFont(font_s120);
    m_lblClent1_ip6 = new QLabel(m_frmOnline);
    m_lblClent1_ip6->setGeometry(QRect(10, 40, 40, 20));
    m_lblClent1_ip6->setFont(font_s120);
    m_lblClent1_addr4 = new HLabel(m_frmOnline);
    m_lblClent1_addr4->setGeometry(QRect(55, 10, 200, 20));
    m_lblClent1_addr4->setFont(font_s120);
    m_lblClent1_addr6 = new HLabel(m_frmOnline);
    m_lblClent1_addr6->setGeometry(QRect(55, 40, m_frmOnline->width()-20, 20));
    m_lblClent1_addr6->setFont(font_s115);
    m_btnOffline1 = new QPushButton(m_frmOnline);
    m_btnOffline1->setGeometry(QRect(m_frmOnline->width()-90, 8, 80, 25));
    m_btnOffline1->setVisible(false);
    m_lblClent2_ip4 = new QLabel(m_frmOnline);
    m_lblClent2_ip4->setGeometry(QRect(10, 70, 40, 20));
    m_lblClent2_ip4->setFont(font_s120);
    m_lblClent2_ip6 = new QLabel(m_frmOnline);
    m_lblClent2_ip6->setGeometry(QRect(10, 100, 40, 20));
    m_lblClent2_ip6->setFont(font_s120);
    m_lblClent2_addr4 = new HLabel(m_frmOnline);
    m_lblClent2_addr4->setGeometry(QRect(55, 70, 200, 20));
    m_lblClent2_addr4->setFont(font_s120);
    m_lblClent2_addr6 = new HLabel(m_frmOnline);
    m_lblClent2_addr6->setGeometry(QRect(55, 100, m_frmOnline->width()-20, 20));
    m_lblClent2_addr6->setFont(font_s115);
    m_btnOffline2 = new QPushButton(m_frmOnline);
    m_btnOffline2->setGeometry(QRect(m_frmOnline->width()-90, 68, 80, 25));
    m_btnOffline2->setVisible(false);
    m_lblCopyTip = new QLabel(m_frmOnline);
    m_lblCopyTip->setGeometry(QRect(m_btnOffline1->pos().x()-80, 8, 80, 25));
    m_lblCopyTip->setFont(font_s120);
    m_lblCopyTip->setStyleSheet("color:#808080;");

    //预约套餐
    m_frmOperation = new QFrame(this);
    m_frmOperation->setGeometry(QRect(390, 190, 170, 130));
    m_frmOperation->setFrameShape(QFrame::NoFrame);
    m_frmOperation->setFrameShadow(QFrame::Plain);
    m_btnOfficalWeb = new QPushButton(m_frmOperation);
    m_btnOfficalWeb->setGeometry(QRect(40, 10, 80, 25));
    m_btnOfficalWeb->setFont(font_s105);
    m_lblCurrentBook = new QLabel(m_frmOperation);
    m_lblCurrentBook->setGeometry(QRect(10, 40, 160, 20));
    m_lblCurrentBook->setFont(font_s100);
    m_cmbListBook = new QComboBox(m_frmOperation);
    m_cmbListBook->setGeometry(QRect(10, 65, 150, 25));
    m_cmbListBook->setFont(font_s100);
    m_btnRefreshBook = new QPushButton(m_frmOperation);
    m_btnRefreshBook->setGeometry(QRect(10, 97, 73, 25));
    m_btnRefreshBook->setFont(font_s100);
    m_btnSubmitBook = new QPushButton(m_frmOperation);
    m_btnSubmitBook->setGeometry(QRect(87, 97, 73, 25));
    m_btnSubmitBook->setFont(font_s100);

    //消息日志框
    m_txtMsg = new QTextEdit(this);
    m_txtMsg->setGeometry(QRect(15, 330, 540, 130));
    m_txtMsg->setReadOnly(true);
    //版本号
    m_lblVersion = new HLabel(this);
    m_lblVersion->setGeometry(QRect(10, this->height()-25, 250, 20));
    m_lblVersion->setFont(font_s100);

    //设置各控件的文本内容
    this->setWindowTitle(QString("北工大网关登录器"));
    m_lblService->setText(QString("未检测到套餐"));
    m_lblFlowUsed->setText(QString("已用：-- %"));
    m_lblStatus->setText(QString("状态"));
    m_lblStatusFlag->setPixmap(QPixmap(":/png/Offline"));
    m_lblStatusFlag->setToolTip(QString("离线"));
    m_btnRefresh->setToolTip("刷新数据");
    m_btnRefresh->setIcon(QIcon(":/png/BtnRefresh"));
    m_frmFlowGraph->setToolTip("流量使用情况图");
    m_frmFlowGraph->setToolTipDuration(0);
    m_lblInfoTime->setText(QString("已用时间："));
    m_lblInfoFlow->setText(QString("已用流量："));
    m_lblInfoFee->setText(QString("剩余金额："));
    m_lblTimeUnit->setText(QString("时"));
    m_lblFlowUnit->setText(QString("KB"));
    m_lblFeeUnit->setText(QString("元"));
    m_btnLogin->setText(QString("上线"));
    m_btnLogout->setText(QString("下线"));
    m_btnDetail->setText(QString("<<高级"));
    m_lblClent1_ip4->setText(QString("IPv4:"));
    m_lblClent1_ip6->setText(QString("IPv6:"));
    m_btnOffline1->setText(QString("强制下线"));
    m_lblClent2_ip4->setText(QString("IPv4:"));
    m_lblClent2_ip6->setText(QString("IPv6:"));
    m_btnOffline2->setText(QString("强制下线"));
    m_lblCopyTip->setText(QString("双击复制IP"));
    m_btnOfficalWeb->setText(QString("办理业务"));
    m_lblCurrentBook->setText(QString("已预约:[请刷新套餐]"));
    m_btnRefreshBook->setText(QString("刷新套餐"));
    m_btnSubmitBook->setText(QString("预约套餐"));
    m_lblVersion->setText(QString("版本号：") + m_updater.getOldVersion());
    //根据objectName关联信号
    //QMetaObject::connectSlotsByName(this);
}
}
