#ifndef WNDINIT_H
#define WNDINIT_H

#include <QProgressDialog>
#include <QTimer>
#include <QApplication>

class WndInit : public QProgressDialog
{

public:

    WndInit(QApplication *app);

public slots:

    void show();

    void cancel_init();

    void init_updater();

private:

    QTimer m_tmInit;

    bool m_bCancel;

    QString m_strBinName;

    QString m_strBinDir;

    QString m_strBinFile;

    QString m_strTmpDir;

    QString m_strTmpFile;

    QApplication *m_app;
};

#endif // WNDINIT_H
