#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    // private slot functions need implementation or the compilation will fail
    void OperandButton_Pressed();
    void OperatorButton_Pressed();
    void GetResult(); // if the equal sign is pressed
    void ClearCalc();
    void ChangeSign();
    void DeleteChar();
};
#endif // MAINWINDOW_H
