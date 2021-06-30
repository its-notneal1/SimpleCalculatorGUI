#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <limits>
#include <iostream>

template<typename T> bool typeLimitReached(T);

// typedef double t_int;
double calcResult = 0.0;
bool multTrigger = false;
bool divTrigger = false;
bool addTrigger = false;
bool subTrigger = false;
bool powerTrigger = false;
bool moduloTrigger = false;

bool resultClear = false;

const int oprtArrSize = 6;
bool *OprtTriggers[oprtArrSize] = {&multTrigger, &divTrigger, &addTrigger, &subTrigger, &powerTrigger, &moduloTrigger}; // array of pointers

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->DisplayLine->setText(QString::number(calcResult)); // display the result or value we want

    QPushButton *numButtons[10];
    // loop to assign
    for(int i = 0; i < 10; i++) {
        QString buttonPressed_name = "Button_" + QString::number(i); // create the name of the button
        numButtons[i] = MainWindow::findChild<QPushButton *>(buttonPressed_name); //
        connect(numButtons[i], SIGNAL(released()), this, SLOT(OperandButton_Pressed())); // slot calls function
    }
    connect(ui->ClearButton, SIGNAL(released()), this, SLOT(ClearCalc()));
    QPushButton *oprtButtons[6];
    oprtButtons[0] = ui->Button_Multiply;
    oprtButtons[1] = ui->Button_Divide;
    oprtButtons[2] = ui->Button_Add;
    oprtButtons[3] = ui->Button_Minus;
    oprtButtons[4] = ui->Button_Exponent;
    oprtButtons[5] = ui->Button_Modulo;
    for(int i = 0; i < 6; i++) {
        connect(oprtButtons[i], SIGNAL(released()), this, SLOT(OperatorButton_Pressed()));
    }
    connect(ui->Button_Equals, SIGNAL(released()), this, SLOT(GetResult())); // connect the equal sign button (get the result)
    connect(ui->Button_Sign, SIGNAL(released()), this, SLOT(ChangeSign())); // connect the change sign button
    connect(ui->Button_Display_Delete, SIGNAL(released()), this, SLOT(DeleteChar()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::OperandButton_Pressed() {
    QPushButton *button = (QPushButton *)sender(); // return a pointer of the button that was pressed
    if(resultClear) {
        calcResult = 0.0; // reset calculated result back to 0
        ui->DisplayLine->setText("");
        resultClear = false;
    }
    QString buttonValue = button->text();
    QString displayValue = ui->DisplayLine->text();
    if((displayValue.toDouble() == 0) || (displayValue.toDouble() == 0.0)) {
        ui->DisplayLine->setText(buttonValue);
    } else {
        QString newVal = displayValue + buttonValue;
        double displayValue_double = newVal.toDouble();
        ui->DisplayLine->setText(QString::number(displayValue_double, 'g', 16));
    }
}


void MainWindow::OperatorButton_Pressed() {
    resultClear = false;
    for(int i = 0; i < oprtArrSize; i++) *OprtTriggers[i] = false; // nulify any previous operator pressed
    QString displayValue = ui->DisplayLine->text(); // DisplayLine->text() gives us the current text
    calcResult = displayValue.toDouble();  // convert the QString object into a double
    QPushButton *button = (QPushButton *)sender(); // No clue what this does
    QString oprtType = button->text();
    if(QString::compare(oprtType, "*") == 0) multTrigger = true;
    else if(QString::compare(oprtType, "/") == 0) divTrigger = true;
    else if(QString::compare(oprtType, "+") == 0) addTrigger = true;
    else if(QString::compare(oprtType, "-") == 0) subTrigger = true;
    else if(QString::compare(oprtType, "^") == 0) powerTrigger = true;
    else if(QString::compare(oprtType, "%") == 0) moduloTrigger = true;
    else {
        ClearCalc();
        ui->DisplayLine->setText("Unknown Error");
    }
    ui->PreviousOprndLabel->setText(QString::number(calcResult) + oprtType);
    ui->DisplayLine->setText("");
}

// Function if user enters a
void MainWindow::GetResult() {
    double result = 0.0;
    int oprtType = -1;
    QString displayValue_qstr = ui->DisplayLine->text();
    double displayValue_dbl = displayValue_qstr.toDouble();
    for(int i = 0; i < oprtArrSize; i++) {
        if(*OprtTriggers[i]) {
            oprtType = i;
            break;
        }
    }
    if(oprtType == -1) { // if invalid button clicks
        ClearCalc(); // clear the calculator
        ui->DisplayLine->setText("Error");
        return;
    }
    switch(oprtType) {
        case(0):
            result = calcResult * displayValue_dbl;
            break;
        case(1):
            result = calcResult / displayValue_dbl;
            break;
        case(2):
            result = calcResult + displayValue_dbl;
                break;
        case(3):
            result = calcResult - displayValue_dbl;
                break;
        case(4):
            result = calcResult;
            for(int i = 1; i < displayValue_dbl; i++) {
                   if(typeLimitReached(result)) break; // break the loop to prevent number overflow
                   result *= calcResult;
                }
        break;
        case(5):
            result = int(calcResult) % int(displayValue_dbl); // typecast back to integer
                break;
    }
    ui->DisplayLine->setText(QString::number(result, 'g', 16)); // 16 digits before number turns into exponent 'g' use any format
    ui->PreviousOprndLabel->setText(ui->PreviousOprndLabel->text() + displayValue_qstr);
    resultClear = true;
}

void MainWindow::ClearCalc() {
    calcResult = 0.0;
    ui->PreviousOprndLabel->setText("");
    ui->DisplayLine->setText(QString::number(0.0));
    for(int i = 0; i < oprtArrSize; i++) { // set everything back to false
        *OprtTriggers[i] = false;
    }
}

// purpose: avoid data (double / int) overflow
template<typename T>
bool typeLimitReached(T item) {
    T minimum = std::numeric_limits<T>::min() / 0.90;
    T maximum = std::numeric_limits<T>::max() / 0.90; // change it to 90% of the minimum or maximum
    if(item > maximum) return true;
    else if(item < minimum) return true;
    return false;
}

void MainWindow::ChangeSign() {
    QString displayValue_str = ui->DisplayLine->text();
    if(QString::compare(displayValue_str, "0.0", Qt::CaseInsensitive) == 0) return; // if input is 0
    if(QString::compare(displayValue_str, "0", Qt::CaseInsensitive) == 0) return;
    if(QString::compare(displayValue_str, "", Qt::CaseInsensitive) == 0) return; // if empty
    double displayValue_dbl = displayValue_str.toDouble();
    displayValue_dbl *= -1; // convert to negative
    ui->DisplayLine->setText(QString::number(displayValue_dbl));
}

void MainWindow::DeleteChar() {
    QString currentInput = ui->DisplayLine->text(); // get the text
    if(QString::compare(currentInput, "", Qt::CaseInsensitive) == 0) return;
    if(QString::compare(currentInput, "0.0", Qt::CaseInsensitive) == 0) return;
    if(QString::compare(currentInput, "0", Qt::CaseInsensitive) == 0) return;
    int lastIndex = currentInput.size() - 1;
    QString nInput;
    for(int i = 0; i < lastIndex; i++) {
        nInput.push_back(currentInput[i]);
    }
    ui->DisplayLine->setText(nInput);
}
