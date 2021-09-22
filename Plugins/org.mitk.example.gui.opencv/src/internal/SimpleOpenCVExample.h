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


#ifndef SimpleOpenCVExample_h
#define SimpleOpenCVExample_h

#include <berryISelectionListener.h>
#include <QTime>
#include <QmitkAbstractView.h>
#include "ui_SimpleOpenCVExampleControls.h"
#include "mitkOpenCVToMitkImageFilter.h"
#include "opencv2/opencv.hpp"

/**
  \brief TODO
*/
class SimpleOpenCVExample : public QmitkAbstractView
{
  Q_OBJECT

public:
  static const std::string VIEW_ID;

  SimpleOpenCVExample();

  virtual ~SimpleOpenCVExample();

protected slots:

  /// \brief Called when the user clicks the GUI button
  void OnStartGrabbing();
  void OnUpdateImage();

protected:
  virtual void CreateQtPartControl(QWidget *parent) override;

  virtual void SetFocus() override;

  bool m_running;

  cv::VideoCapture *m_VideoCapture;
  mitk::OpenCVToMitkImageFilter::Pointer m_conversionFilter;

  Ui::SimpleOpenCVExampleControls m_Controls;

  QTimer *m_UpdateTimer;
};

#endif // SimpleOpenCVExample_h
