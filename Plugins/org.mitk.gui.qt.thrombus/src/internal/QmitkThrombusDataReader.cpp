#include "QmitkThrombusDataReader.h"

void QmitkThrombusDataReader::SetTrackingDevice(mitk::TrackingDevice::Pointer td)
{
  m_TrackingDevice = td;
}

void QmitkThrombusDataReader::ConnectDevice()
{
  // build IGT pipeline
  mitk::TrackingDevice::Pointer trackingDevice = m_TrackingDevice;
  trackingDevice->SetData(m_TrackingDeviceData);

  // get volume tracking data
  mitk::TrackingDeviceData data = m_TrackingDeviceData;

  //---TESTING NEEDED---//
}

void QmitkThrombusDataReader::StartTracking()
{
  QString errMsg = "Unknown Error occured!";
  try
  {
    m_TrackingDeviceSource->StartTracking();
  }
  catch (...) // ToDo: change to mitk::IGTException
  {
    MITK_ERROR << errMsg;
    return;
  }
}

void QmitkThrombusDataReader::StopTracking()
{
  QString errMsg = "Unknown Error Occured!";
  try
  {
    m_TrackingDeviceSource->StopTracking();
  }
  catch (...) //Todo, as above
  {
    MITK_ERROR << errMsg;
    return;
  }
}

void QmitkThrombusDataReader::DisconnectDevice() {
  if (m_TrackingDeviceSource->IsTracking())
  {
    m_TrackingDeviceSource->StopTracking();
  }
  m_TrackingDeviceSource->Disconnect();
  m_TrackingDeviceSource = nullptr;
}