#include "QmitkThrombusDataReader.h"


void QmitkThrombusDataReader::SetTrackingDevice(mitk::TrackingDeviceSource::Pointer m_TrackingDevice) {

}

void QmitkThrombusDataReader::ConnectDevice() {
    // build IGT pipeline
  mitk::TrackingDevice::Pointer trackingDevice = m_TrackingDevice;
    trackingDevice->SetData(m_TrackingDeviceData);

    // get volume tracking data
    mitk::TrackingDeviceData data = m_TrackingDeviceData;

    //---TESTING NEEDED---//
}

void QmitkThrombusDataReader::StartTracking() {
  QString errMsg = "Unknown Error occured!";
  try
  {
    m_TrackingDeviceSource->StartTracking();
  }
  catch (...) // ToDo: change to mitk::IGTException
  {
    MITK_ERROR << "No Tracking Device Found!";
    return;
  }

}