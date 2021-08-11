#include "QmitkThrombusDataReader.h"

#include <mitkTrackingDeviceSourceConfigurator.h>

void QmitkThrombusDataReader::ConnectingToTrackingDevice()
{
  std::cout << "Generating TrackingDevice ..." << std::endl;
  mitk::VirtualTrackingDevice::Pointer tracker = mitk::VirtualTrackingDevice::New();
  tracker->AddTool("tool1");
  tracker->AddTool("tool2");
  return;
}

// void QmitkThrombusDataReader::SetTrackingDevice(mitk::TrackingDevice::Pointer td)
//{
//  m_TrackingDevice = td;
//}
//
// void QmitkThrombusDataReader::AutoDetectTools()
//{
//  MITK_INFO << "BANANARAMA!";
//  mitk::NavigationToolStorage::Pointer autoDetectedStorage = mitk::NavigationToolStorage::New(m_DataStorage);
//  try
//  {
//    mitk::NavigationToolStorage::Pointer tempStorage = m_TrackingDevice->AutoDetectTools();
//    for (unsigned int i = 0; i < tempStorage->GetToolCount(); i++)
//    {
//      autoDetectedStorage->AddTool(tempStorage->GetTool(i));
//    }
//  }
//  catch (mitk::Exception &e)
//  {
//    MITK_WARN << e.GetDescription();
//    //emit AutoDetectToolsFinished(false, e.GetDescription());
//    return;
//  }
//  m_NavigationToolStorage = nullptr;
//  m_NavigationToolStorage = autoDetectedStorage;
//  //emit AutoDetectToolsFinished(true, "");
//  MITK_INFO << "AutoDetect Tools Finished.";
//}
//
// void QmitkThrombusDataReader::ConnectDevice()
//{
//  // build IGT pipeline
//  mitk::TrackingDevice::Pointer trackingDevice = m_TrackingDevice;
//  trackingDevice->SetData(m_TrackingDeviceData);
//
//  // get volume tracking data
//  mitk::TrackingDeviceData data = m_TrackingDeviceData;
//
//  //---TESTING NEEDED---//
//}
//
// void QmitkThrombusDataReader::StartTracking()
//{
//  QString errMsg = "Unknown Error occured!";
//  try
//  {
//    m_TrackingDeviceSource->StartTracking();
//  }
//  catch (...) // ToDo: change to mitk::IGTException
//  {
//    MITK_ERROR << errMsg.toStdString();
//    return;
//  }
//}
//
// void QmitkThrombusDataReader::StopTracking()
//{
//  QString errMsg = "Unknown Error Occured!";
//  try
//  {
//    m_TrackingDeviceSource->StopTracking();
//  }
//  catch (...) //Todo, as above
//  {
//    MITK_ERROR << errMsg.toStdString();
//    return;
//  }
//}
//
// void QmitkThrombusDataReader::DisconnectDevice() {
//  if (m_TrackingDeviceSource->IsTracking())
//  {
//    m_TrackingDeviceSource->StopTracking();
//  }
//  m_TrackingDeviceSource->Disconnect();
//  m_TrackingDeviceSource = nullptr;
//}