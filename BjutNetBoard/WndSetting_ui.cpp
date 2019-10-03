#include "WndSetting.h"
#include "HPanel.h"
#include <QFrame>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>

void bna::gui::WndSetting::initUI()
{
    QFont font_s11;
    font_s11.setPointSize(11);
    QFont font_s12;
    font_s12.setPointSize(12);

    this->setObjectName(QStringLiteral("WndSetting"));
    //设置固定窗口大小
    this->setFixedSize(300, 410);
    this->setFont(font_s11);

    m_frmBasic = new HPanel(this);
    m_frmBasic->setGeometry(QRect(10, 10, 280, 90));
    m_frmBasic->setFrameShape(QFrame::NoFrame);
    m_frmBasic->setFrameShadow(QFrame::Plain);
    m_chkAutoRun = new QCheckBox(m_frmBasic);
    m_chkAutoRun->setGeometry(QRect(20, 25, 250, 20));
    m_chkAppDebug = new QCheckBox(m_frmBasic);
    m_chkAppDebug->setGeometry(QRect(20, 55, 250, 20));

    // grapgic user interface
    m_frmGUI = new HPanel(this);
    m_frmGUI->setGeometry(QRect(10, 5+m_frmBasic->geometry().bottom(),
                                    280, 85));
    m_frmFlowGraph = new HPanel(m_frmGUI);
    m_frmFlowGraph->setGeometry(10, 22, m_frmGUI->width()-20, 50);
    m_rdFlowGraphPie2D = new QRadioButton(m_frmFlowGraph);
    m_rdFlowGraphPie2D->setGeometry(10, 20, 120, 22);
    m_rdFlowGraphPie3D = new QRadioButton(m_frmFlowGraph);
    m_rdFlowGraphPie3D->setGeometry(m_rdFlowGraphPie2D->geometry().right()+10, 20, 120, 22);

    //账号信息
    m_frmAccount = new HPanel(this);
    m_frmAccount->setGeometry(QRect(10, 5+m_frmGUI->geometry().bottom(),
                                    280, 185));
    m_frmAccount->setFrameShape(QFrame::NoFrame);
    m_frmAccount->setFrameShadow(QFrame::Plain);
    m_lblAccount = new QLabel(m_frmAccount);
    m_lblAccount->setGeometry(QRect(20, 30, 50, 20));
    m_lblPassword = new QLabel(m_frmAccount);
    m_lblPassword->setGeometry(QRect(20, 70, 50, 20));
    m_lblType = new QLabel(m_frmAccount);
    m_lblType->setGeometry(QRect(20, 110, 50, 20));
    m_editAccount = new QLineEdit(m_frmAccount);
    m_editAccount->setGeometry(QRect(65, 30, 195, 25));
    m_editPassword = new QLineEdit(m_frmAccount);
    m_editPassword->setGeometry(QRect(65, 70, 195, 25));
    m_editPassword->setEchoMode(QLineEdit::Password);
    m_cmbType = new QComboBox(m_frmAccount);
    m_cmbType->setGeometry(QRect(65, 110, 195, 25));
    m_cmbType->setFont(font_s12);
    m_cmbType->addItems({"IPv4", "IPv6", "IPv4+IPv6"});
    m_cmbType->setCurrentIndex(2);
    //应用上线
    m_btnApplyLogin = new QPushButton(m_frmAccount);
    m_btnApplyLogin->setGeometry(QRect(155, 145, 90, 30));
    //应用按钮
    m_btnApply = new QPushButton(m_frmAccount);
    m_btnApply->setGeometry(QRect(35, 145, 90, 30));
    m_btnApply->setFlat(false);

    m_lblURL = new QLabel(this);
    m_lblURL->setGeometry(QRect(10, this->height()-30, this->width(), 30));
    m_lblURL->setOpenExternalLinks(true);
    m_lblURL->setText("官方网址：<a href=\"http://bnl.hrrcn.com/\">http://bnl.hrrcn.com</a>");

    this->setWindowTitle(QStringLiteral("设置"));
    m_frmBasic->setText(QStringLiteral("基本"));
    m_chkAutoRun->setText(QStringLiteral("开机自动运行"));
    m_chkAppDebug->setText(QStringLiteral("调试模式(请勿开启)"));
    m_frmGUI->setText("界面");
    m_frmFlowGraph->setText("流量图");
    m_rdFlowGraphPie2D->setText("平面饼状图");
    m_rdFlowGraphPie3D->setText("立体饼状图");
    m_frmAccount->setText("账号信息");
    m_lblAccount->setText(QString("账号："));
    m_lblPassword->setText(QString("密码："));
    m_lblType->setText(QString("类型："));
    m_btnApplyLogin->setText(QString("立即上线"));
    m_btnApply->setText(QString("仅保存"));
//    m_btnOK->setText(QStringLiteral("确定"));
//    m_btnCancel->setText(QStringLiteral("取消"));
}
