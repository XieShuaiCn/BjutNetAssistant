#include "WndMain.h"
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
#include "../BjutNetService/BjutNet.h"
#include "WndTrayIcon.h"
#include "WndHelp.h"
#include "Utility.h"

using namespace bna::core;

namespace bna {

namespace gui {


WndMain::WndMain(WndTrayIcon *tray, QWidget *parent) :
    QWidget(parent),
    m_bNeedUpdate(false),
    m_tray(tray)
{
    m_szFrameSimple.setWidth(575);
    m_szFrameSimple.setHeight(195);
    m_szFrameAdvanced.setWidth(575);
    m_szFrameAdvanced.setHeight(495);
    m_szFrameShowMsg.setWidth(575);
    m_szFrameShowMsg.setHeight(620);
    m_nFlowGraphThickness = 10;
    tray->setMainWindow(this);
    m_coreBjutNet = tray->getBjutNet();
    //初始化界面
    initUI();
    //bjut web
    initBjutWeb();

    //关联信号
    connect(m_btnDetail, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnDetail_clicked);
    connect(m_btnLogout, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnLogout_clicked);
    connect(m_btnLogin, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnLogin_clicked);
    connect(m_btnRefresh, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnRefresh_clicked);
    connect(m_btnSetting, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnSetting_clicked);
    connect(m_btnHelp, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnHelp_clicked);
    connect(m_btnForceOffline1, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnForceOffline1_clicked);
    connect(m_btnForceOffline2, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnForceOffline2_clicked);
    connect(m_btnOffline1, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnOffline1_clicked);
    connect(m_btnOffline2, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnOffline2_clicked);
    connect(m_btnOnline1, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnOnline1_clicked);
    connect(m_btnOnline2, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnOnline2_clicked);
    connect(m_lblClent1_addr4, &bna::gui::HLabel::doubleClicked, this, &bna::gui::WndMain::on_lblClientaddr_doubleClicked);
    connect(m_lblClent1_addr6, &bna::gui::HLabel::doubleClicked, this, &bna::gui::WndMain::on_lblClientaddr_doubleClicked);
    connect(m_lblClent2_addr4, &bna::gui::HLabel::doubleClicked, this, &bna::gui::WndMain::on_lblClientaddr_doubleClicked);
    connect(m_lblClent2_addr6, &bna::gui::HLabel::doubleClicked, this, &bna::gui::WndMain::on_lblClientaddr_doubleClicked);
    connect(m_btnRefreshBook, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnRefreshBook_clicked);
    connect(m_cmbListBook, SIGNAL(highlighted(int)), this, SLOT(on_cmbListBook_highlight(int)));
    connect(m_btnSubmitBook, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnSubmitBook_clicked);
    connect(m_lblShowMsg, &bna::gui::HLabel::clicked, this, &bna::gui::WndMain::on_lblShowMsg_clicked);
    connect(m_btnBjutWebMore, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnBjutWebMore_clicked);
    connect(m_btnBjutWeb1, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnBjutWebCommon_clicked);
    connect(m_btnBjutWeb2, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnBjutWebCommon_clicked);
    connect(m_btnBjutWeb3, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnBjutWebCommon_clicked);
    connect(m_btnBjutWeb4, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnBjutWebCommon_clicked);
    connect(m_btnBjutWeb5, &QPushButton::clicked, this, &bna::gui::WndMain::on_btnBjutWebCommon_clicked);
    connect(m_lblVersion, &bna::gui::HLabel::clicked, this, &bna::gui::WndMain::on_lblVersion_clicked);
    connect(m_lblFeedback, &bna::gui::HLabel::clicked, this, &bna::gui::WndMain::on_lblFeedback_clicked);
    //connect(this, &WndMain::showed, this, &WndMain::on_show);
    if(m_coreBjutNet){
        connect(m_coreBjutNet, &bna::core::BjutNet::message, this, &bna::gui::WndMain::on_txtMsg_MessageWithTime);
        WebLgn &lgn = m_coreBjutNet->getWebLgn();
        connect(&lgn, &bna::core::WebLgn::status_update, this, &bna::gui::WndMain::on_account_StatusUpdated);
        WebJfself &jfself = m_coreBjutNet->getWebJfself();
        connect(&jfself, &bna::core::WebJfself::online_status_update, this, &bna::gui::WndMain::on_online_status);
    }
    else{
        QMessageBox::critical(this, "Init", "Fail to init core.", QMessageBox::Ok);
        m_tray->cmdExitApp();
    }
    //光标
    m_txtMsg->setFocus();
}

WndMain::~WndMain()
{
}

void WndMain::initBjutWeb()
{
    m_vecBjutWeb = m_tray->getBjutWebList();
    if(m_vecBjutWeb.size() > 0){
        m_btnBjutWeb1->setText(m_vecBjutWeb[0].name);
    }
    if(m_vecBjutWeb.size() > 1){
        m_btnBjutWeb2->setText(m_vecBjutWeb[1].name);
    }
    if(m_vecBjutWeb.size() > 2){
        m_btnBjutWeb3->setText(m_vecBjutWeb[2].name);
    }
    if(m_vecBjutWeb.size() > 3){
        m_btnBjutWeb4->setText(m_vecBjutWeb[3].name);
    }
    if(m_vecBjutWeb.size() > 4){
        m_btnBjutWeb5->setText(m_vecBjutWeb[4].name);
    }
    m_menuBjutWeb->clear();
    for(int i = 5; i < m_vecBjutWeb.size(); ++i){
        QAction *act = new QAction(m_vecBjutWeb[i].name);
        act->setData(QVariant(i));
        m_menuBjutWeb->addAction(act);
        connect(act, &QAction::triggered, this, &bna::gui::WndMain::on_btnBjutWebCommon_clicked);
    }
}

void WndMain::show()
{
    QWidget::show();
    QWidget::raise();
    QWidget::activateWindow();
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
    {
        int currentFlow = m_coreBjutNet->getWebLgn().getFlow() / 1024;//MB
        int totalFlow = m_coreBjutNet->getWebJfself().getTotalFlow();//MB
        double flowRate = -1.0;
        if(totalFlow > 0){
            flowRate = 1.0 * currentFlow / totalFlow;
        }
        //flowRate = 0.20;
        const QBrush brushGreen(QColor(60,180,60));
        const QBrush brushGrayDark(QColor(100,100,100));
        const QBrush brushGrayLight(QColor(150,150,150));
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
    }
    //draw a thin line as spliter in online devices' box
    {
        QPen penSpliter(QColor(180, 180, 180));
        int y = m_frmOnline->y()+m_frmOnline->height()/2+5;
        penSpliter.setWidth(1);
        penSpliter.setStyle(Qt::DashLine);
        painter.setPen(penSpliter);
        painter.drawLine(m_frmOnline->x(), y, m_frmOnline->x()+m_frmOnline->width(), y);
    }
    // draw a triangle of lblShowMsg
    {
        painter.setBrush(QBrush(QColor(50,50,50)));
        painter.setPen(Qt::NoPen);
        if(m_bShowMsg){
            QPolygon polyTriangle(3);
            polyTriangle.setPoint(0, m_lblShowMsg->pos().x()+5, m_lblShowMsg->pos().y()+5);
            polyTriangle.setPoint(1, m_lblShowMsg->pos().x()+m_lblShowMsg->height()-5,
                                     m_lblShowMsg->pos().y()+5);
            polyTriangle.setPoint(2, m_lblShowMsg->pos().x()+m_lblShowMsg->height()/2,
                                     m_lblShowMsg->pos().y()+m_lblShowMsg->height()-5);
            painter.drawPolygon(polyTriangle);
        }
        else{
            QPolygon polyTriangle(3);
            polyTriangle.setPoint(0, m_lblShowMsg->pos().x()+5, m_lblShowMsg->pos().y()+5);
            polyTriangle.setPoint(1, m_lblShowMsg->pos().x()+m_lblShowMsg->height()-5,
                                     m_lblShowMsg->pos().y()+m_lblShowMsg->height()/2);
            polyTriangle.setPoint(2, m_lblShowMsg->pos().x()+5,
                                     m_lblShowMsg->pos().y()+m_lblShowMsg->height()-5);
            painter.drawPolygon(polyTriangle);
        }
    }
}

void WndMain:: on_show()
{
    initBjutWeb();
    m_updater.checkUpdate();
    if(m_updater.needUpdate()){
        m_bNeedUpdate = true;
        QString ver("%1 <font color=#dd3333>最新版本：%2 点我更新！</font>");
        m_lblVersion->setText(ver.arg(m_updater.getOldVersion(), m_updater.getNewVersion()));
    }
    on_btnRefresh_clicked();
}

void WndMain::on_account_StatusUpdated(bool login, int time, int flow, int fee)
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
    fflow = double(int(fflow*1000)) / 1000.0;
    m_lcdNumFlow->display(fflow);
    m_lblFlowUnit->setText(flowUnit[flowUnitIndex]);
    m_lcdNumFee->display(float(fee) / 100);
    strFlowTip.append(QString("流量状态：已用%1%2").arg(fflow).arg(flowUnit[flowUnitIndex]));
    int totalFlow = m_coreBjutNet->getWebJfself().getTotalFlow();//MB
    if(totalFlow > 0){
        m_lblFlowUsed->setText(QString("已用：%1 %").arg(int(100.0 * flow / totalFlow / 1024)));
        fflow = totalFlow-flow/1024;
        flowUnitIndex = 1;
        while(fflow > 1024)
        {
            fflow /= 1024;
            ++flowUnitIndex;
        }
        fflow = double(int(fflow*1000)) / 1000.0;
        strFlowTip.append(QString("，剩余%1%2").arg(fflow).arg(flowUnit[flowUnitIndex]));
        m_frmFlowGraph->setToolTip(strFlowTip);
        this->update();
    }
    auto service = m_coreBjutNet->getWebJfself().getServiceName();
    if(service.size()){
        m_lblService->setText(service);
    }
}

void WndMain::on_online_status(const QVariant &var_info)
{
    QVector<QHostAddress> addrs;
    ListLocalIpAddress(addrs);
    QVector<bna::core::OnlineClientInfo> info = var_info.value<QVector<bna::core::OnlineClientInfo>>();
    // set offline
    for(auto &client : m_lstOnline){
        std::get<OLDEV_ONLINE>(client) = false;
    }
    // refresh list
    for(const OnlineClientInfo &client : info){
        OnlineDevice dev{client.nID, client.strIPv4, client.strIPv6, client.strMAC, true, false};
        m_lstOnline.push_front(dev);
        for(auto it = m_lstOnline.begin()+1; it != m_lstOnline.end();){
            if((std::get<OLDEV_IPV4>(*it).size() && client.strIPv4.size() && std::get<OLDEV_IPV4>(*it) == client.strIPv4)
            || (std::get<OLDEV_IPV6>(*it).size() && client.strIPv6.size() && std::get<OLDEV_IPV6>(*it) == client.strIPv6)){
                it = m_lstOnline.erase(it);
            }
            else{
                ++it;
            }
        }
    }
    // check local (this PC)
    for(auto &client : m_lstOnline){
        if(std::get<OLDEV_IPV4>(client).size() && addrs.contains(QHostAddress(std::get<OLDEV_IPV4>(client))))
        {
            std::get<OLDEV_LOCAL>(client) = true;
        }
        else if(std::get<OLDEV_IPV6>(client).size() && addrs.contains(QHostAddress(std::get<OLDEV_IPV6>(client))))
        {
            std::get<OLDEV_LOCAL>(client) = true;
        }
        else{
            std::get<OLDEV_LOCAL>(client) = false;
        }
    }
    // update frame
    if(m_lstOnline.size() > 0){
        auto &c = m_lstOnline.at(0);
        m_lblClent1_addr4->setText(std::get<OLDEV_IPV4>(c));
        m_lblClent1_addr6->setText(std::get<OLDEV_IPV6>(c));
        m_lblClent1_addr4->setVisible(true);
        m_lblClent1_addr6->setVisible(true);
        m_btnOffline1->setVisible(true);
        m_btnOnline1->setVisible(true);
        m_btnForceOffline1->setVisible(true);
        if(std::get<OLDEV_IPV4>(c).size() && std::get<OLDEV_LOCAL>(c))
        {
            m_lblClent1_addr4->setText(m_lblClent1_addr4->text()+"(本机)");
        }
        else if(std::get<OLDEV_IPV6>(c).size() && std::get<OLDEV_LOCAL>(c))
        {
            m_lblClent1_addr6->setText(m_lblClent1_addr6->text()+"(本机)");
        }
        // offline / online
        if(!std::get<OLDEV_ONLINE>(c)){
            m_lblClent1_addr4->setStyleSheet("color:#808080;");
            m_lblClent1_addr6->setStyleSheet("color:#808080;");
            m_btnOffline1->setEnabled(false);
            m_btnOnline1->setEnabled(true);
            if(std::get<OLDEV_IPV4>(c).size()==0 && std::get<OLDEV_IPV6>(c).size()>0){
                m_lblClent1_addr6->setText(m_lblClent1_addr6->text()+"(离线)");
            }
            else{
                m_lblClent1_addr4->setText(m_lblClent1_addr4->text()+"(离线)");
            }
        }
        else{
            m_lblClent1_addr4->setStyleSheet("color:#000000;");
            m_lblClent1_addr6->setStyleSheet("color:#000000;");
            m_btnOffline1->setEnabled(true);
            m_btnOnline1->setEnabled(false);
        }
    }
    else {
        m_lblClent1_addr4->setVisible(false);
        m_lblClent1_addr6->setVisible(false);
        m_btnOffline1->setVisible(false);
        m_btnOnline1->setVisible(false);
        m_btnForceOffline1->setVisible(false);
    }
    if(m_lstOnline.size() > 1){
        const auto &c = m_lstOnline.at(1);
        m_lblClent2_addr4->setText(std::get<OLDEV_IPV4>(c));
        m_lblClent2_addr6->setText(std::get<OLDEV_IPV6>(c));
        m_lblClent2_addr4->setVisible(true);
        m_lblClent2_addr6->setVisible(true);
        m_btnOffline2->setVisible(true);
        m_btnOnline2->setVisible(true);
        m_btnForceOffline2->setVisible(true);
        if(std::get<OLDEV_IPV4>(c).size() && std::get<OLDEV_LOCAL>(c))
        {
            m_lblClent2_addr4->setText(m_lblClent2_addr4->text()+"(本机)");
        }
        else if(std::get<OLDEV_IPV6>(c).size() && std::get<OLDEV_LOCAL>(c))
        {
            m_lblClent2_addr6->setText(m_lblClent2_addr6->text()+"(本机)");
        }
        // offline / online
        if(!std::get<OLDEV_ONLINE>(c)){
            m_lblClent2_addr4->setStyleSheet("color:#808080;");
            m_lblClent2_addr6->setStyleSheet("color:#808080;");
            m_btnOffline2->setEnabled(false);
            m_btnOnline2->setEnabled(true);
            if(std::get<OLDEV_IPV4>(c).size()==0 && std::get<OLDEV_IPV6>(c).size()>0){
                m_lblClent2_addr6->setText(m_lblClent2_addr6->text()+"(离线)");
            }
            else{
                m_lblClent2_addr4->setText(m_lblClent2_addr4->text()+"(离线)");
            }
        }
        else{
            m_lblClent2_addr4->setStyleSheet("color:#000000;");
            m_lblClent2_addr6->setStyleSheet("color:#000000;");
            m_btnOffline2->setEnabled(true);
            m_btnOnline2->setEnabled(false);
        }
    }
    else {
        m_lblClent2_addr4->setVisible(false);
        m_lblClent2_addr6->setVisible(false);
        m_btnOffline2->setVisible(false);
        m_btnOnline2->setVisible(false);
        m_btnForceOffline2->setVisible(false);
    }
    // remove redundant items
    if(m_lstOnline.size() > 2){
        auto it = m_lstOnline.begin();
        ++it;
        ++it;
        m_lstOnline.erase(it, m_lstOnline.end());
    }
}

void WndMain::on_txtMsg_MessageWithTime(const QDateTime &time, const QString &info)
{
    m_txtMsg->append(time.toString("[yyyy-MM-dd hh:mm:ss] ") + (info.endsWith('\n') ? info : (info + '\n')));
}

void WndMain::on_txtMsg_Message(const QString& info)
{
    m_txtMsg->append(QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ") + (info.endsWith('\n') ? info : (info + '\n')));
}


void WndMain::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    m_lblVersion->setGeometry(QRect(10, this->height()-24, 250, 20));
    m_lblFeedback->setGeometry(QRect(this->width()-70, this->height()-24, 70, 20));
}

void WndMain::on_btnRefresh_clicked()
{
    m_btnRefresh->setEnabled(false);
    on_txtMsg_Message("[INFO] Refresh all");
    WebLgn &lgn = m_coreBjutNet->getWebLgn();
    WebJfself &jfself = m_coreBjutNet->getWebJfself();
    jfself.refreshAccount();
    jfself.refreshOnline();
    if(jfself.getServiceName().size()){
        m_lblService->setText(jfself.getServiceName());
    }
    else{
        m_lblService->setText(QString("未检测到套餐"));
    }
    //更新页面显示的流量状态
    //on_txtMsg_Message(QDateTime::currentDateTime(), "检查网络状态");
    bool loged = lgn.checkLoginStatus();
    //on_account_status(lgn.getTime() > 0, lgn.getTime(), lgn.getFlow(), lgn.getFee());
    // 没有登陆外网时，连不上服务器，不检查更新
    if(loged){
        //on_txtMsg_Message(QDateTime::currentDateTime(), "检查版本更新");
        m_updater.checkUpdate();
        if(m_updater.needUpdate()){
            m_bNeedUpdate = true;
            QString ver("%1 <font color=#dd3333>最新版本：%2 点我更新！</font>");
            m_lblVersion->setText(ver.arg(m_updater.getOldVersion(), m_updater.getNewVersion()));
        }
    }
    //on_txtMsg_message(QDateTime::currentDateTime(), "刷新完成");
    m_btnRefresh->setEnabled(true);
}

void WndMain::on_btnSetting_clicked()
{
    m_tray->cmdShowSettingWnd();
}

void WndMain::on_btnHelp_clicked()
{
    m_tray->cmdShowHelpWnd();
}

void WndMain::on_btnDetail_clicked()
{
    m_bShowDetail = !m_bShowDetail;
    if(m_bShowDetail)
    {
        this->setFixedSize(m_bShowMsg ? m_szFrameShowMsg : m_szFrameAdvanced);
        m_btnDetail->setText(QString("<<简洁"));
        m_frmOnline->setVisible(true);
        m_frmOperation->setVisible(true);
        m_frmBjutWeb->setVisible(true);
    }
    else
    {
        this->setFixedSize(m_szFrameSimple);
        m_btnDetail->setText(QString(">>更多"));
        m_frmOnline->setVisible(false);
        m_frmOperation->setVisible(false);
        m_frmBjutWeb->setVisible(false);
    }
}

void WndMain::on_lblShowMsg_clicked()
{
    m_bShowMsg = !m_bShowMsg;
    if(m_bShowMsg)
    {
        this->setFixedSize(m_szFrameShowMsg);
    }
    else
    {
        this->setFixedSize(m_bShowDetail ? m_szFrameAdvanced : m_szFrameSimple);
    }
    this->m_txtMsg->setVisible(m_bShowMsg);
}

void WndMain::on_btnLogout_clicked()
{
    m_tray->cmdLogoutLgn();
    m_coreBjutNet->getWebJfself().refreshOnline();
}

void WndMain::on_btnLogin_clicked()
{
    m_tray->cmdLoginLgn();
    m_coreBjutNet->getWebJfself().refreshOnline();
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

void WndMain::on_btnRefreshBook_clicked()
{
    m_cmbListBook->clear();
    WebJfself &jfself = m_coreBjutNet->getWebJfself();
    jfself.refreshBookService();
    on_bookedService(jfself.getCurrentBookService());
    for(const auto &l : jfself.getBookServiceList())
    {
        m_cmbListBook->addItem(l.strName, QVariant::fromValue(l));
        //m_cmbListBook->setToolTip("");
    }
}

void WndMain::on_cmbListBook_highlight(int index)
{
    if(index >=0 && index < m_cmbListBook->count()){
        QVariant var = m_cmbListBook->itemData(index, Qt::UserRole);
        bool ok = var.canConvert<ServiceInfo>();
        if(ok){
            ServiceInfo info = var.value<ServiceInfo>();
            m_cmbListBook->setToolTip(info.strDescription);
        }
    }
}

void WndMain::on_btnSubmitBook_clicked()
{
    bool ok = m_cmbListBook->currentData().canConvert<ServiceInfo>();
    if(ok){
        ServiceInfo info = m_cmbListBook->currentData().value<ServiceInfo>();
        m_coreBjutNet->getWebJfself().submitBookService(info.nID);
    }
}

void WndMain::on_btnBjutWebCommon_clicked()
{
    try{
        QObject *obj = sender();
        if(obj == m_btnBjutWeb1){
            QDesktopServices::openUrl(m_vecBjutWeb[0].url);
            m_tray->increaseBjutWebFrequency(m_vecBjutWeb[0].id);
        }
        else if(obj == m_btnBjutWeb2){
            QDesktopServices::openUrl(m_vecBjutWeb[1].url);
            m_tray->increaseBjutWebFrequency(m_vecBjutWeb[1].id);
        }
        else if(obj == m_btnBjutWeb3){
            QDesktopServices::openUrl(m_vecBjutWeb[2].url);
            m_tray->increaseBjutWebFrequency(m_vecBjutWeb[2].id);
        }
        else if(obj == m_btnBjutWeb4){
            QDesktopServices::openUrl(m_vecBjutWeb[3].url);
            m_tray->increaseBjutWebFrequency(m_vecBjutWeb[3].id);
        }
        else if(obj == m_btnBjutWeb5){
            QDesktopServices::openUrl(m_vecBjutWeb[4].url);
            m_tray->increaseBjutWebFrequency(m_vecBjutWeb[4].id);
        }
        else{
            QAction *act = static_cast<QAction*>(obj);
            if(act->data().type() == QVariant::Int){
                auto &w = m_vecBjutWeb[act->data().value<int>()];
                QDesktopServices::openUrl(w.url);
                m_tray->increaseBjutWebFrequency(w.id);
            }
            else{
                QMessageBox mb(this);
                mb.setWindowTitle("常用入口");
                mb.setText("无法识别的调用！");
                mb.setStandardButtons(QMessageBox::Ok);
                mb.exec();
                return;
            }
        }
        initBjutWeb();
    }catch(...){
        QMessageBox mb(this);
        mb.setWindowTitle("常用入口");
        mb.setText("无法打开页面！");
        mb.setStandardButtons(QMessageBox::Ok);
        mb.exec();
        return;
    }
}

void WndMain::on_btnBjutWebMore_clicked()
{
    QPoint pos = this->mapToGlobal(m_frmBjutWeb->pos() + m_btnBjutWebMore->pos());
    m_menuBjutWeb->move(pos+QPoint(m_btnBjutWebMore->width(), 0));
    m_menuBjutWeb->exec();
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

void WndMain::on_lblFeedback_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.wjx.top/jq/44055734.aspx"));
}

void WndMain::logRemoteDevice(int index, bool login)
{
    if(m_lstOnline.size() > index){
        const OnlineDevice &od = m_lstOnline.at(index);
        // local device
        if(std::get<OLDEV_LOCAL>(od)){
            login ? m_tray->cmdLoginLgn() : m_tray->cmdLogoutLgn();
            on_txtMsg_Message(QString("[ OK ] ") + (login? "on":"off") + "line local device");
        }
        else{
            QString addr = std::get<OLDEV_IPV4>(od).size() ? std::get<OLDEV_IPV4>(od) : std::get<OLDEV_IPV6>(od);
            if(addr.size()){
                QHostAddress param_ip(addr);
                if(!param_ip.isNull()){
                    // local device
                    if(param_ip.isLoopback()){
                        login ? m_tray->cmdLoginLgn() : m_tray->cmdLogoutLgn();
                        on_txtMsg_Message(QString("[ OK ] ") + (login? "on":"off") + "line local device");
                    }
                    else{
                        m_guiServiceBridge.setAuth(true, m_coreBjutNet->getAccount(), m_coreBjutNet->getPassword());
                        if(!m_guiServiceBridge.setHost(addr)){
                            on_txtMsg_Message("[Fail] Cannot change remote host address.");
                            return;
                        }
                        if(!m_guiServiceBridge.sendSYN()){
                            on_txtMsg_Message("[Fail] Cannot sync with remote device.");
                            return;
                        }
                        bool ok = (login ? m_guiServiceBridge.sendActLoginBjut() : m_guiServiceBridge.sendActLogoutBjut());
                        if(ok){
                            on_txtMsg_Message(QString("[")+(ok?" OK ":"FAIL")+"] " + (login? "on":"off") + "line remote device:"+addr);
                        }
                        m_guiServiceBridge.setAuth(false);
                        m_guiServiceBridge.setHost(QHostAddress::LocalHost);
                    }
                }
            }
        }
    }
}

void WndMain::on_btnOnline1_clicked()
{
    logRemoteDevice(0, true);
    m_coreBjutNet->getWebJfself().refreshOnline();
}

void WndMain::on_btnOnline2_clicked()
{
    logRemoteDevice(1, true);
    m_coreBjutNet->getWebJfself().refreshOnline();
}

void WndMain::on_btnOffline1_clicked()
{
    logRemoteDevice(0, false);
    m_coreBjutNet->getWebJfself().refreshOnline();
}

void WndMain::on_btnOffline2_clicked()
{
    logRemoteDevice(1, false);
    m_coreBjutNet->getWebJfself().refreshOnline();
}

void WndMain::on_btnForceOffline1_clicked()
{
    if(m_lstOnline.size() >= 1){
        const OnlineDevice &od = m_lstOnline.at(0);
        if(std::get<OLDEV_ONLINE>(od))
            m_coreBjutNet->getWebJfself().toOffline(std::get<OLDEV_ID>(od));
    }
    m_coreBjutNet->getWebJfself().refreshOnline();
}

void WndMain::on_btnForceOffline2_clicked()
{
    if(m_lstOnline.size() >= 2){
        const OnlineDevice &od = m_lstOnline.at(1);
        if(std::get<OLDEV_ONLINE>(od))
            m_coreBjutNet->getWebJfself().toOffline(std::get<OLDEV_ID>(od));
    }
    m_coreBjutNet->getWebJfself().refreshOnline();
}

// recive the info of account service
void WndMain::on_serviceInfo(const QString &name, int totalFlow)
{
    if(totalFlow > 0){
        m_lblFlowUsed->setText(QString("已用：%1 %").arg(int(100.0 * m_coreBjutNet->getWebLgn().getFlow() / totalFlow / 1024)));
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
    if(m_coreBjutNet->getWebLgn().getLoginStatus()){
        m_updater.checkUpdate();
        if(m_updater.needUpdate()){
            m_bNeedUpdate = true;
            QString ver("%1 <font color=#dd3333>最新版本：%2 点我更新！</font>");
            m_lblVersion->setText(ver.arg(version, m_updater.getNewVersion()));
        }
    }
}

void WndMain::on_bookedService(const QString &name)
{
    if(name.size()){
        m_lblCurrentBook->setText("已预约:"+name);
    }
    else{
        m_lblCurrentBook->setText("暂未预约套餐");
    }
}

void WndMain::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if(m_dlgProgress) {
        m_dlgProgress->setValue(100.0 * bytesReceived / bytesTotal);
    }
}
}}
