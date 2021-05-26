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
  mitk::TrackingDeviceSource::Pointer trackingSource;


  void SetTrackingDevice(mitk::TrackingDeviceSource::Pointer trackingSource);
};

#endif // QMITKTHROMBUSDATAREADER_H