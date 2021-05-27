#ifndef QMITKTHROMBUSDATAREADER_H
#define QMITKTHROMBUSDATAREADER_H

// blueberry headers
#include <berryISelectionListener.h>

// mitk headers
#include <MitkIGTExports.h>
#include <mitkCommon.h>
#include <mitkNavigationToolStorage.h>
#include <mitkTrackingDevice.h>
#include <mitkTrackingDeviceSource.h>

// Qt headers
#include <QTimer>

class QmitkThrombusDataReader
{
public:
  void SetTrackingDevice(mitk::TrackingDevice::Pointer td);

  void ConnectDevice();
  void StartTracking();
  void StopTracking();
  void DisconnectDevice();


protected:
  mitk::TrackingDevice::Pointer m_TrackingDevice;

  mitk::DataStorage::Pointer m_DataStorage; // storage for the aquired data
  mitk::NavigationToolStorage::Pointer m_NavigationToolStorage; // navigation tool storage

  //Created during ConnectDevice()
  mitk::TrackingDeviceSource::Pointer m_TrackingDeviceSource; // source of the IGT pipeline
  mitk::TrackingDeviceData m_TrackingDeviceData;              // handles the tracking device data
};

#endif // QMITKTHROMBUSDATAREADER_H