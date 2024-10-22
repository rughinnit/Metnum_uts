#ifndef METNUM_UTS_H
#define METNUM_UTS_H

#include <QMainWindow>
#include <QJSEngine>

QT_BEGIN_NAMESPACE
namespace Ui {
class metnum_uts;
}
QT_END_NAMESPACE

class metnum_uts : public QMainWindow
{
    Q_OBJECT

public:
    metnum_uts(QWidget *parent = nullptr);
    ~metnum_uts();

private slots:
    void on_pushButton_clicked();

private:
    Ui::metnum_uts *ui;
    QJSEngine *m_engine;
    double evaluateFunction(const QString& functionString, double x);
    double evaluateDerivative(const QString& functionString, double x);
};
#endif // MAINWINDOW_H
