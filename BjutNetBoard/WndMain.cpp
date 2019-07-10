#include "WndMain.h"
#include "WndTrayIcon.h"
#include "BjutNet.h"
#include <QPainter>
#include <QShowEvent>
#include <QDesktopServices>
#include <QMenu>
#include <QMessageBox>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QHeaderView>
#include <QLCDNumber>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include <QProgressDialog>
#include <QHostInfo>
#include <QHostAddress>
#include <QClipboard>
#include <QDir>

namespace bna{

WndMain::WndMain(WndTrayIcon *tray, QWidget *parent) :
    QWidget(parent),
    m_bNeedUpdate(false),
    m_tray(tray)
{
    tray->setMainWindow(this);
    m_service = tray->getService();
    //初始化界面
    initUI();

    //关联信号
    connect(m_btnDetail, &QPushButton::clicked, this, &WndMain::on_btnDetail_clicked);
    connect(m_btnLogout, &QPushButton::clicked, this, &WndMain::on_btnLogout_clicked);
    connect(m_btnLogin, &QPushButton::clicked, this, &WndMain::on_btnLogin_clicked);
    connect(m_btnRefresh, &QPushButton::clicked, this, &WndMain::on_btnRefresh_clicked);
    connect(m_btnOffline1, &QPushButton::clicked, this, &WndMain::on_btnOffline1_clicked);
    connect(m_btnOffline2, &QPushButton::clicked, this, &WndMain::on_btnOffline2_clicked);
    connect(m_lblClent1_addr4, &HLabel::doubleClicked, this, &WndMain::on_lblClient1addr4_doubleClicked);
    connect(m_lblClent1_addr6, &HLabel::doubleClicked, this, &WndMain::on_lblClient1addr6_doubleClicked);
    connect(m_lblClent2_addr4, &HLabel::doubleClicked, this, &WndMain::on_lblClient2addr4_doubleClicked);
    connect(m_lblClent2_addr6, &HLabel::doubleClicked, this, &WndMain::on_lblClient2addr6_doubleClicked);
    connect(m_btnOfficalWeb, &QPushButton::clicked, this, &WndMain::on_btnOffcicalWeb_clicked);
    connect(m_btnRefreshBook, &QPushButton::clicked, this, &WndMain::on_btnRefreshBook_clicked);
    connect(m_btnSubmitBook, &QPushButton::clicked, this, &WndMain::on_btnSubmitBook_clicked);
    connect(m_lblVersion, &HLabel::clicked, this, &WndMain::on_lblVersion_clicked);
    //connect(this, &WndMain::showed, this, &WndMain::on_show);
    //光标
    m_txtMsg->setFocus();
}

WndMain::~WndMain()
{
}

void WndMain::show()
{
    QWidget::show();
    //后台处理事件
    QCoreApplication::processEvents();
    on_show();
}

void WndMain::closeEvent(QCloseEvent *event)
{
    //QMessageBox::StandardButton ret = QMessageBox::information(this, "关闭", "是否关闭？", QMessageBox::Cancel, QMessageBox::Ok);
    //event->setAccepted(ret == QMessageBox::Ok);
    //取消关闭命令
    event->setAccepted(false);
    this->hide();
}

void WndMain::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    // 设置反锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    //绘制流量图
//    QFile f("flow.txt");
//    f.open(QIODevice::ReadOnly);
//    auto data = f.readAll();
//    f.close();
//    int currentFlow = QString(data).toInt();
    int currentFlow = m_net->getWebLgn().getFlow() / 1024;//MB
    int totalFlow = m_net->getWebJfself().getTotalFlow();//MB
    QBrush brushPie(QColor(60,180,60));
    if(totalFlow > 0)
    {
        int angleStart = 90;
        int angleLength = 360;
        //绘制底色
        double flowRate = 1.0 * currentFlow / totalFlow;
        angleLength = 360 * flowRate;
        if(angleLength >= 360){
            angleLength = 360;//
            brushPie.setColor(QColor(240, 60, 60));
        }
        else {
            painter.setBrush(QBrush(QColor(220,220,220)));
            painter.setPen(QPen(QColor(200,200,200)));
            painter.drawEllipse(m_rectFlowGraph);
            if(angleLength < 180){}
            else if(angleLength < 270)//流量大于一半，变红警示
            {
                brushPie.setColor(QColor(int(75+180*(flowRate-0.5)/0.25),
                                         180,
                                         40));
            }
            else if(angleLength < 361)//流量大于一半，变红警示
            {
                brushPie.setColor(QColor(255,
                                         int(180-140*(flowRate-0.75)/0.25),
                                         40));
            }
        }
        painter.setBrush(brushPie);
        painter.setPen(Qt::NoPen);
        painter.drawPie(m_rectFlowGraph, angleStart*16, -angleLength*16);
    }
    else {
        painter.setBrush(brushPie);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(m_rectFlowGraph);
    }
    //绘制panel边框
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QColor(180,180,180));
    //painter.drawRoundedRect(m_frmGraph->geometry(), 5, 5);
    //painter.drawRoundedRect(m_frmInfo->geometry(), 5, 5);
    painter.drawRoundedRect(m_frmOperation->geometry(), 5, 5);
    painter.drawRoundedRect(m_frmOnline->geometry(), 5, 5);
}

