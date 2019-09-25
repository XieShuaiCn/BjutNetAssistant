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


void bna::gui::WndMain::initUI()
{
    //窗口
    this->setObjectName(QStringLiteral("WndMain"));
    this->setWindowIcon(QIcon(":/icon/logo.ico"));
    //设置固定窗口大小
    this->setFixedSize(m_szFrameSimple);
    //窗口控件

    //状态图
    m_frmGraph = new QFrame(this);
    m_lblFlowUsed = new QLabel(m_frmGraph);
    m_lblService = new QLabel(m_frmGraph);
    m_lblStatus = new QLabel(m_frmGraph);
    m_lblStatusFlag = new QLabel(m_frmGraph);
    m_btnRefresh = new QPushButton(m_frmGraph);
    m_btnSetting = new QPushButton(m_frmGraph);
    m_btnHelp = new QPushButton(m_frmGraph);
    m_frmFlowGraph = new QFrame(m_frmGraph);
    //账户状态
    m_frmInfo = new QFrame(this);
    //状态label
    m_lblInfoTime = new QLabel(m_frmInfo);
    m_lblInfoFlow = new QLabel(m_frmInfo);
    m_lblInfoFee = new QLabel(m_frmInfo);
    //状态value
    m_lcdNumTime = new QLCDNumber(m_frmInfo);
    m_lcdNumFlow = new QLCDNumber(m_frmInfo);
    m_lcdNumFee = new QLCDNumber(m_frmInfo);
    //状态unit
    m_lblTimeUnit = new QLabel(m_frmInfo);
    m_lblFlowUnit = new QLabel(m_frmInfo);
    m_lblFeeUnit = new QLabel(m_frmInfo);
    //上线按钮
    m_btnLogin = new QPushButton(this);
    //下线按钮
    m_btnLogout = new QPushButton(this);
    //展开按钮
    m_btnDetail = new QPushButton(this);
    //在线列表
    m_frmOnline = new HPanel(this);
    m_frmOnline->setVisible(false);
    m_lblClent1_ip4 = new QLabel(m_frmOnline);
    m_lblClent1_ip6 = new QLabel(m_frmOnline);
    m_lblClent1_addr4 = new HLabel(m_frmOnline);
    m_lblClent1_addr6 = new HLabel(m_frmOnline);
    m_btnOnline1 = new QPushButton(m_frmOnline);
    m_btnOnline1->setVisible(false);
    m_btnOffline1 = new QPushButton(m_frmOnline);
    m_btnOffline1->setVisible(false);
    m_btnForceOffline1 = new QPushButton(m_frmOnline);
    m_btnForceOffline1->setVisible(false);
    m_lblClent2_ip4 = new QLabel(m_frmOnline);
    m_lblClent2_ip6 = new QLabel(m_frmOnline);
    m_lblClent2_addr4 = new HLabel(m_frmOnline);
    m_lblClent2_addr6 = new HLabel(m_frmOnline);
    m_btnOnline2 = new QPushButton(m_frmOnline);
    m_btnOnline2->setVisible(false);
    m_btnOffline2 = new QPushButton(m_frmOnline);
    m_btnOffline2->setVisible(false);
    m_btnForceOffline2 = new QPushButton(m_frmOnline);
    m_btnForceOffline2->setVisible(false);

    //预约套餐
    m_frmOperation = new HPanel(this);
    m_frmOperation->setVisible(false);
    m_lblCurrentBook = new QLabel(m_frmOperation);
    m_cmbListBook = new QComboBox(m_frmOperation);
    m_btnRefreshBook = new QPushButton(m_frmOperation);
    m_btnSubmitBook = new QPushButton(m_frmOperation);
    // entrance
    m_frmBjutWeb = new HPanel(this);
    m_frmBjutWeb->setVisible(false);
    m_btnBjutWeb1 = new QPushButton(m_frmBjutWeb);
    m_btnBjutWeb2 = new QPushButton(m_frmBjutWeb);
    m_btnBjutWeb3 = new QPushButton(m_frmBjutWeb);
    m_btnBjutWeb4 = new QPushButton(m_frmBjutWeb);
    m_btnBjutWeb5 = new QPushButton(m_frmBjutWeb);
    m_btnBjutWebMore = new QPushButton(m_frmBjutWeb);
    m_menuBjutWeb = new QMenu(m_frmBjutWeb);

    //消息日志框
    m_lblShowMsg = new HLabel(this);
    m_txtMsg = new QTextEdit(this);
    m_txtMsg->setReadOnly(true);
    m_txtMsg->setVisible(false);
    //版本号
    m_lblVersion = new HLabel(this);
    // feedback
    m_lblFeedback = new HLabel(this);

    layoutUI();

    // UI relative variable
    m_rectFlowGraph = QRect(m_frmGraph->pos()+m_frmFlowGraph->pos(),
                            m_frmFlowGraph->size());
    m_rectFlowGraphTop = QRect(m_rectFlowGraph.topLeft()+QPoint(0, m_nFlowGraphThickness),
                            m_rectFlowGraph.size()-QSize(0, m_nFlowGraphThickness*3));
    m_rectFlowGraphBottom = QRect(m_rectFlowGraphTop.topLeft()+QPoint(0, m_nFlowGraphThickness),
                            m_rectFlowGraphTop.size());

    //设置各控件的文本内容
    this->setWindowTitle(QString("北工大网关登录器"));
    m_lblService->setText(QString("未检测到套餐"));
    m_lblFlowUsed->setText(QString("Used"));
    m_lblStatus->setText(QString("状态"));
    m_lblStatusFlag->setPixmap(QPixmap(":/png/Offline"));
    m_lblStatusFlag->setToolTip(QString("离线"));
    m_btnRefresh->setToolTip("刷新数据");
    m_btnRefresh->setIcon(QIcon(":/png/BtnRefresh"));
    m_btnSetting->setToolTip("设置");
    m_btnSetting->setIcon(QIcon(":/png/BtnSetting"));
    m_btnHelp->setToolTip("帮助信息");
    m_btnHelp->setIcon(QIcon(":/png/BtnHelp"));
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
    m_btnDetail->setText(QString(">>更多"));
    m_frmOnline->setText(QString("在线列表(双击IP可复制)"));
    m_lblClent1_ip4->setText(QString("IPv4:"));
    m_lblClent1_ip6->setText(QString("IPv6:"));
    m_btnOnline1->setText(QString("尝试上线"));
    m_btnOffline1->setText(QString("尝试下线"));
    m_btnForceOffline1->setText(QString("强制下线"));
    m_lblClent2_ip4->setText(QString("IPv4:"));
    m_lblClent2_ip6->setText(QString("IPv6:"));
    m_btnOnline2->setText(QString("尝试上线"));
    m_btnOffline2->setText(QString("尝试下线"));
    m_btnForceOffline2->setText(QString("强制下线"));
    //m_lblCopyTip->setText(QString("双击可复制IP"));
    m_frmOperation->setText(QString("业务办理"));
    m_lblCurrentBook->setText(QString("请刷新套餐"));
    m_btnRefreshBook->setText(QString("刷新套餐"));
    m_btnSubmitBook->setText(QString("预约套餐"));
    m_frmBjutWeb->setText(QString("常用入口"));
    m_btnBjutWebMore->setText(QString("更多入口>>"));
    m_lblShowMsg->setText(QString("日志"));
    m_lblVersion->setText(QString("版本号：") + m_updater.getOldVersion());
    m_lblFeedback->setText(QString("意见反馈"));
    //根据objectName关联信号
    //QMetaObject::connectSlotsByName(this);
}
