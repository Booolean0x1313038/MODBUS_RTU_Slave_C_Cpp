#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../MODBUS_RTU_Slave.h"
#include <QDebug>
#include <QRegularExpression>
#include <QDateTime>
#include <QThread>

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
using namespace _Boolean::Communication;
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    MODBUS_RTU_Slave* _MODBUS_RTU_Slave;
    QByteArray _Frame;

    void ShowRegisterArea(int index);


    static const char *MODBUS_RTU_Slave_GetLastResultString(const MODBUS_RTU_Slave *modbus)
    {
        static const char *resultCodeStrings[MODBUS_RTU_RESULT_UNKNOWN + 1] = {
                                                                               "MODBUS_RTU_RESULT_OK",
                                                                               "MODBUS_RTU_RESULT_NOT_THIS_DEVICE",
                                                                               "MODBUS_RTU_RESULT_CRC_WRONG",
                                                                               "MODBUS_RTU_RESULT_FUNCTIONCODE_IS_NOT_SUPPORT",
                                                                               "MODBUS_RTU_RESULT_DATA_LENGTH_IS_INVALID",
                                                                               "MODBUS_RTU_RESULT_REGISTER_TYPE_IS_INCORRECT",
                                                                               "MODBUS_RTU_RESULT_REGISTER_NOT_EXIST",
                                                                               "MODBUS_RTU_RESULT_REGISTER_OUT_OF_RANGE",
                                                                               "MODBUS_RTU_RESULT_REGISTER_NOT_READABLE",
                                                                               "MODBUS_RTU_RESULT_REGISTER_NOT_WRITEABLE",
                                                                               "MODBUS_RTU_RESULT_BROADCAST_READ",
                                                                               "MODBUS_RTU_RESULT_WRITE_TIME_NOT_MATCH",
                                                                               "MODBUS_RTU_RESULT_WRITE_SINGLE_COIL_DATA_IS_INCORRECT",
                                                                               "MODBUS_RTU_RESULT_UNKNOWN"};
        return resultCodeStrings[modbus->_LastResult];
    }

#define LED_TOTAL_COUNT 18

#define MODBUS_COILEDREGISTERS 3

#define MODBUS_REGISTERS_DEVICE_AMOUNT 12
#define MODBUS_REGISTERS_DEVICE_FIRSTADDRESS 0
    MODBUS_RTU_Register_16Bits _Registers_Device[MODBUS_REGISTERS_DEVICE_AMOUNT];
    MODBUS_RTU_Register_Attribute _RegistersAttribute_Device[MODBUS_REGISTERS_DEVICE_AMOUNT];
#define MODBUS_REGISTER_SAVECONFIGURATION 7
#define MODBUS_REGISTER_DEVICEADDRESS 8
#define MODBUS_REGISTER_BAUD_MSB 9
#define MODBUS_REGISTER_BAUD_LSB 10
#define MODBUS_REGISTER_PARITY 11
#define MODBUS_REGISTER_STOPBITS 12

#define MODBUS_REGISTERS_GRADIENT_AMOUNT 63
#define MODBUS_REGISTERS_GRADIENT_FIRSTADDRESS 100
#ifdef USE_COIL
    MODBUS_RTU_Register_BIT _Registers_Gradient[MODBUS_REGISTERS_GRADIENT_AMOUNT];
#else
    MODBUS_RTU_Register_16Bits _Registers_Gradient[MODBUS_REGISTERS_GRADIENT_AMOUNT];
#endif
    MODBUS_RTU_Register_Attribute _RegistersAttribute_Gradient[MODBUS_REGISTERS_GRADIENT_AMOUNT];
#define MODBUS_REGISTERS_DIRECTCOLOR_AMOUNT (LED_TOTAL_COUNT << 1) + 1
#define MODBUS_REGISTERS_DIRECTCOLOR_FIRSTADDRESS 1000
    MODBUS_RTU_Register_16Bits _Registers_DirectColor[MODBUS_REGISTERS_DIRECTCOLOR_AMOUNT];
    MODBUS_RTU_Register_Attribute _RegistersAttribute_DirectColor[MODBUS_REGISTERS_DIRECTCOLOR_AMOUNT];

    MODBUS_RTU_ContiguousRegistersGroup _CoiledRegistersArray[MODBUS_COILEDREGISTERS];

private slots:
    void SLOT_PushButton_Prase_Clicked();
    void SLOT_UpdateFrame();
    void SLOT_LineEdit_Data_TextChanged(const QString &arg1);
    void SLOT_TableWidget_RegisterAreas_CurrentCellChanged(int newRow, int newColumn, int oldRow, int oldColumn);
};
#endif // MAINWINDOW_H