void WndMain:: on_show()
{
    m_updater.checkUpdate();
    if(m_updater.needUpdate()){
        m_bNeedUpdate = true;
        QString ver("%1 <font color=#dd3333>最新版本：%2 点我更新！</font>");
        m_lblVersion->setText(ver.arg(m_updater.getOldVersion(), m_updater.getNewVersion()));
    }
    on_btnRefresh_clicked();
}

void WndMain::on_account_status(bool login, int time, int flow, int fee)
{
    if(login)
    {
        m_lblStatusFlag->setPixmap(QPixmap(":/png/Online"));
        m_lblStatusFlag->setToolTip(QString("在线"));
    }
    else
    {
        m_lblStatusFlag->setPixmap(QPixmap(":/png/Offline"));
        m_lblStatusFlag->setToolTip(QString("离线"));
    }
    //窗口隐藏时，不更新
    if(this->isHidden())
    {
        return;
    }
    QString timeUnit[] = {"分钟", "小时"};
    QString flowUnit[] = {"KB", "MB", "GB", "TB"};
    //QString feeUnit[] = {"元"};
    if(time < 60)
    {
        m_lcdNumTime->display(time);
        m_lblTimeUnit->setText(timeUnit[0]);
    }
    else
    {
        m_lcdNumTime->display(float(time) / 60);
        m_lblTimeUnit->setText(timeUnit[1]);
    }
    QString strFlowTip;
    int flowUnitIndex = 0;
    float fflow = flow;
    while(fflow > 1024)
    {
        fflow /= 1024;
        ++flowUnitIndex;
    }
    m_lcdNumFlow->display(fflow);
    m_lblFlowUnit->setText(flowUnit[flowUnitIndex]);
    m_lcdNumFee->display(float(fee) / 100);
    strFlowTip.append(QString("流量状态：已用%1%2").arg(fflow).arg(flowUnit[flowUnitIndex]));
    int totalFlow = m_net->getWebJfself().getTotalFlow();//MB
    if(totalFlow > 0){
        m_lblFlowUsed->setText(QString("已用：%1 %").arg(int(100.0 * flow / totalFlow / 1024)));
        fflow = totalFlow-flow/1024;
        flowUnitIndex = 1;
        while(fflow > 1024)
        {
            fflow /= 1024;
            ++flowUnitIndex;
        }
        strFlowTip.append(QString("，剩余%1%2").arg(fflow).arg(flowUnit[flowUnitIndex]));
        m_frmFlowGraph->setToolTip(strFlowTip);
        this->update();
    }
    auto service = m_net->getWebJfself().getServiceName();
    if(service.size()){
        m_lblService->setText(service);
    }
    else{
        m_lblService->setText(QString("未检测到套餐"));
    }
}

void WndMain::on_online_status(const QVector<OnlineClientInfo> &info)
{
    QHostInfo host_info = QHostInfo::fromName(QHostInfo::localHostName());
    QList<QHostAddress> addrs = host_info.addresses();
    if(info.size() > 0)
    {
        const auto &c = info.first();
        m_lblClent1_addr4->setText(c.strIPv4);
        m_lblClent1_addr6->setText(c.strIPv6);
        if((c.strIPv4.size() && addrs.contains(QHostAddress(c.strIPv4)))
            || (c.strIPv6.size() && addrs.contains(QHostAddress(c.strIPv6))))
        {
            m_lblClent1_addr4->setText(m_lblClent1_addr4->text()+"(本机)");
        }
        m_strOnlineID[0] = c.strID;
        m_lblClent1_addr4->setVisible(true);
        m_lblClent1_addr6->setVisible(true);
        m_btnOffline1->setVisible(true);
    }
    else {
        m_strOnlineID[0].clear();
        m_lblClent1_addr4->setVisible(false);
        m_lblClent1_addr6->setVisible(false);
        m_btnOffline1->setVisible(false);
    }
    if(info.size() > 1)
    {
        const auto &c = info.last();
        m_lblClent2_addr4->setText(c.strIPv4);
        m_lblClent2_addr6->setText(c.strIPv6);
        if((c.strIPv4.size() && addrs.contains(QHostAddress(c.strIPv4)))
            || (c.strIPv6.size() && addrs.contains(QHostAddress(c.strIPv6))))
        {
            m_lblClent2_addr4->setText(m_lblClent2_addr4->text()+"(本机)");
        }
        m_strOnlineID[1] = c.strID;
        m_lblClent2_addr4->setVisible(true);
        m_lblClent2_addr6->setVisible(true);
        m_btnOffline2->setVisible(true);
    }
    else {
        m_strOnlineID[1].clear();
        m_lblClent2_addr4->setVisible(false);
        m_lblClent2_addr6->setVisible(false);
        m_btnOffline2->setVisible(false);
    }
}

