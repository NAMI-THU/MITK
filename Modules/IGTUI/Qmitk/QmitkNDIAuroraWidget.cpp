/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "QmitkNDIAuroraWidget.h"
#include "QmitkTrackingDeviceConfigurationWidget.h"
#include <mitkNDITrackingDevice.h>

#include <QFileDialog>
#include <QMessageBox>

const std::string QmitkNDIAuroraWidget::VIEW_ID = "org.mitk.views.NDIAuroraWidget";

QmitkNDIAuroraWidget::QmitkNDIAuroraWidget(QWidget* parent, Qt::WindowFlags f)
  : QmitkNDIAbstractDeviceWidget(parent, f)
{
  m_Controls = NULL;
  CreateQtPartControl(this);
  CreateConnections();
  m_ErrorMessage = "";
}

QmitkNDIAuroraWidget::~QmitkNDIAuroraWidget()
{
}

void QmitkNDIAuroraWidget::CreateQtPartControl(QWidget *parent)
{
  if (!m_Controls)
  {
    // create GUI widgets
    m_Controls = new Ui::QmitkNDIAuroraWidget;
    m_Controls->setupUi(parent);
  }
}

void QmitkNDIAuroraWidget::CreateConnections()
{
  if (m_Controls)
  {
    connect((QObject*)(m_Controls->m_testConnectionAurora), SIGNAL(clicked()), this, SLOT(TestConnection()));
    connect((QObject*)(m_Controls->m_AutoScanAurora), SIGNAL(clicked()), this, SLOT(AutoScanPorts()));
    //set a few UI components depending on Windows / Linux
#ifdef WIN32
    m_Controls->portTypeLabelAurora->setVisible(false);
    m_Controls->portTypeAurora->setVisible(false);
#else
    m_Controls->comPortLabelAurora->setText("Port Nr:");
    m_Controls->m_portSpinBoxAurora->setPrefix("");
#endif
  }
}

void QmitkNDIAuroraWidget::ResetOutput()
{
  m_Controls->m_outputTextAurora->setHtml("<body style=\" font-family:\'MS Shell Dlg 2\'; font-size:7pt; font-weight:400; font-style:normal;\" bgcolor=black><span style=\"color:#ffffff;\"><u>output:</u>");
}

void QmitkNDIAuroraWidget::AddOutput(std::string s)
{
  m_Controls->m_outputTextAurora->setHtml(QString(s.c_str()));
  m_Controls->m_outputTextAurora->verticalScrollBar()->setValue(m_Controls->m_outputTextAurora->verticalScrollBar()->maximum());
}

mitk::TrackingDevice::Pointer QmitkNDIAuroraWidget::ConstructTrackingDevice()
{
  mitk::NDITrackingDevice::Pointer tempTrackingDevice = mitk::NDITrackingDevice::New();

  //get port
  int port = 0;
  port = m_Controls->m_portSpinBoxAurora->value();

  //build prefix (depends on linux/win)
  QString prefix = "";
#ifdef WIN32
  prefix = "COM";
  tempTrackingDevice->SetPortNumber(static_cast<mitk::SerialCommunication::PortNumber>(port)); //also set the com port for compatibility
#else
  prefix = m_Controls->portTypeAurora->currentText();
#endif

  //build port name string
  QString portName = prefix + QString::number(port);

  tempTrackingDevice->SetDeviceName(portName.toStdString()); //set the port name
  tempTrackingDevice->SetBaudRate(mitk::SerialCommunication::BaudRate115200);//set baud rate
  tempTrackingDevice->SetType(mitk::TRACKING_DEVICE_IDENTIFIER_AURORA);
  return static_cast<mitk::TrackingDevice::Pointer>(tempTrackingDevice);
}

void QmitkNDIAuroraWidget::StoreUISettings()
{}

void QmitkNDIAuroraWidget::LoadUISettings()
{}

void QmitkNDIAuroraWidget::SetPortValueToGUI(int portValue){
  m_Controls->m_portSpinBoxAurora->setValue(portValue);
}
void QmitkNDIAuroraWidget::SetPortTypeToGUI(int portType){
  m_Controls->portTypeAurora->setCurrentIndex(portType);
}
