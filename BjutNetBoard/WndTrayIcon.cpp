#include "WndTrayIcon.h"
#include <QMenu>
#include <QApplication>
#include <QLocalSocket>
#include "BjutNet.h"
#include "WndMain.h"
#include "WndSetting.h"

namespace bna {

namespace gui {

WndTrayIcon::WndTrayIcon(QApplication *app,QObject *parent):
    QSystemTrayIcon(parent),
    m_app(app),
    m_wndMain(Q_NULLPTR),
    m_wndSetting(Q_NULLPTR)
{
    this->setToolTip("北工大校园网助手");
    QIcon ico(":/icon/logo.ico");
    this->setIcon(ico);

    m_menuTray = new QMenu();
    m_actMenuShowMain = new QAction("显示主窗口");
    m_actMenuLogin = new QAction("上线");
    m_actMenuLogout = new QAction("下线");
    m_actMenuSetting = new QAction("设置");
    m_actMenuQuit = new QAction("退出");
    m_menuTray->addAction(m_actMenuShowMain);
    m_menuTray->addSeparator();
    m_menuTray->addAction(m_actMenuLogin);
    m_menuTray->addAction(m_actMenuLogout);
    m_menuTray->addSeparator();
    m_menuTray->addAction(m_actMenuSetting);
    m_menuTray->addAction(m_actMenuQuit);
    this->setContextMenu(m_menuTray);

    //双击间隔200ms
    m_tmClick.setInterval(200);
    //一次触发
    m_tmClick.setSingleShot(true);

    connect(this, &WndTrayIcon::activated, this, &WndTrayIcon::on_actived);
    connect(m_actMenuShowMain, &QAction::triggered, this, &WndTrayIcon::cmdShowMainWnd);
    connect(m_actMenuLogin, &QAction::triggered, this, &WndTrayIcon::cmdLoginLgn);
    connect(m_actMenuLogout, &QAction::triggered, this, &WndTrayIcon::cmdLogoutLgn);
    connect(m_actMenuSetting, &QAction::triggered, this, &WndTrayIcon::cmdShowSettingWnd);
    connect(m_actMenuQuit, &QAction::triggered, this, &WndTrayIcon::cmdExitApp);
    connect(&m_tmClick, &QTimer::timeout, this, &WndTrayIcon::on_clicked);

    m_bjutnet = new BjutNet;
    Q_ASSERT(m_bjutnet!=nullptr);
    m_bjutnet->start();
}

WndTrayIcon::~WndTrayIcon()
{
    if(m_bjutnet)
    {
        if(m_bjutnet->isRunning()){
            m_bjutnet->stop();
            if(!m_bjutnet->wait(500)){ // ms
                m_bjutnet->terminate();
            }
        }
        m_bjutnet->deleteLater();
        m_bjutnet = Q_NULLPTR;
    }
    if(m_wndSetting)
    {
        m_wndSetting->close();
    }
    if(m_wndMain)
    {
        m_wndMain->close();
    }
    delete m_menuTray;
}

void WndTrayIcon::on_clicked()
{
    QString status;
    bool online;
    int time;
    int flow;
    int fee;
    const char* flowUnit[] = {"KB", "MB", "GB", "TB"};
    int flowUnitIndex = 0;
    m_bjutnet->getNetInfo(online, flow, time, fee);
    float ftime = float(time) / 60;
    float fflow = float(flow);
    while(fflow > 1024)
    {
        fflow /= 1024;
        ++flowUnitIndex;
    }
    float ffee = float(fee) / 100;
    status.sprintf("已用时间：%.2f小时，已用流量：%.2f%s，剩余金额：%.2f元", ftime, fflow, flowUnit[flowUnitIndex], ffee);
    //return status;
    this->showMessage("北工大校园网助手", status, QSystemTrayIcon::NoIcon);
    m_tmClick.stop();
}

void WndTrayIcon::cmdShowMainWnd()
{
    if (!m_wndMain)
    {
        m_wndMain = new WndMain(this);
    }
    m_wndMain->show();
}

void WndTrayIcon::cmdShowSettingWnd()
{
    if(!m_wndSetting)
    {
        m_wndSetting = new WndSetting(this);
    }
    m_wndSetting->show();
}

void WndTrayIcon::cmdExitApp()
{
    if(m_app != Q_NULLPTR)
    {
        m_app->quit();
    }
}

void WndTrayIcon::cmdLoginLgn()
{
    m_bjutnet->sendLogin();
}

void WndTrayIcon::cmdLogoutLgn()
{
    m_bjutnet->sendLogout();
}

void WndTrayIcon::on_actived(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Context:
        if(m_tmClick.isActive())
        {//双击
            m_tmClick.stop();
            m_menuTray->exec();
        }
        else
        {//单击计时器
            m_tmClick.start();
        }
        break;
    case QSystemTrayIcon::DoubleClick:
        cmdShowMainWnd();
        break;
    case QSystemTrayIcon::Trigger:
        if(m_tmClick.isActive())
        {//双击
            m_tmClick.stop();
            cmdShowMainWnd();
        }
        else
        {//单击计时器
            m_tmClick.start();
        }
        break;
    default:
        break;
    }
}

void WndTrayIcon::reciveMessage(const QString &msg)
{
    const char *m = msg.toStdString().data();
    if(0==strcmp(m, "ShowMainWnd")){
        cmdShowMainWnd();
    }
    else if(0 == strcmp(m, "ShowSettingWnd")) {
        cmdShowSettingWnd();
    }
    else if(0==strcmp(m, "ExitApp")){
        cmdExitApp();
    }
}
}}