void WndMain::on_txtMsg_message(const QDateTime& time, const QString& info)
{
    m_txtMsg->append(time.toString("yyyy-MM-dd hh:mm:ss  ") + (info.endsWith('\n') ? info : (info + '\n')));
}


void WndMain::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    m_lblVersion->setGeometry(QRect(10, this->height()-25, 250, 20));
}

void WndMain::on_btnRefresh_clicked()
{
    m_btnRefresh->setEnabled(false);
    WebLgn &lgn = m_net->getWebLgn();
    WebJfself &jfself = m_net->getWebJfself();
    on_txtMsg_message(QDateTime::currentDateTime(), "刷新账户信息");
    jfself.refreshAccount();
    //on_txtMsg_message(QDateTime::currentDateTime(), "刷新在线设备");
    jfself.refreshOnline();
    //on_txtMsg_message(QDateTime::currentDateTime(), "更新界面信息");
    if(jfself.getTotalFlow() > 0){
        m_lblFlowUsed->setText(QString("已用：%1 %").arg(100 * lgn.getFlow() / jfself.getTotalFlow() / 1024));
    }
    else {
         m_lblFlowUsed->setText(QString("已用：-- %"));
    }
    if(jfself.getServiceName().size()){
        m_lblService->setText(jfself.getServiceName());
    }
    else{
        m_lblService->setText(QString("未检测到套餐"));
    }
    //更新页面显示的流量状态
    on_txtMsg_message(QDateTime::currentDateTime(), "检查网络状态");
    bool loged = lgn.checkLoginStatus();
    //on_account_status(lgn.getTime() > 0, lgn.getTime(), lgn.getFlow(), lgn.getFee());
    // 没有登陆外网时，连不上服务器，不检查更新
    if(loged){
        on_txtMsg_message(QDateTime::currentDateTime(), "检查版本更新");
        m_updater.checkUpdate();
        if(m_updater.needUpdate()){
            m_bNeedUpdate = true;
            QString ver("%1 <font color=#dd3333>最新版本：%2 点我更新！</font>");
            m_lblVersion->setText(ver.arg(m_updater.getOldVersion(), m_updater.getNewVersion()));
        }
    }
    on_txtMsg_message(QDateTime::currentDateTime(), "刷新完成");
    m_btnRefresh->setEnabled(true);
}

void WndMain::on_btnDetail_clicked()
{
    if(m_bShowDetail)
    {   this->setFixedSize(575, 190);
        m_bShowDetail = false;
        m_btnDetail->setText(QString("<<高级"));
    }
    else
    {   this->setFixedSize(575, 490);
        m_bShowDetail = true;
        m_btnDetail->setText(QString("<<简洁"));
    }
}

void WndMain::on_btnLogout_clicked()
{
    m_net->stop_monitor();
    m_net->getWebLgn().logout();
}

void WndMain::on_btnLogin_clicked()
{
    m_net->stop_monitor();
    m_net->start_monitor();
}

void WndMain::on_lblClient1addr4_doubleClicked()
{
    QString addr = m_lblClent1_addr4->text();
    if(addr.size()==0) return;
    int index = addr.indexOf(QChar('('));
    if(index == 0) return;
    if(index > 0) addr = addr.left(index);
    QApplication::clipboard()->setText(addr);
}
void WndMain::on_lblClient1addr6_doubleClicked()
{
    QString addr = m_lblClent1_addr6->text();
    if(addr.size()==0) return;
    int index = addr.indexOf(QChar('('));
    if(index == 0) return;
    if(index > 0) addr = addr.left(index);
    QApplication::clipboard()->setText(addr);
}
void WndMain::on_lblClient2addr4_doubleClicked()
{
    QString addr = m_lblClent2_addr4->text();
    if(addr.size()==0) return;
    int index = addr.indexOf(QChar('('));
    if(index == 0) return;
    if(index > 0) addr = addr.left(index);
    QApplication::clipboard()->setText(addr);
}
void WndMain::on_lblClient2addr6_doubleClicked()
{
    QString addr = m_lblClent2_addr6->text();
    if(addr.size()==0) return;
    int index = addr.indexOf(QChar('('));
    if(index == 0) return;
    if(index > 0) addr = addr.left(index);
    QApplication::clipboard()->setText(addr);
}

