#include "WndTrayIcon.h"
#include <QMenu>
#include <QApplication>
#include <QLocalSocket>
#include <QDesktopServices>
#include <QMessageBox>
#include "../BjutNetService/BjutNet.h"
#include "WndMain.h"
#include "WndSetting.h"
#include "WndHelp.h"
#include "UISetting.h"

namespace bna {
namespace gui {

WndTrayIcon::WndTrayIcon(QApplication *app, core::BjutNet *core_bjut, QObject *parent):
    QSystemTrayIcon(parent),
    m_app(app),
    m_coreBjutNet(core_bjut),
    m_wndMain(Q_NULLPTR),
    m_wndSetting(Q_NULLPTR),
    m_wndHelp(Q_NULLPTR)
{
    if(m_vecBjutWeb.empty()){
        m_vecBjutWeb.append({1, QString("学校首页"), QUrl("http://www.bjut.edu.cn/"), 0});
        m_vecBjutWeb.append({2, QString("综合服务门户"), QUrl("https://my.bjut.edu.cn/"), 0});
        m_vecBjutWeb.append({3, QString("自助服务系统"), QUrl("https://jfself.bjut.edu.cn/"), 0});
        m_vecBjutWeb.append({4, QString("软件正版化平台"), QUrl("http://ca.bjut.edu.cn/"), 0});
        m_vecBjutWeb.append({5, QString("邮件系统"), QUrl("https://mail.bjut.edu.cn/"), 0});
        m_vecBjutWeb.append({6, QString("网关登陆页面"), QUrl("http://lgn.bjut.edu.cn/"), 0});
        m_vecBjutWeb.append({7, QString("研究生管理信息系统"), QUrl("http://webrecdoc.bjut.edu.cn/pyxx/"), 0});
        m_vecBjutWeb.append({8, QString("财务信息门户"), QUrl("http://172.27.12.88/WFManager/homesso.jsp"), 0});

        UISetting &uisettings = UISetting::getInstance();
        for(BjutWebItemInfo &t : m_vecBjutWeb){
            t.frequency = uisettings.getBjutWebFrequency(t.id);
        }
        std::stable_sort(m_vecBjutWeb.begin(), m_vecBjutWeb.end(),
                         [](const BjutWebItemInfo &a, const BjutWebItemInfo &b){
            return a.frequency > b.frequency;
        });
    }

    this->setToolTip("北工大校园网助手");
    QIcon ico(":/icon/logo.ico");
    this->setIcon(ico);

    m_menuTray = new QMenu("托盘菜单");
    //
#ifdef Q_OS_LINUX
    m_actMenuShowStatus = new QAction("当前网关状态");
#endif
    m_actMenuShowMain = new QAction("显示主窗口");
    m_actMenuLogin = new QAction("上线");
    m_actMenuLogout = new QAction("下线");
    m_actMenuLoginOnly = new QAction("上线不监控");
    m_actMenuStopMonitor = new QAction("停止在线监控");
    m_actMenuSetting = new QAction("设置");
    m_actMenuQuit = new QAction("退出");
    m_menuBjutWeb = new QMenu("常用网址");
#ifdef Q_OS_LINUX
    m_menuTray->addAction(m_actMenuShowStatus);
#endif
    m_menuTray->addAction(m_actMenuShowMain);
    m_menuTray->addMenu(m_menuBjutWeb);
    m_menuTray->addSeparator();
    m_menuTray->addAction(m_actMenuLogin);
    m_menuTray->addAction(m_actMenuLogout);
    m_menuTray->addAction(m_actMenuLoginOnly);
    m_menuTray->addAction(m_actMenuStopMonitor);
    m_menuTray->addSeparator();
    m_menuTray->addAction(m_actMenuSetting);
    m_menuTray->addAction(m_actMenuQuit);
    this->setContextMenu(m_menuTray);

    connect(this, &WndTrayIcon::activated, this, &WndTrayIcon::on_actived);
#ifdef Q_OS_LINUX
    connect(m_actMenuShowStatus, &QAction::triggered, this, &WndTrayIcon::on_clicked);
#endif
    connect(m_actMenuShowMain, &QAction::triggered, this, &WndTrayIcon::cmdShowMainWnd);
    connect(m_actMenuLogin, &QAction::triggered, this, &WndTrayIcon::cmdLoginAndStartMonitor);
    connect(m_actMenuLogout, &QAction::triggered, this, &WndTrayIcon::cmdLogoutAndStopMonitor);
    connect(m_actMenuLoginOnly, &QAction::triggered, this, &WndTrayIcon::cmdLoginOnly);
    connect(m_actMenuStopMonitor, &QAction::triggered, this, &WndTrayIcon::cmdStopMonitor);
    connect(m_actMenuSetting, &QAction::triggered, this, &WndTrayIcon::cmdShowSettingWnd);
    connect(m_actMenuQuit, &QAction::triggered, this, &WndTrayIcon::cmdExitApp);
    connect(m_menuBjutWeb, &QMenu::aboutToShow, this, &WndTrayIcon::initBjutWebMenu);
}

WndTrayIcon::~WndTrayIcon()
{
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
    core::WebLgn &lgn = m_coreBjutNet->getWebLgn();
    online = lgn.getLoginStatus();
    flow = lgn.getFlow();
    time = lgn.getTime();
    fee = lgn.getFee();
    float ftime = float(time) / 60;
    float fflow = float(flow);
    while(fflow > 1024)
    {
        fflow /= 1024;
        ++flowUnitIndex;
    }
    float ffee = float(fee) / 100;
    status.sprintf("%s线，已用时间：%.2f小时，已用流量：%.2f%s，剩余金额：%.2f元",
                   (online?"在":"离"), ftime, fflow, flowUnit[flowUnitIndex], ffee);
    //return status;
    this->showMessage("北工大校园网助手", status, QSystemTrayIcon::NoIcon);
}

void WndTrayIcon::openBjutWeb_action()
{
    try{
        QAction *action = static_cast<QAction*>(sender());
        if(action->data().type() == QVariant::UserType && action->data().canConvert<BjutWebItemInfo>()){
            BjutWebItemInfo info = action->data().value<BjutWebItemInfo>();
            if(info.url.isValid()){
                QDesktopServices::openUrl(info.url);
                increaseBjutWebFrequency(info.id);
                return;
            }
        }
    }catch(...){  }
    QMessageBox mb;
    mb.setWindowTitle("常用网址");
    mb.setText("无法识别的调用");
    mb.setStandardButtons(QMessageBox::Ok);
    mb.exec();
}

void WndTrayIcon::initBjutWebMenu()
{
    m_menuBjutWeb->clear();

    for(const BjutWebItemInfo &w : m_vecBjutWeb){
        QAction *act = new QAction(w.name);
        act->setData(QVariant::fromValue(w));
        m_menuBjutWeb->addAction(act);
        connect(act, &QAction::triggered, this, &WndTrayIcon::openBjutWeb_action);
    }

}

void WndTrayIcon::cmdShowMainWnd()
{
    if (!m_wndMain)
    {
        m_wndMain = new WndMain(this, nullptr);
    }
    m_wndMain->show();
    m_wndMain->activateWindow();
    m_wndMain->raise();
}

void WndTrayIcon::cmdShowSettingWnd()
{
    if(!m_wndSetting)
    {
        m_wndSetting = new WndSetting(this, nullptr);
    }
    m_wndSetting->show();
    m_wndSetting->activateWindow();
    m_wndSetting->raise();
}

void WndTrayIcon::cmdShowHelpWnd()
{
    if(!m_wndHelp){
        m_wndHelp = new WndHelp();
    }
    if(m_wndMain){
        m_wndHelp->move(m_wndMain->pos());
    }
    m_wndHelp->show();
    m_wndHelp->activateWindow();
    m_wndHelp->raise();
}

void WndTrayIcon::cmdExitApp()
{
    if(m_app != Q_NULLPTR)
    {
        m_app->quit();
    }
}

void WndTrayIcon::cmdExitAll()
{
    cmdExitApp();
}

void WndTrayIcon::cmdLoginAndStartMonitor()
{
    m_coreBjutNet->getWebLgn().login();
    m_coreBjutNet->start_monitor();
}

void WndTrayIcon::cmdLogoutAndStopMonitor()
{
    m_coreBjutNet->stop_monitor();
    m_coreBjutNet->getWebLgn().logout();
}

void WndTrayIcon::cmdLoginOnly()
{
    m_coreBjutNet->getWebLgn().login();
}

void WndTrayIcon::cmdLogoutOnly()
{
    m_coreBjutNet->getWebLgn().logout();
}

void WndTrayIcon::cmdStartMonitor()
{
    cmdLoginAndStartMonitor();
}

void WndTrayIcon::cmdStopMonitor()
{
    m_coreBjutNet->stop_monitor();
}

void WndTrayIcon::increaseBjutWebFrequency(int id, long freq_inc)
{
    for(auto &w : m_vecBjutWeb){
        if(w.id == id){
            w.frequency += freq_inc;
            UISetting::I().setBjutWebFrequency(id, w.frequency);
            break;
        }
    }
    std::stable_sort(m_vecBjutWeb.begin(), m_vecBjutWeb.end(),
                     [](const BjutWebItemInfo &a, const BjutWebItemInfo &b){
        return a.frequency > b.frequency;
    });
}

void WndTrayIcon::updateBjutWebFrequency(int id, long frequency)
{
    for(auto &w : m_vecBjutWeb){
        if(w.id == id){
            w.frequency = frequency;
            UISetting::I().setBjutWebFrequency(id, frequency);
            break;
        }
    }
    std::stable_sort(m_vecBjutWeb.begin(), m_vecBjutWeb.end(),
                     [](const BjutWebItemInfo &a, const BjutWebItemInfo &b){
        return a.frequency > b.frequency;
    });
}

void WndTrayIcon::on_actived(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    //case QSystemTrayIcon::Context:
    //    m_menuTray->exec();
    //    break;
    case QSystemTrayIcon::DoubleClick:
        cmdShowMainWnd();
        break;
    case QSystemTrayIcon::Trigger:
        on_clicked();
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
