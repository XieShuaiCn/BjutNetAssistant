#include "WndSetting.h"
#include <QApplication>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QSettings>
#include "../BjutNetService/BjutNet.h"
#include "../BjutNetService/Setting.h"
#include "WndTrayIcon.h"

using namespace bna;
using namespace bna::gui;

WndSetting::WndSetting(WndTrayIcon *tray, QWidget *parent) :
    QWidget(parent),
    m_tray(tray)
{
    initUI();
    QWidget::setAttribute(Qt::WA_QuitOnClose,false);
    connect(m_chkAutoRun, &QCheckBox::clicked, this, &WndSetting::on_chkAutoRun_clicked);
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
}

void WndSetting::on_chkAutoRun_clicked(bool checked)
{
    bna::core::Setting::setAutoRun(checked);
}

void WndSetting::on_chkDebug_clicked(bool checked)
{
    bna::g_bAppDebug = checked;
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
        m_tray->cmdLoginLgn();
    }
}

void WndSetting::updateAccountInfo(const QString &name, const QString &passwd, int type)
{
    m_editAccount->setText(name);
    m_editPassword->setText(passwd);
    m_cmbType->setCurrentIndex((1 <= type && type <= 3) ? type - 1 : 0);
}
