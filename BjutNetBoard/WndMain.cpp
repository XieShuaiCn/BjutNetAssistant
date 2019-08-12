#include "WndMain.h"
#include "WndTrayIcon.h"
#include "BjutNet.h"
#include "Utility.h"
#include <math.h>
#include <QDateTime>
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

namespace bna {

namespace gui {


WndMain::WndMain(WndTrayIcon *tray, QWidget *parent) :
    QWidget(parent),
    m_bNeedUpdate(false),
    m_tray(tray)
{
    m_szFrameSimple.setWidth(575);
    m_szFrameSimple.setHeight(190);
    m_szFrameAdvanced.setWidth(575);
    m_szFrameAdvanced.setHeight(650);
    tray->setMainWindow(this);
    m_net = tray->getBjutNet();
    //初始化界面
    initUI();

    //关联信号
    connect(m_btnDetail, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnDetail_clicked);
    connect(m_btnLogout, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnLogout_clicked);
    connect(m_btnLogin, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnLogin_clicked);
    connect(m_btnRefresh, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnRefresh_clicked);
    connect(m_btnOffline1, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnOffline1_clicked);
    connect(m_btnOffline2, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnOffline2_clicked);
    connect(m_lblClent1_addr4, &bna::gui::HLabel::doubleClicked, this, &bna::gui::WndMain::on_lblClientaddr_doubleClicked);
    connect(m_lblClent1_addr6, &bna::gui::HLabel::doubleClicked, this, &bna::gui::WndMain::on_lblClientaddr_doubleClicked);
    connect(m_lblClent2_addr4, &bna::gui::HLabel::doubleClicked, this, &bna::gui::WndMain::on_lblClientaddr_doubleClicked);
    connect(m_lblClent2_addr6, &bna::gui::HLabel::doubleClicked, this, &bna::gui::WndMain::on_lblClientaddr_doubleClicked);
    connect(m_btnOfficalWeb, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnOffcicalWeb_clicked);
    connect(m_btnRefreshBook, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnRefreshBook_clicked);
    connect(m_btnSubmitBook, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnSubmitBook_clicked);
    connect(m_lblVersion, &bna::gui::HLabel::clicked, this, &bna::gui::WndMain::on_lblVersion_clicked);
    //connect(this, &WndMain::showed, this, &WndMain::on_show);
    connect(m_net, &bna::gui::BjutNet::updateMessage, this, &bna::gui::WndMain::on_txtMsg_message);
    connect(m_net, &bna::gui::BjutNet::updateNetInfo, this, &bna::gui::WndMain::on_account_status);
    connect(m_net, &bna::gui::BjutNet::updateOnlineDevice, this, &bna::gui::WndMain::on_online_status);
    connect(m_net, &bna::gui::BjutNet::updateServiceInfo, this, &bna::gui::WndMain::on_serviceInfo);
    connect(m_net, &bna::gui::BjutNet::updateRemoteVersion, this, &bna::gui::WndMain::on_remoteVersion);
    connect(m_net, &bna::gui::BjutNet::updateAllServices, this, &bna::gui::WndMain::on_allServices);
    connect(m_net, &bna::gui::BjutNet::updateBookedService, this, &bna::gui::WndMain::on_bookedService);
    //光标
    m_txtMsg->setFocus();
}

WndMain::~WndMain()
{
}

void WndMain::show()
{
    QWidget::show();
    QWidget::raise();
    QWidget::activateWindow();
#ifdef Q_OS_WIN
    HWND hWnd = reinterpret_cast<HWND>QWidget::winId(); //reinterpret_cast
    ::SwitchToThisWindow(hWnd);
#endif
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
    int currentFlow = 999;//m_net->getUsedFlow() / 1024;//MB
    int totalFlow = 1000;//m_net->getServiceFlow();//MB
    double flowRate = -1.0;
    if(totalFlow > 0){
        flowRate = 1.0 * currentFlow / totalFlow;
    }
    const QBrush brushGreen(QColor(60,180,60));
    const QBrush brushGrayDark(QColor(100,100,100));
    const QBrush brushGrayLight(QColor(170,170,170));
    const QBrush brushRed(QColor(240, 60, 60));
    const QPen penSolidGrayDark(brushGrayDark, 1.0, Qt::SolidLine);
    const QPen penSolidGrayLight(brushGrayLight, 1.0, Qt::SolidLine);
    const QPen penDashGrayLight(brushGrayLight, 1.0, Qt::DashLine);
    if(totalFlow > 0 && flowRate < 0.998)
    {
        int angleStart = 90;
        int angleLength = 360 * flowRate;
        QPolygonF polySide(4);
        QPointF posCenter1 = m_rectFlowGraphTop.center()+QPoint(1,1);
        QPointF posCenter2 = m_rectFlowGraphBottom.center();
        polySide[0] = posCenter1;
        polySide[1] = posCenter2;
        polySide[2] = QPointF(posCenter2.x()+m_rectFlowGraphBottom.width()*0.5*sin(flowRate*3.1415926*2.0),
                          posCenter2.y()-m_rectFlowGraphBottom.height()*0.5*cos(flowRate*3.1415926*2.0));
        polySide[3] = QPointF(posCenter1.x()+m_rectFlowGraphTop.width()*0.5*sin(flowRate*3.1415926*2.0),
                          posCenter1.y()-m_rectFlowGraphTop.height()*0.5*cos(flowRate*3.1415926*2.0));
        //绘制底色
        painter.setBrush(Qt::NoBrush);
        // bottom plane border
        painter.setPen(penDashGrayLight);
        painter.drawArc(m_rectFlowGraphBottom, 0, 180*16);
        painter.setPen(penSolidGrayDark);
        QBrush brushPie = brushGreen;
        if(angleLength <= 90){
            painter.setBrush(brushGreen);
            painter.setPen(Qt::NoPen);
            painter.drawPolygon(polySide);
            painter.setPen(penSolidGrayDark);
            painter.drawLine(polySide[0], polySide[1]);
            painter.drawLine(polySide[1], polySide[2]);
            painter.drawLine(polySide[2], polySide[3]);
        }
        else if(angleLength <= 180){
            painter.setBrush(brushGreen);
            painter.drawPie(m_rectFlowGraphBottom, 0, -(angleLength-89.9)*16);
            painter.drawPolygon(polySide);
            painter.drawArc(m_rectFlowGraphBottom, 0, -(angleLength-89.9)*16);
        }
        else if(angleLength < 270)//流量大于1/2，变橙警示
        {
            brushPie.setColor(QColor(int(75+180*(flowRate-0.5)/0.25),
                                     180,
                                     40));
            painter.setBrush(brushPie);
            painter.setPen(Qt::NoPen);
            // bottom plane
            painter.drawPie(m_rectFlowGraphBottom, 0, -(angleLength-89.3)*16);
            // side plane
            painter.drawPolygon(polySide);
            painter.setPen(penSolidGrayDark);
            // side border
            painter.drawLine(polySide[2], polySide[3]);
            painter.drawArc(m_rectFlowGraphBottom, 0, -(angleLength-89.5)*16);
        }
        else if(angleLength < 360)//流量大于3/4，变红警示
        {
            brushPie.setColor(QColor(255,
                                     int(180-140*(flowRate-0.75)/0.25),
                                     40));
            // side plane
            painter.setBrush(brushPie);
            painter.drawPie(m_rectFlowGraphBottom, 0, -180*16);
            painter.drawRect(m_rectFlowGraphTop.left(), m_rectFlowGraphTop.y()+m_rectFlowGraphTop.height()*0.5,
                             m_rectFlowGraphTop.width(), m_nFlowGraphThickness+1);
            painter.drawArc(m_rectFlowGraphBottom, 0, -180*16);
        }
        else{
            // can not reach here
            painter.setBrush(brushRed);
            // side plane
            painter.drawPie(m_rectFlowGraphBottom, 0, -180*16);
            painter.drawRect(m_rectFlowGraphTop.left(), m_rectFlowGraphTop.y()+m_rectFlowGraphTop.height()*0.5,
                             m_rectFlowGraphTop.width(), m_nFlowGraphThickness+1);
        }
        painter.setPen(Qt::NoPen);
        // top plane
        painter.drawPie(m_rectFlowGraphTop, angleStart*16, -angleLength*16);
        painter.setPen(penSolidGrayDark);
        painter.setBrush(Qt::NoBrush);
        // side plane border
        painter.setPen(penSolidGrayLight);
        painter.drawArc(m_rectFlowGraphBottom, 0, -180*16);
        painter.drawLine(m_rectFlowGraphTop.left(), m_rectFlowGraphTop.y()+m_rectFlowGraphTop.height()*0.5,
                         m_rectFlowGraphBottom.left(), m_rectFlowGraphBottom.y()+m_rectFlowGraphBottom.height()*0.5);
        painter.drawLine(m_rectFlowGraphTop.right()+1, m_rectFlowGraphTop.y()+m_rectFlowGraphTop.height()*0.5,
                         m_rectFlowGraphBottom.right()+1, m_rectFlowGraphBottom.y()+m_rectFlowGraphBottom.height()*0.5);
        // top plane border
        painter.drawEllipse(m_rectFlowGraphTop);
        painter.setPen(penSolidGrayDark);
        painter.drawArc(m_rectFlowGraphTop, 90*16, -(angleLength)*16);
        // top pie border
        painter.drawLine(posCenter1.x(), m_rectFlowGraphTop.top(), posCenter1.x(), posCenter1.y());
        painter.drawLine(polySide[3], posCenter1);
    }
    else {
        if(totalFlow > 0 && flowRate >= 0.99){// > 100%
            painter.setBrush(brushRed);
        }
        else{// unKnown
            painter.setBrush(brushGreen);
        }
        painter.setPen(Qt::NoPen);
        painter.drawPie(m_rectFlowGraphBottom, 0, -180*16);
        painter.drawRect(m_rectFlowGraphTop.left(), m_rectFlowGraphTop.y()+m_rectFlowGraphTop.height()*0.5,
                         m_rectFlowGraphTop.width(), m_nFlowGraphThickness+1);

        painter.setPen(penSolidGrayDark);
        painter.drawArc(m_rectFlowGraphBottom, 0, -180*16);
        painter.drawLine(m_rectFlowGraphTop.left(), m_rectFlowGraphTop.y()+m_rectFlowGraphTop.height()*0.5,
                         m_rectFlowGraphBottom.left(), m_rectFlowGraphBottom.y()+m_rectFlowGraphBottom.height()*0.5);
        painter.drawLine(m_rectFlowGraphTop.right()+1, m_rectFlowGraphTop.y()+m_rectFlowGraphTop.height()*0.5,
                         m_rectFlowGraphBottom.right()+1, m_rectFlowGraphBottom.y()+m_rectFlowGraphBottom.height()*0.5);
        painter.drawEllipse(m_rectFlowGraphTop);
        if(totalFlow > 0 && flowRate >= 0.99){// > 100%
            painter.drawLine(m_rectFlowGraphTop.left()+m_rectFlowGraphTop.width()*0.5,
                             m_rectFlowGraphTop.y(),
                             m_rectFlowGraphTop.left()+m_rectFlowGraphTop.width()*0.5,
                             m_rectFlowGraphBottom.y()+m_rectFlowGraphBottom.height()*0.5);
        }
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

void WndMain::on_account_status(bool login, int flow, int time, int fee)
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
    int totalFlow = m_net->getServiceFlow();//MB
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
    auto service = m_net->getServiceName();
    if(service.size()){
        m_lblService->setText(service);
    }
}

void WndMain::on_online_status(const QVariant &var_info)
{
    QVector<QHostAddress> addrs;
    ListLocalIpAddress(addrs);
    BjutNet::TypeOnlineDevices info = var_info.value<BjutNet::TypeOnlineDevices>();
    if(info.size() > 0)
    {
        const auto &c = info[0];
        m_lblClent1_addr4->setText(c[1]);
        m_lblClent1_addr6->setText(c[2]);
        if((c[1].size() && addrs.contains(QHostAddress(c[1])))
            || (c[2].size() && addrs.contains(QHostAddress(c[2]))))
        {
            m_lblClent1_addr4->setText(m_lblClent1_addr4->text()+"(本机)");
        }
        m_strOnlineID[0] = c[0].toInt();
        m_lblClent1_addr4->setVisible(true);
        m_lblClent1_addr6->setVisible(true);
        m_btnOffline1->setVisible(true);
    }
    else {
        m_strOnlineID[0] = 0;
        m_lblClent1_addr4->setVisible(false);
        m_lblClent1_addr6->setVisible(false);
        m_btnOffline1->setVisible(false);
    }
    if(info.size() > 1)
    {
        const auto &c = info[1];
        m_lblClent2_addr4->setText(c[1]);
        m_lblClent2_addr6->setText(c[2]);
        if((c[1].size() && addrs.contains(QHostAddress(c[1])))
            || (c[2].size() && addrs.contains(QHostAddress(c[2]))))
        {
            m_lblClent2_addr4->setText(m_lblClent2_addr4->text()+"(本机)");
        }
        m_strOnlineID[1] = c[1].toInt();
        m_lblClent2_addr4->setVisible(true);
        m_lblClent2_addr6->setVisible(true);
        m_btnOffline2->setVisible(true);
    }
    else {
        m_strOnlineID[1] = 0;
        m_lblClent2_addr4->setVisible(false);
        m_lblClent2_addr6->setVisible(false);
        m_btnOffline2->setVisible(false);
    }
}

void WndMain::on_txtMsg_message(const QString& info)
{
    m_txtMsg->append(QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ") + (info.endsWith('\n') ? info : (info + '\n')));
}


void WndMain::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    m_lblVersion->setGeometry(QRect(10, this->height()-25, 250, 20));
}

void WndMain::on_btnRefresh_clicked()
{
    m_btnRefresh->setEnabled(false);
    on_txtMsg_message("[INFO] Refresh all");
    m_net->sendRefreshNetInfo();
    m_net->sendRefreshOnlineDevices();
    m_net->requireRemoteVersion();
    m_net->requireServiveInfo();
    m_net->requireNetInfo();
    m_net->requireOnlineDevices();
    m_btnRefresh->setEnabled(true);
}

void WndMain::on_btnDetail_clicked()
{
    if(m_bShowDetail)
    {   this->setFixedSize(m_szFrameSimple);
        m_bShowDetail = false;
        m_btnDetail->setText(QString("<<高级"));
    }
    else
    {   this->setFixedSize(m_szFrameAdvanced);
        m_bShowDetail = true;
        m_btnDetail->setText(QString("<<简洁"));
    }
}

void WndMain::on_btnLogout_clicked()
{
    m_net->sendLogout();
}

void WndMain::on_btnLogin_clicked()
{
    m_net->sendLogin();
}

void WndMain::on_lblClientaddr_doubleClicked()
{
    try{
        HLabel *lbl = dynamic_cast<HLabel *>(sender());
        QString addr = lbl->text();
        if(addr.size()==0) return;
        int index = addr.indexOf(QChar('('));
        if(index == 0) return;
        if(index > 0) addr = addr.left(index);
        QApplication::clipboard()->setText(addr);
    }
    catch(...){
        QMessageBox mb(this);
        mb.setText("复制IP地址失败");
        mb.setWindowTitle("双击IP复制到剪贴板");
        mb.exec();
    }
}
void WndMain::on_btnOffcicalWeb_clicked()
{
    QDesktopServices::openUrl(QUrl("https://jfself.bjut.edu.cn/"));
}

void WndMain::on_btnRefreshBook_clicked()
{
    m_net->requireAlldService();
    m_net->requireBookedService();
}

void WndMain::on_btnSubmitBook_clicked()
{
    const auto &lst = m_net->getAllServices();
    if(m_cmbListBook->currentIndex() >=0 && static_cast<size_t>(m_cmbListBook->currentIndex()) < lst.size())
        m_net->sendBookService(std::get<0>(lst[m_cmbListBook->currentIndex()]));
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
    if(m_strOnlineID[0])
        m_net->sendOfflineDevice(m_strOnlineID[0]);
    m_net->requireOnlineDevices();
}

void WndMain::on_btnOffline2_clicked()
{
    if(m_strOnlineID[1])
        m_net->sendOfflineDevice(m_strOnlineID[1]);
    m_net->requireOnlineDevices();
}

// recive the info of account service
void WndMain::on_serviceInfo(const QString &name, int totalFlow)
{
    //TODO: 1% why????
    if(totalFlow > 0){
        m_lblFlowUsed->setText(QString("已用：%1 %").arg(int(100.0 * m_net->getUsedFlow() / totalFlow / 1024)));
    }
    else {
         m_lblFlowUsed->setText(QString("已用：-- %"));
    }
    if(name.size()){
        m_lblService->setText(name);
    }
    else{
        m_lblService->setText(QString("未检测到套餐"));
    }
}
// recive the version of remote service
void WndMain::on_remoteVersion(const QString &version, int inner_ver)
{
    Q_UNUSED(inner_ver);
    if(m_net->getNetOnline()){
        m_updater.checkUpdate();
        if(m_updater.needUpdate()){
            m_bNeedUpdate = true;
            QString ver("%1 <font color=#dd3333>最新版本：%2 点我更新！</font>");
            m_lblVersion->setText(ver.arg(version, m_updater.getNewVersion()));
        }
    }
}
//
void WndMain::on_allServices(const QVariant &services)
{
    BjutNet::TypeAllServices serv = services.value<BjutNet::TypeAllServices>();
    m_cmbListBook->clear();
    for(const auto &l : serv)
    {
        m_cmbListBook->addItem(std::get<1>(l), QVariant(std::get<0>(l)));
    }
}
//
void WndMain::on_bookedService(const QString &name)
{
    m_lblCurrentBook->setText("已预约:["+name+"]");
}

void WndMain::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(m_dlgProgress) {
        m_dlgProgress->setValue(100.0 * bytesReceived / bytesTotal);
    }
}
}}
