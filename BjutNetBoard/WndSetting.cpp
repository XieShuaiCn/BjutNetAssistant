#include "WndSetting.h"
#include <QApplication>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QSettings>
#include "WndTrayIcon.h"
#include "BjutNet.h"

using namespace bna;
using namespace bna::gui;

WndSetting::WndSetting(WndTrayIcon *tray, QWidget *parent) :
    QWidget(parent),
    m_tray(tray)
{
    initUI();
    QWidget::setAttribute(Qt::WA_QuitOnClose,false);
    auto *service = m_tray->getBjutNet();
    connect(m_chkAutoRun, &QCheckBox::clicked, this, &WndSetting::on_chkAutoRun_clicked);
    connect(m_btnApply, &QPushButton::clicked, this, &WndSetting::on_btnApply_clicked);
    connect(m_btnApplyLogin, &QPushButton::clicked, this, &WndSetting::on_btnApplyLogin_clicked);
    connect(service, &BjutNet::updateAccount, this, &WndSetting::on_account);
    connect(service, &BjutNet::updateAutoStart, this, &WndSetting::on_autoStart);
}

void WndSetting::show()
{
    QWidget::show();
    auto *service = m_tray->getBjutNet();
    service->requireIsAutoStart();
    service->requireAccount();
}

void WndSetting::on_chkAutoRun_clicked(bool checked)
{
    BjutNet *service = m_tray->getBjutNet();
    service->sendAutoStart(checked);
    service->requireIsAutoStart();
}

void WndSetting::on_chkDebug_clicked(bool checked)
{
    Q_UNUSED(checked);
}

void WndSetting::on_btnApply_clicked()
{
    if(m_tray)
    {
        auto *service = m_tray->getBjutNet();
        service->sendNewAccount(m_editAccount->text(), m_editPassword->text(), m_cmbType->currentIndex()+1);
    }
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

void WndSetting::on_account(const QString &name, const QString &passwd, int type)
{
    m_editAccount->setText(name);
    m_editPassword->setText(passwd);
    m_cmbType->setCurrentIndex((1 <= type && type <= 3) ? type - 1 : 0);
}
//
void WndSetting::on_autoStart(bool autostart)
{
    m_chkAutoRun->setChecked(autostart);
}
