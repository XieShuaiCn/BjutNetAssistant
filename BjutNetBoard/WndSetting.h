#ifndef WNDSETTING_H
#define WNDSETTING_H

#include "common.h"
#include <QWidget>

class QWidget;
class QFrame;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QCheckBox;
class QLabel;
class QComboBox;

namespace bna{

class WndTrayIcon;
class HPanel;

class WndSetting : public QWidget
{
    Q_OBJECT
public:
    explicit WndSetting(WndTrayIcon *tray, QWidget *parent = Q_NULLPTR);

signals:

public slots:
    void initUI();
    void show();
    void on_chkAutoRun_clicked(bool checked);
    void on_chkDebug_clicked(bool checked);
    void on_btnApply_clicked();
    void on_btnApplyLogin_clicked();

private:
    WndTrayIcon *m_tray;
    HPanel *m_frmBasic;
    QCheckBox *m_chkAutoRun;
    QCheckBox *m_chkAppDebug;
    QLabel *m_lblAutoRun;

    QFrame *m_frmAccount;
    QLabel *m_lblAccount;
    QLabel *m_lblPassword;
    QLabel *m_lblType;
    QLineEdit *m_editAccount;
    QLineEdit *m_editPassword;
    QComboBox *m_cmbType;
    QPushButton *m_btnApply;
    QPushButton *m_btnApplyLogin;

    QLabel *m_lblURL;

    //QPushButton *m_btnOK;
    //QPushButton *m_btnCancel;
};

}

#endif // WNDSETTING_H
