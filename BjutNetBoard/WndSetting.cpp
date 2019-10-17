#include "WndSetting.h"
#include <QApplication>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include "../BjutNetService/BjutNet.h"
#include "../BjutNetService/Setting.h"
#include "WndMain.h"
#include "WndTrayIcon.h"
#include "UISetting.h"

namespace bna{
namespace gui{

WndSetting::WndSetting(WndTrayIcon *tray, QWidget *parent) :
    QWidget(parent),
    m_tray(tray)
{
    initUI();
    QWidget::setAttribute(Qt::WA_QuitOnClose,false);
    connect(m_chkAutoRun, &QCheckBox::clicked, this, &WndSetting::on_chkAutoRun_clicked);
    connect(m_rdFlowGraphPie2D, &QRadioButton::clicked, this, &WndSetting::on_rdFlowGraphPie2D_clicked);
    connect(m_rdFlowGraphPie3D, &QRadioButton::clicked, this, &WndSetting::on_rdFlowGraphPie3D_clicked);
    connect(m_btnApply, &QPushButton::clicked, this, &WndSetting::on_btnApply_clicked);
    connect(m_btnApplyLogin, &QPushButton::clicked, this, &WndSetting::on_btnApplyLogin_clicked);
}

void WndSetting::show()
{
    QWidget::show();
    updateAccountInfo(m_tray->getBjutNet()->getAccount(),
               m_tray->getBjutNet()->getPassword(),
               m_tray->getBjutNet()->getLoginType());
    m_chkAutoRun->setChecked(bna::core::Setting::getAutoRun());
    m_rdFlowGraphPie2D->setChecked(UISetting::I().getFlowGraphType()==UISetting::FLOW_GRAPH_PIE_2D);
    m_rdFlowGraphPie3D->setChecked(UISetting::I().getFlowGraphType()==UISetting::FLOW_GRAPH_PIE_3D);
}

void WndSetting::on_chkAutoRun_clicked(bool checked)
{
    bna::core::Setting::setAutoRun(checked);
}

void WndSetting::on_chkDebug_clicked(bool checked)
{
    bna::g_bAppDebug = checked;
}

void WndSetting::on_rdFlowGraphPie2D_clicked(bool checked)
{
    if(checked){
        UISetting::I().setFlowGraphType(UISetting::FLOW_GRAPH_PIE_2D);
        m_tray->getMainWindow()->setFlowGraphType(UISetting::FLOW_GRAPH_PIE_2D);
        m_tray->getMainWindow()->update();
    }
}

void WndSetting::on_rdFlowGraphPie3D_clicked(bool checked)
{
    if(checked){
        UISetting::I().setFlowGraphType(UISetting::FLOW_GRAPH_PIE_3D);
        m_tray->getMainWindow()->setFlowGraphType(UISetting::FLOW_GRAPH_PIE_3D);
        m_tray->getMainWindow()->update();
    }
}

void WndSetting::on_btnApply_clicked()
{
    if(m_tray)
    {
        m_tray->getBjutNet()->setAccount(m_editAccount->text());
        m_tray->getBjutNet()->setPassword(m_editPassword->text());
        m_tray->getBjutNet()->setLoginType(m_cmbType->currentIndex()+1);
        m_tray->getBjutNet()->saveAccount();
        QMessageBox::information(this, "保存账号","账号信息已保存");
    }
    else
    {
        QMessageBox::information(this, "保存账号","账号信息未保存");
    }
}

void WndSetting::on_btnApplyLogin_clicked()
{
    on_btnApply_clicked();
    if(m_tray)
    {
        m_tray->cmdLoginAndStartMonitor();
    }
}

void WndSetting::updateAccountInfo(const QString &name, const QString &passwd, int type)
{
    m_editAccount->setText(name);
    m_editPassword->setText(passwd);
    m_cmbType->setCurrentIndex((1 <= type && type <= 3) ? type - 1 : 0);
}

}}
