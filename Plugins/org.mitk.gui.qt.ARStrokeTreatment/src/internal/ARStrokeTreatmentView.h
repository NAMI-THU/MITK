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

public:
  virtual void CreateQtPartControl(QWidget *parent) override;

  virtual void SetFocus() override;

  /// \brief called by QmitkFunctionality when DataManager's selection has changed
  virtual void OnSelectionChanged(berry::IWorkbenchPart::Pointer source,
                                  const QList<mitk::DataNode::Pointer> &nodes) override;

  /// \brief Called when the user clicks the GUI button
  void DoImageProcessing();

  /// \connects all the buttons, labels, etc. of the Qt gui
  void CreateConnections();

  Ui::ARStrokeTreatmentControls* m_Controls; // automatically generated from ARStrokeTreatmentView.ui

protected:
  cv::VideoCapture *m_VideoCapture = new cv::VideoCapture;
  //opencv video support modul
  // mitk::OpenCVVideoSource

  mitk::OpenCVToMitkImageFilter::Pointer m_ConversionFilter = mitk::OpenCVToMitkImageFilter::New();

  // update timer for the tracking grabber
  QTimer *m_UpdateTimer;

  bool m_TrackingActive = false;

  bool m_VideoGrabbingActive = false;

  mitk::NavigationData::Pointer m_InstrumentNavigationData;

  mitk::NavigationData::Pointer m_TrackingData;

  mitk::DataNode::Pointer m_imageNode;

  // members for initial registration
  mitk::DataNode::Pointer m_ImageFiducialsDataNode;
  mitk::DataNode::Pointer m_TrackerFiducialsDataNode;

  //void ARStrokeTreatmentView::OnAddRegistrationTrackingFiducial();

protected slots:
  // starts/stops the tracking of the live tracking data
  void OnTrackingGrabberPushed();
  // starts/stops the tracking of the live video data
  void OnVideoGrabberPushed();

  //void OnRegistrationPushed();

  void UpdateLiveData();

  // prints a small text through MITK_INFO, for testing purposes
  void TestText();
};

#endif // ARStrokeTreatmentView_h
