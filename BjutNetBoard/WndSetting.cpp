#include "WndSetting.h"
#include "WndTrayIcon.h"
#include <QApplication>
#include <QMessageBox>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QSettings>

namespace bna{

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
    bool bAutoRun = false;
    auto *service = m_tray->getService();
    service->sendGetAutoStart(bAutoRun);
    m_chkAutoRun->setChecked(bAutoRun);

    if(m_tray)
    {
        QString name;
        QString passwd;
        int type;
        service->sendGetAccount(name, passwd, type);
        m_editAccount->setText(name);
        m_cmbType->setCurrentIndex(type-1);
    }
}

void WndSetting::on_chkAutoRun_clicked(bool checked)
{
    bool bAutoRun = false;
    auto *service = m_tray->getService();
    service->sendSetAutoStart(checked);
    service->sendGetAutoStart(bAutoRun);
    m_chkAutoRun->setChecked(bAutoRun);
}

void WndSetting::on_chkDebug_clicked(bool checked)
{
    Q_UNUSED(checked);
}

void WndSetting::on_btnApply_clicked()
{
    if(m_tray)
    {
        auto *service = m_tray->getService();
        bool succ = service->sendSetAccount(m_editAccount->text(), m_editPassword->text(), m_cmbType->currentIndex()+1);
        if(succ)
        {
            QMessageBox::information(this, "保存账号","账号信息保存成功");
            return;
        }
    }
    QMessageBox::information(this, "保存账号","账号信息未保存");
}

void WndSetting::on_btnApplyLogin_clicked()
{
    on_btnApply_clicked();
    if(m_tray)
    {
        m_tray->cmdLoginLgn();
    }
}

}
