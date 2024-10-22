#include "metnum_uts.h"
#include "ui_metnum_uts.h"
#include <QJSEngine>
#include <cmath>
#include <QDebug>
using namespace std;

metnum_uts::metnum_uts(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::metnum_uts)
    , m_engine(new QJSEngine(this))
{
    ui->setupUi(this);
    QJSValue mathObject = m_engine->globalObject().property("Math");
    m_engine->globalObject().setProperty("sin", mathObject.property("sin"));
    m_engine->globalObject().setProperty("cos", mathObject.property("cos"));
    m_engine->globalObject().setProperty("tan", mathObject.property("tan"));
    m_engine->globalObject().setProperty("exp", mathObject.property("exp"));
    m_engine->globalObject().setProperty("log", mathObject.property("log"));
    m_engine->globalObject().setProperty("pow", mathObject.property("pow"));
    m_engine->globalObject().setProperty("sqrt", mathObject.property("sqrt"));
    ui->comboBox->addItem("Metode Regula Falsi");
    ui->comboBox->addItem("Metode Newton Raphson");
}

metnum_uts::~metnum_uts()
{
    delete ui;
}

double metnum_uts::evaluateFunction(const QString& functionString, double x) {
    QString jsFunction = QString("(function(x) { return %1; })").arg(functionString);
    QJSValue function = m_engine->evaluate(jsFunction);

    if (function.isError()) {
        qDebug() << "Error in function:" << function.toString();
        return std::numeric_limits<double>::quiet_NaN();
    }

    QJSValueList args;
    args << x;
    QJSValue result = function.call(args);

    if (result.isError()) {
        qDebug() << "Error evaluating function:" << result.toString();
        return std::numeric_limits<double>::quiet_NaN();
    }

    return result.toNumber();
}

double metnum_uts::evaluateDerivative(const QString& functionString, double x) {
    const double h = 1e-7; // Small step for numerical derivative
    double fx1 = evaluateFunction(functionString, x + h);
    double fx2 = evaluateFunction(functionString, x - h);
    return (fx1 - fx2) / (2 * h);
}

void metnum_uts::on_pushButton_clicked()
{
    ui->plainTextEdit->clear();

    QString selectedmetode = ui->comboBox->currentText();
    QString functionString = ui->Function->text();
    QString errorString = ui->Error->text();
    QString initAString = ui->InitA->text();
    QString initBString = ui->InitB->text();

    if(selectedmetode == "Metode Regula Falsi") {
        bool ok;
        double errorTolerance = errorString.toDouble(&ok);
        if (!ok) {
            ui->plainTextEdit->appendPlainText("Error Salah");
            return;
        }

        double a = initAString.toDouble(&ok);
        if (!ok) {
            ui->plainTextEdit->appendPlainText("Init A salah");
            return;
        }

        double b = initBString.toDouble(&ok);
        if (!ok) {
            ui->plainTextEdit->appendPlainText("Init B salah");
            return;
        }

        double fa = evaluateFunction(functionString, a);
        double fb = evaluateFunction(functionString, b);

        if (isnan(fa) || isnan(fb)) {
            ui->plainTextEdit->appendPlainText("bukan angka");
            return;
        }

        if (fa * fb >= 0) {
            ui->plainTextEdit->appendPlainText("fa * fb tidak >= 0");
            return;
        }

        double c = a;
        int iteration = 0;
        const int MAX_ITERATIONS = 1000;

        while (iteration < MAX_ITERATIONS) {
            c = (a * fb - b * fa) / (fb - fa);
            double fc = evaluateFunction(functionString, c);

            ui->plainTextEdit->appendPlainText(QString("Iteration %1: c = %2").arg(iteration).arg(c, 0, 'f', 6));

            if (fabs(fc) < errorTolerance) {
                break;
            }

            if (fc * fa < 0) {
                b = c;
                fb = fc;
            } else {
                a = c;
                fa = fc;
            }

            iteration++;
        }

        if (iteration == MAX_ITERATIONS) {
            ui->plainTextEdit->appendPlainText("Max iteration");
        } else {
            ui->plainTextEdit->appendPlainText(QString("Root found at: %1").arg(c, 0, 'f', 6));
        }
    }
    if(selectedmetode == "Metode Newton Raphson") {
        bool ok;
        double errorTolerance = errorString.toDouble(&ok);
        if (!ok) {
            ui->plainTextEdit->appendPlainText("Error Salah");
            return;
        }

        double x0 = initAString.toDouble(&ok);  // We'll use initA as initial guess
        if (!ok) {
            ui->plainTextEdit->appendPlainText("Initial guess salah");
            return;
        }

        double x = x0;
        int iteration = 0;
        const int MAX_ITERATIONS = 1000;

        while (iteration < MAX_ITERATIONS) {
            double fx = evaluateFunction(functionString, x);
            double dfx = evaluateDerivative(functionString, x);

            if (std::fabs(dfx) < 1e-10) {
                ui->plainTextEdit->appendPlainText("Derivative too close to zero");
                return;
            }

            double x_new = x - fx/dfx;

            ui->plainTextEdit->appendPlainText(
                QString("Iteration %1: x = %2, f(x) = %3")
                    .arg(iteration)
                    .arg(x_new, 0, 'f', 6)
                    .arg(fx, 0, 'f', 6)
                );

            if (std::fabs(x_new - x) < errorTolerance) {
                ui->plainTextEdit->appendPlainText(
                    QString("Root found at: %1").arg(x_new, 0, 'f', 6)
                    );
                return;
            }

            x = x_new;
            iteration++;
        }

        if (iteration >= MAX_ITERATIONS) {
            ui->plainTextEdit->appendPlainText("Maximum iterations reached");
        }
    }
}
