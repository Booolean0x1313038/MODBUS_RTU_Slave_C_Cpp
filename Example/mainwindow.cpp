#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    typedef struct
    {
        uint8_t Value0;
        uint8_t Value1;
        uint8_t Value2;
        uint8_t Value3;
    } UInt8_4;
    uint32_t value_UInt32 = 0x44332211;
    UInt8_4 uint8_4;
    uint8_4 = *(UInt8_4 *)&value_UInt32;
    qDebug() << QString("%1").arg(value_UInt32, 8, 16, '0');
    qDebug() << QString("%1%2%3%4").arg(uint8_4.Value0, 2, 16, '0').arg(uint8_4.Value1, 2, 16, '0').arg(uint8_4.Value2, 2, 16, '0').arg(uint8_4.Value3, 2, 16, '0');

#ifndef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
    _MODBUS_RTU_Slave = new MODBUS_RTU_Slave();
    MODBUS_RTU_Slave_Initialize(_MODBUS_RTU_Slave, 1, _CoiledRegistersArray, MODBUS_COILEDREGISTERS);
#else
    _MODBUS_RTU_Slave = new MODBUS_RTU_Slave(1, _CoiledRegistersArray, MODBUS_COILEDREGISTERS);
#endif
    connect(ui->pushButton_Prase, SIGNAL(clicked(bool)), this, SLOT(SLOT_PushButton_Prase_Clicked()));
    connect(ui->lineEdit_Data, SIGNAL(textChanged(QString)), this, SLOT(SLOT_LineEdit_Data_TextChanged(QString)));
    connect(ui->tableWidget_RegistersAreas, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(SLOT_TableWidget_RegisterAreas_CurrentCellChanged(int,int,int,int)));

    connect(ui->spinBox_DeviceAddress, SIGNAL(valueChanged(int)), this, SLOT(SLOT_UpdateFrame()));
    connect(ui->comboBox_FunctionCode, SIGNAL(currentIndexChanged(int)), this, SLOT(SLOT_UpdateFrame()));
    connect(ui->spinBox_RegisterAddress, SIGNAL(valueChanged(int)), this, SLOT(SLOT_UpdateFrame()));
    connect(ui->spinBox_Amount, SIGNAL(valueChanged(int)), this, SLOT(SLOT_UpdateFrame()));
    connect(ui->lineEdit_Data, SIGNAL(textChanged(QString)), this, SLOT(SLOT_UpdateFrame()));
    SLOT_UpdateFrame();

    ui->tableWidget_Result->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_Result->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget_Registers->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_Registers->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget_Registers->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget_Registers->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWidget_RegistersAreas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

