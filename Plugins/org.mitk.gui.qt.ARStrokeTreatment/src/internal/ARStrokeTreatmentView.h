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

#ifndef ARStrokeTreatmentView_h
#define ARStrokeTreatmentView_h

#include "opencv2/opencv.hpp"
#include "ui_ARStrokeTreatmentControls.h"
#include <QTime>
#include <QTimer>
#include <QmitkAbstractView.h>
#include <QmitkDataStorageComboBox.h>
#include <berryISelectionListener.h>
#include <mitkOpenCVToMitkImageFilter.h>

/**
  \brief ARStrokeTreatmentView

  \warning  This class is not yet documented. Use "git blame" and ask the author to provide basic documentation.

  \sa QmitkAbstractView
  \ingroup ${plugin_target}_internal
*/

class ARStrokeTreatmentView : public QmitkAbstractView
{
  // this is needed for all Qt objects that should have a Qt meta-object
  // (everything that derives from QObject and wants to have signal/slots)
  Q_OBJECT

public:
  static const std::string VIEW_ID;

protected:
  virtual void CreateQtPartControl(QWidget *parent) override;

  virtual void SetFocus() override;

  /// \brief called by QmitkFunctionality when DataManager's selection has changed
  virtual void OnSelectionChanged(berry::IWorkbenchPart::Pointer source,
                                  const QList<mitk::DataNode::Pointer> &nodes) override;

  /// \brief Called when the user clicks the GUI button
  void DoImageProcessing();

  /// \connects all the buttons, labels, etc. of the Qt gui
  void CreateConnections();

  Ui::ARStrokeTreatmentControls m_Controls; // automatically generated from ARStrokeTreatmentView.ui

protected slots:

  /// \brief Called when the user clicks the GUI button
  // starts/stops the tracking of the live tracking data
  void OnStartTrackingGrabbing();
  // starts/stops the tracking of the live video data
  void OnStartVideoGrabbing();
  // ???
  void OnUpdateImage();
  // prints a small text through MITK_INFO, for testing purposes
  void TestText();

protected:
  // true, if tracking data is grabbed
  bool m_GrabbingTrackingData;
  // true, if video data is grabbed
  bool m_GrabbingVideoData;

  cv::VideoCapture *m_VideoCapture;
  mitk::OpenCVToMitkImageFilter *m_ConversionFilter;

  // update timer for the tracking grabber
  QTimer *m_UpdateTimerTracking;

  QTimer *m_UpdateTimerVideo;

protected slots:
  // slots to connect....

  void OnTrackingGrabberPushed();
  void UpdateTrackingData();

protected:
  bool m_TrackingActive = false;
  mitk::NavigationDataSource::Pointer m_TrackingSource;

  QTimer *m_Updatetimer;
};

#endif // ARStrokeTreatmentView_h