void WndMain::on_btnOffcicalWeb_clicked()
{
    QDesktopServices::openUrl(QUrl("https://jfself.bjut.edu.cn/"));
}

void WndMain::on_btnRefreshBook_clicked()
{
    m_cmbListBook->clear();
    auto &jfself = m_net->getWebJfself();
    jfself.refreshBookService();
    m_lblCurrentBook->setText("已预约:["+jfself.getCurrentBookService()+"]");
    const auto &lst = jfself.getBookServiceList();
    QStringList items;
    for(const auto &l : lst)
    {
        items.append(l.second);
    }
    m_cmbListBook->addItems(items);
}

void WndMain::on_btnSubmitBook_clicked()
{
    auto &jfself = m_net->getWebJfself();
    const auto &lst = jfself.getBookServiceList();
    if(m_cmbListBook->currentIndex() >=0 && m_cmbListBook->currentIndex() < lst.size())
        jfself.submitBookService(lst[m_cmbListBook->currentIndex()].first);
}

void WndMain::on_lblVersion_clicked()
{
    QMessageBox mb(this);
    if(m_bNeedUpdate) {
        QString msg = QString("新版本更新：\n版本号：%1\n%2\n(更新过程尽可能不消耗流量)")
                .arg(m_updater.getNewVersion())
                .arg(m_updater.getNewIntroduction());
        mb.setWindowTitle("立即更新");
        mb.setText(msg);
        mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        if(QMessageBox::Yes == mb.exec()) {
            if(m_updater.runMaintainTool()){
                m_tray->cmdExitApp();
            }
            else{
#ifdef Q_OS_WIN32
                QMessageBox mbnew(this);
                mbnew.setText("启动更新工具失败，是否下载完整安装包？");
                mbnew.setWindowTitle("完整安装");
                mbnew.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                if(QMessageBox::Yes == mbnew.exec()){
                    m_dlgProgress = new QProgressDialog("正在下载....", "无法取消", 0, 101, this);
                    m_dlgProgress->setWindowModality(Qt::WindowModal);
                    m_dlgProgress->setCancelButton(nullptr);
                    m_dlgProgress->show();
                    connect(&m_updater, &Updater::downloadProgress, this, &WndMain::downloadProgress);
                    if(m_updater.downloadNewPackage())
                    {
                       mb.setWindowTitle("更新成功");
                       mb.setText("正在启动....");
                       if(m_tray)
                       {
                           m_tray->cmdExitApp();
                       }
                    } else {
                       mb.setWindowTitle("更新失败");
                       mb.setText("软件更新失败，请稍候重试。");
                    }
                    disconnect(&m_updater, nullptr, nullptr, nullptr);
                    mb.setStandardButtons(QMessageBox::Ok);
                    mb.exec();
                    m_dlgProgress->close();
                }
#else
                QMessageBox mbnew(this);
                mbnew.setText("启动更新工具失败，请到官网下载完整安装包。");
                mbnew.setWindowTitle("完整安装");
                mbnew.setStandardButtons(QMessageBox::Ok);
                mbnew.exec();
                QDesktopServices::openUrl(QUrl("http://bnl.hrrcn.com/"));
#endif
            }
        }
    } else {
        mb.setWindowTitle("已是最新版本");
        QString status("未检测到更新。当前已是最新版本(%1)。");
        mb.setText(status.arg(m_updater.getNewVersion()));
        mb.setStandardButtons(QMessageBox::Ok);
        mb.exec();
    }
}

void WndMain::on_btnOffline1_clicked()
{
    if(m_strOnlineID[0].size())
        m_net->getWebJfself().toOffline(m_strOnlineID[0]);
    m_net->getWebJfself().refreshOnline();
}

void WndMain::on_btnOffline2_clicked()
{
    if(m_strOnlineID[1].size())
        m_net->getWebJfself().toOffline(m_strOnlineID[1]);
    m_net->getWebJfself().refreshOnline();
}

void WndMain::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(m_dlgProgress) {
        m_dlgProgress->setValue(100.0 * bytesReceived / bytesTotal);
    }
}
}