#pragma region { // 初始化寄存器
    memset(_Registers_Device, 0, sizeof(_Registers_Device));
    memset(_RegistersAttribute_Device, MODBUS_RTU_REGISTER_ATTRIBUTE_READABLE, 7);
    _Registers_Device[6] = LED_TOTAL_COUNT;
    _RegistersAttribute_Device[MODBUS_REGISTER_SAVECONFIGURATION] = MODBUS_RTU_REGISTER_ATTRIBUTE_WRITABLE;
    memset(_RegistersAttribute_Device + MODBUS_REGISTER_DEVICEADDRESS, MODBUS_RTU_REGISTER_ATTRIBUTE_READABLE | MODBUS_RTU_REGISTER_ATTRIBUTE_WRITABLE, MODBUS_REGISTERS_DEVICE_AMOUNT - MODBUS_REGISTER_DEVICEADDRESS);
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
    _Boolean::Communication::MODBUS_RTU_Slave::InitializeContiguousRegistersGroup(
#else
    MODBUS_RTU_Slave_InitializeContiguousRegistersGroup(
#endif
        _CoiledRegistersArray, _Registers_Device, MODBUS_REGISTERS_DEVICE_AMOUNT, _RegistersAttribute_Device, MODBUS_REGISTERS_DEVICE_FIRSTADDRESS
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
                                                                                  ,2
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL

                                                                                  ,MODBUS_RTU_REGISTER_TYPE_UINT16
#endif
                                                                                  );

    memset(_Registers_Gradient, 0, sizeof(_Registers_Gradient));
    _RegistersAttribute_Gradient[0] = MODBUS_RTU_REGISTER_ATTRIBUTE_WRITABLE;
    memset(_RegistersAttribute_Gradient + 1, MODBUS_RTU_REGISTER_ATTRIBUTE_READABLE | MODBUS_RTU_REGISTER_ATTRIBUTE_WRITABLE, MODBUS_REGISTERS_GRADIENT_AMOUNT - 1);
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
    _Boolean::Communication::MODBUS_RTU_Slave::InitializeContiguousRegistersGroup(
#else
    MODBUS_RTU_Slave_InitializeContiguousRegistersGroup(
#endif
        _CoiledRegistersArray + 1, _Registers_Gradient, MODBUS_REGISTERS_GRADIENT_AMOUNT, _RegistersAttribute_Gradient, MODBUS_REGISTERS_GRADIENT_FIRSTADDRESS
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
                                                                                  ,1
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL

                                                                                  ,MODBUS_RTU_REGISTER_TYPE_BIT
#endif
                                                                                  );

    memset(_Registers_DirectColor, 0, sizeof(_Registers_DirectColor));
    _RegistersAttribute_DirectColor[0] = MODBUS_RTU_REGISTER_ATTRIBUTE_WRITABLE;
    memset(_RegistersAttribute_DirectColor + 1, MODBUS_RTU_REGISTER_ATTRIBUTE_READABLE | MODBUS_RTU_REGISTER_ATTRIBUTE_WRITABLE, MODBUS_REGISTERS_DIRECTCOLOR_AMOUNT - 1);

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
    _Boolean::Communication::MODBUS_RTU_Slave::InitializeContiguousRegistersGroup(
#else
    MODBUS_RTU_Slave_InitializeContiguousRegistersGroup(
#endif
        _CoiledRegistersArray + 2, _Registers_DirectColor, MODBUS_REGISTERS_DIRECTCOLOR_AMOUNT, _RegistersAttribute_DirectColor, MODBUS_REGISTERS_DIRECTCOLOR_FIRSTADDRESS
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
                                                                                  ,1
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL

                                                                                  ,MODBUS_RTU_REGISTER_TYPE_UINT16
#endif
                                                                                  );

#pragma endregion }
    ui->tableWidget_RegistersAreas->setRowCount(MODBUS_COILEDREGISTERS);
    ui->tableWidget_RegistersAreas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for (int i = 0; i < MODBUS_COILEDREGISTERS; ++i)
    {
        QTableWidgetItem* col0 = new QTableWidgetItem(QString("0x%1  %2").arg(QString("%1").arg(_CoiledRegistersArray[i].FirstRegisterAddress, 4, 16, '0').toUpper()).arg(_CoiledRegistersArray[i].FirstRegisterAddress, 4));
        QTableWidgetItem* col1 = new QTableWidgetItem(QString("0x%1 %2").arg(QString("%1").arg(_CoiledRegistersArray[i].Bytes, 4, 16, '0').toUpper()).arg(_CoiledRegistersArray[i].Bytes, 4));
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
        QTableWidgetItem* col3 = new QTableWidgetItem(QString::number(_CoiledRegistersArray[i].WriteTimes));
        col3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget_RegistersAreas->setItem(i, 3, col3);
#endif
        col0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        col1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget_RegistersAreas->setItem(i, 0, col0);
        ui->tableWidget_RegistersAreas->setItem(i, 1, col1);
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
        QTableWidgetItem* col2 = new QTableWidgetItem(_CoiledRegistersArray[i].RegisterType == MODBUS_RTU_REGISTER_TYPE_UINT16 ? "UINT16" : "1Bit");
        col2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget_RegistersAreas->setItem(i, 2, col2);
#endif
    }
    ui->tableWidget_RegistersAreas->setCurrentItem(ui->tableWidget_RegistersAreas->item(0, 0));
}

void MainWindow::SLOT_PushButton_Prase_Clicked()
{
    ui->lineEdit_ReturnFrame->setText("");
    ui->tableWidget_Result->clearContents();
    uint16_t timeSpanMS = 0;
    QDateTime time_Start = QDateTime::currentDateTime();
    if (
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
        _MODBUS_RTU_Slave->PraseHeader((unsigned char*)_Frame.constData(), _Frame.length()))
#else
        MODBUS_RTU_Slave_PraseHeader(_MODBUS_RTU_Slave,(unsigned char*)_Frame.constData(), _Frame.length()))
#endif
    {
        timeSpanMS += time_Start.msecsTo(QDateTime::currentDateTime());
        ui->tableWidget_Result->setRowCount(11);
        QTableWidgetItem* resultKey = new QTableWidgetItem("Header 检查结果");
        resultKey->setBackground(QColor(0x00, 0xFF, 0x00, 0x1F));
        ui->tableWidget_Result->setItem(0, 0, resultKey);
        QTableWidgetItem* resultValue = new QTableWidgetItem("通过");
        resultValue->setBackground(QColor(0x00, 0xFF, 0x00, 0x1F));
        ui->tableWidget_Result->setItem(0, 1, resultValue);
        ui->tableWidget_Result->setItem(1, 0, new QTableWidgetItem("FrameLength                 帧长度"));
        ui->tableWidget_Result->setItem(1, 1, new QTableWidgetItem("0x" + QString("%1 %2").arg(_Frame.length(), 4, 16, '0').arg(_Frame.length(), 4).toUpper()));
        ui->tableWidget_Result->setItem(2, 0, new QTableWidgetItem("DeviceAddress               设备地址"));
        ui->tableWidget_Result->setItem(2, 1, new QTableWidgetItem("0x" + QString("%1 %2").arg(
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
                                                                                              _MODBUS_RTU_Slave->GetDeviceAddress()
#else
                                                                                              MODBUS_RTU_Slave_GetDeviceAddress(_MODBUS_RTU_Slave)
#endif
                                                                                              , 4, 16, '0').arg(
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
                                                                                  _MODBUS_RTU_Slave->GetDeviceAddress()
#else
                                                                                  MODBUS_RTU_Slave_GetDeviceAddress(_MODBUS_RTU_Slave)
#endif
                                                                                  , 4).toUpper()));
        ui->tableWidget_Result->setItem(3, 0, new QTableWidgetItem("FunctionCode                功能码"));
        ui->tableWidget_Result->setItem(3, 1, new QTableWidgetItem("0x" + QString("%1 %2").arg(_MODBUS_RTU_Slave->_Prase_RequestFrame_FunctionCode, 4, 16, '0').arg(_MODBUS_RTU_Slave->_Prase_RequestFrame_FunctionCode, 4).toUpper()));
        ui->tableWidget_Result->setItem(4, 0, new QTableWidgetItem("RegisterAddress             寄存器地址"));
        ui->tableWidget_Result->setItem(4, 1, new QTableWidgetItem("0x" + QString("%1 %2").arg(_MODBUS_RTU_Slave->_Prase_RequestFrame_RegisterAddress, 4, 16, '0').arg(_MODBUS_RTU_Slave->_Prase_RequestFrame_RegisterAddress, 4).toUpper()));
        ui->tableWidget_Result->setItem(5, 0, new QTableWidgetItem("RegisterAmount              寄存器数量"));
        ui->tableWidget_Result->setItem(5, 1, new QTableWidgetItem("0x" + QString("%1 %2").arg(_MODBUS_RTU_Slave->_Prase_RequestFrame_RequestAmount, 4, 16, '0').arg(_MODBUS_RTU_Slave->_Prase_RequestFrame_RequestAmount, 4).toUpper()));
        ui->tableWidget_Result->setItem(6, 0, new QTableWidgetItem("RequestFrameLength          帧期望长度"));
        ui->tableWidget_Result->setItem(6, 1, new QTableWidgetItem("0x" + QString("%1 %2").arg(_MODBUS_RTU_Slave->_Prase_Calculate_RequestFrameBytes, 4, 16, '0').arg(_MODBUS_RTU_Slave->_Prase_Calculate_RequestFrameBytes, 4).toUpper()));
        ui->tableWidget_Result->setItem(7, 0, new QTableWidgetItem("ReturnFrameLength           返回帧长度"));
        ui->tableWidget_Result->setItem(7, 1, new QTableWidgetItem("0x" + QString("%1 %2").arg(

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
                                                                                              _MODBUS_RTU_Slave->GetReturnFrameLength()
#else
                                                                                              MODBUS_RTU_Slave_GetReturnFrameLength(_MODBUS_RTU_Slave)
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
#endif
                                                                                              , 4, 16, '0').arg(

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
                                                                                  _MODBUS_RTU_Slave->GetReturnFrameLength()
#else
                                                                                  MODBUS_RTU_Slave_GetReturnFrameLength(_MODBUS_RTU_Slave)
#endif
                                                                                  , 4).toUpper()));

        ui->tableWidget_Result->setItem(8, 0, new QTableWidgetItem("TargetRegisterOffsetAddress 目标寄存器偏移地址"));
        ui->tableWidget_Result->setItem(8, 1, new QTableWidgetItem("0x" + QString("%1 %2").arg(_MODBUS_RTU_Slave->_Prase_Calculate_TargetRegistersMemoryAddressOffset, 4, 16, '0').arg(_MODBUS_RTU_Slave->_Prase_Calculate_TargetRegistersMemoryAddressOffset, 4).toUpper()));
        ui->tableWidget_Result->setItem(9, 0, new QTableWidgetItem("TargetRegistersBytes        目标字节数"));
        ui->tableWidget_Result->setItem(9, 1, new QTableWidgetItem("0x" + QString("%1 %2").arg(_MODBUS_RTU_Slave->_Prase_Calculate_TargetRegistersBytes, 4, 16, '0').arg(_MODBUS_RTU_Slave->_Prase_Calculate_TargetRegistersBytes, 4).toUpper()));
        ui->tableWidget_Result->setItem(10, 0, new QTableWidgetItem("BitOffset                   位偏移"));
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
        ui->tableWidget_Result->setItem(10, 1, new QTableWidgetItem("0x" + QString("%1 %2").arg(_MODBUS_RTU_Slave->_Prase_Calculate_TargetRegistersBitOffset, 4, 16, '0').arg(_MODBUS_RTU_Slave->_Prase_Calculate_TargetRegistersBitOffset, 4).toUpper()));
#endif

        int startRow = ui->tableWidget_Result->rowCount();
        time_Start = QDateTime::currentDateTime();
        if (
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
            _MODBUS_RTU_Slave->ProcessData(
#else
            MODBUS_RTU_Slave_ProcessData(_MODBUS_RTU_Slave,
#endif
                (unsigned char*)_Frame.constData(), _Frame.length()))
        {
            timeSpanMS += time_Start.msecsTo(QDateTime::currentDateTime());
            ui->tableWidget_Result->setRowCount(startRow + 2);
            QTableWidgetItem* resultKey = new QTableWidgetItem("操作结果");
            resultKey->setBackground(QColor(0x00, 0xFF, 0x00, 0x1F));
            ui->tableWidget_Result->setItem(startRow, 0, resultKey);
            QTableWidgetItem* resultValue = new QTableWidgetItem("成功");
            resultValue->setBackground(QColor(0x00, 0xFF, 0x00, 0x1F));
            ui->tableWidget_Result->setItem(startRow, 1, resultValue);
            ui->tableWidget_Result->setItem(startRow + 1, 0, new QTableWidgetItem("Time Elapsed                耗时"));
            ui->tableWidget_Result->setItem(startRow + 1, 1, new QTableWidgetItem(QString("%1 ms").arg(timeSpanMS)));
            for (int i = 0; i < MODBUS_COILEDREGISTERS; ++i)
            {
                if (_MODBUS_RTU_Slave->_Prase_Calculate_TargetCoiledRegistersAreaIndex == i)
                {
                    ui->tableWidget_RegistersAreas->setCurrentCell(i, 0);
                    ShowRegisterArea(i);
                }
            }
        }
        else
        {
            timeSpanMS += time_Start.msecsTo(QDateTime::currentDateTime());
            ui->tableWidget_Result->setRowCount(startRow + 3);
            QTableWidgetItem* resultKey = new QTableWidgetItem("操作结果");
            resultKey->setBackground(QColor(0xFF, 0x00, 0x00, 0x1F));
            ui->tableWidget_Result->setItem(startRow, 0, resultKey);
            QTableWidgetItem* resultValue = new QTableWidgetItem("未完成");
            resultValue->setBackground(QColor(0xFF, 0x00, 0x00, 0x1F));
            ui->tableWidget_Result->setItem(startRow, 1, resultValue);
            ui->tableWidget_Result->setItem(startRow + 1, 0, new QTableWidgetItem("ERROR CODE                  错误代码"));
            ui->tableWidget_Result->setItem(startRow + 1, 1, new QTableWidgetItem("0x" + QString("%1 %2").arg(
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
                                                                                                             _MODBUS_RTU_Slave->GetLastResult()
#else
                                                                                                             MODBUS_RTU_Slave_GetLastResult(_MODBUS_RTU_Slave)
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
#endif
                                                                                                             , 4, 16, '0').arg(MODBUS_RTU_Slave_GetLastResultString(_MODBUS_RTU_Slave)).toUpper()));
            ui->tableWidget_Result->setItem(startRow + 2, 0, new QTableWidgetItem("Time Elapsed                耗时"));
            ui->tableWidget_Result->setItem(startRow + 2, 1, new QTableWidgetItem(QString("%1 ms").arg(timeSpanMS)));
        }
    }
    else
    {
        timeSpanMS += time_Start.msecsTo(QDateTime::currentDateTime());
        ui->tableWidget_Result->setRowCount(3);
        QTableWidgetItem* resultKey = new QTableWidgetItem("Header 检查结果");
        resultKey->setBackground(QColor(0xFF, 0x00, 0x00, 0x1F));
        ui->tableWidget_Result->setItem(0, 0, resultKey);
        QTableWidgetItem* resultValue = new QTableWidgetItem("失败");
        resultValue->setBackground(QColor(0xFF, 0x00, 0x00, 0x1F));
        ui->tableWidget_Result->setItem(0, 1, resultValue);
        ui->tableWidget_Result->setItem(1, 0, new QTableWidgetItem("ERROR CODE   错误代码"));
        ui->tableWidget_Result->setItem(1, 1, new QTableWidgetItem("0x" + QString("%1 %2").arg(
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
                                                                                              _MODBUS_RTU_Slave->GetLastResult()
#else
                                                                                              MODBUS_RTU_Slave_GetLastResult(_MODBUS_RTU_Slave)
#endif
                                                                                              , 4, 16, '0').arg(MODBUS_RTU_Slave_GetLastResultString(_MODBUS_RTU_Slave)).toUpper()));
        ui->tableWidget_Result->setItem(2, 0, new QTableWidgetItem("Time Elapsed 耗时"));
        ui->tableWidget_Result->setItem(2, 1, new QTableWidgetItem(QString("%1 ms").arg(timeSpanMS)));
    }
    QByteArray returnFrameArray((char*)
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
                                _MODBUS_RTU_Slave->GetReturnData(),_MODBUS_RTU_Slave->GetReturnFrameLength());
#else
                                MODBUS_RTU_Slave_GetReturnData(_MODBUS_RTU_Slave), MODBUS_RTU_Slave_GetReturnFrameLength(_MODBUS_RTU_Slave));
#endif
    ui->lineEdit_ReturnFrame->setText(returnFrameArray.toHex(' ').toUpper());
}

void MainWindow::SLOT_UpdateFrame()
{
    _Frame.clear();
    _Frame.append(ui->spinBox_DeviceAddress->value());
    const static uint8_t comboBoxSelectionoFunctionCode[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x0F, 0x10};
    _Frame.append(comboBoxSelectionoFunctionCode[ui->comboBox_FunctionCode->currentIndex()]);
    _Frame.append(ui->spinBox_RegisterAddress->value() >> 8);
    _Frame.append(ui->spinBox_RegisterAddress->value());
    _Frame.append(ui->spinBox_Amount->value() >> 8);
    _Frame.append(ui->spinBox_Amount->value());
    _Frame.append(QByteArray::fromHex(ui->lineEdit_Data->text().toLatin1()));
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
    uint16_t crcValue = _Boolean::Communication::MODBUS_RTU_Slave::CalculateCRC((unsigned char*)_Frame.constData(), _Frame.length());
#else
    uint16_t crcValue = MODBUS_RTU_Slave_CalculateCRC((unsigned char*)_Frame.constData(), _Frame.length());
#endif
        _Frame.append(crcValue);
    _Frame.append(crcValue >> 8);
    ui->lineEdit_CRC->setText(QString("%1 %2").arg(crcValue & 0xFF, 2, 16, '0').arg(crcValue >> 8, 2, 16, '0').toUpper());
    QString str = "";
    for (int i = 0; i < _Frame.length(); i++)
    {
        str += QString("%1 ").arg((unsigned char)_Frame[i], 2, 16, '0');
    }
    ui->lineEdit_Frame->setText(str.toUpper());
}

void MainWindow::SLOT_TableWidget_RegisterAreas_CurrentCellChanged(int newRow, int newColumn, int oldRow, int oldColumn)
{
    if (oldRow != newRow && newRow >= 0 && newRow < MODBUS_COILEDREGISTERS)
    {
        ShowRegisterArea(newRow);
    }
}

void MainWindow::ShowRegisterArea(int index)
{
    ui->tableWidget_Registers->clearContents();
    MODBUS_RTU_ContiguousRegistersGroup* area = _CoiledRegistersArray + index;
    int amount = area->Bytes >>
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                 (area->RegisterType == (uint8_t)MODBUS_RTU_FUNCTIONCODE_ATTRIBUTE_16BITS ?
#endif
                      1
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                                                                                           : 0)
#endif
        ;
    ui->tableWidget_Registers->setRowCount(amount);
    for (int i = 0; i < amount; i++)
    {

        QTableWidgetItem* col0 = new QTableWidgetItem("0x" + (QString("%1 %2").arg(i, 4, 16, '0').arg(i, 4).toUpper()));
        int regAddr = area->FirstRegisterAddress + (i <<
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                                                    (area->RegisterType == MODBUS_RTU_REGISTER_TYPE_UINT16 ?
#endif
                                                         0
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                                                                                                           : 3)
#endif
                                                    );
        QTableWidgetItem* col1 = new QTableWidgetItem("0x" + QString("%1 %2").arg(regAddr, 4, 16, '0').arg(regAddr, 4).toUpper());
        QTableWidgetItem* col2;
        if (area->RegistersAttribute[i] == (MODBUS_RTU_REGISTER_ATTRIBUTE_READABLE | MODBUS_RTU_REGISTER_ATTRIBUTE_WRITABLE))
        {
            col2 = new QTableWidgetItem("R/W");
        }
        else if (area->RegistersAttribute[i] == MODBUS_RTU_REGISTER_ATTRIBUTE_READABLE)
        {
            col2 = new QTableWidgetItem("R");
        }
        else
        {
            col2 = new QTableWidgetItem("W");
        }
        unsigned short data =
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
            area->RegisterType == MODBUS_RTU_REGISTER_TYPE_UINT16 ?
#endif
                ((unsigned short*)area->Registers)[i]
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                                                                  : ((unsigned char*)area->Registers)[i]
#endif
            ;
        QTableWidgetItem* col3 = new QTableWidgetItem("0x" + QString("%1").arg(data,
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                                                                               area->RegisterType == MODBUS_RTU_REGISTER_TYPE_UINT16 ?
#endif
                                                                                   4
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                                                                                                                                     : 2
#endif
                                                                               , 16, '0').toUpper() +

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                                                      ((area->RegisterType == MODBUS_RTU_REGISTER_TYPE_UINT16) ? "" : " 0b " +  QString("%1 %2").arg(data >> 4 & 0x0F, 4, 2, '0').arg(data & 0x0F, 4, 2, '0')) +
#endif
                                                      QString(" %1").arg(data, 4));
        col0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        col1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        col2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        col3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget_Registers->setItem(i, 0, col0);
        ui->tableWidget_Registers->setItem(i, 1, col1);
        ui->tableWidget_Registers->setItem(i, 2, col2);
        ui->tableWidget_Registers->setItem(i, 3, col3);
    }
    ui->tableWidget_Registers->viewport()->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SLOT_LineEdit_Data_TextChanged(const QString &arg1)
{
    int i,j;
    QString uStr,uStr2,uStr3;
    uStr=arg1;
    uStr=uStr.replace(" ","");//移除空格

    j=uStr.count();
    if((j>3)&&(0==(j%2)))
    {
        uStr3="";
        for(i=0;i<((j/2)-1);i++)
        {
            uStr2=uStr.mid(i*2,2); //取子串
            uStr3 +=uStr2+" ";  //加空格
        }
        uStr2=uStr.mid(j-2,2); //加上后面的
        uStr3 +=uStr2;

        ui->lineEdit_Data->setText(uStr3.toUpper());
    }
}
