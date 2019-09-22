#ifndef BNA_GUI_WNDHELP_H
#define BNA_GUI_WNDHELP_H

#include <QWidget>
#include "HLabel.h"

namespace bna{
namespace gui{

class WndHelp : public QWidget
{
    Q_OBJECT
public:
    explicit WndHelp(QWidget *parent = nullptr);

signals:

public slots:
    void on_lblImage_clicked();
private:
    HLabel *m_lblImage;
};

}}
#endif // BNA_GUI_WNDHELP_H
