#ifndef QMITKTHROMBUSDATAREADER_H
#define QMITKTHROMBUSDATAREADER_H

// blueberry headers
#include <berryISelectionListener.h>

// mitk headers
#include <MitkIGTExports.h>
#include <mitkNavigationToolStorage.h>
#include <mitkTrackingDevice.h>
#include <mitkTrackingDeviceSource.h>
#include <mitkCommon.h>

// Qt headers
#include <QTimer>

class QmitkThrombusDataReader
{
  mitk::TrackingDevice::Pointer m_TrackingDevice;

  mitk::TrackingDeviceSource::Pointer m_TrackingDeviceSource; // Source of the IGT pipeline
  mitk::TrackingDeviceData m_TrackingDeviceData; // Handles the tracking device data

  void SetTrackingDevice(mitk::TrackingDeviceSource::Pointer trackingSource);


  void ConnectDevice();
  void StartTracking();

};

#endif // QMITKTHROMBUSDATAREADER_H