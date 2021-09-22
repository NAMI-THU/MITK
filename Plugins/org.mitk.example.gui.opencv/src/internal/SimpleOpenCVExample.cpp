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

// Blueberry
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>

// Qmitk
#include "SimpleOpenCVExample.h"

// mitk image
#include <mitkImage.h>
#include <QTimer>
#include <mitkIRenderingManager.h>



const std::string SimpleOpenCVExample::VIEW_ID = "org.mitk.views.simpleopencvexample";

void SimpleOpenCVExample::SetFocus() {}

void SimpleOpenCVExample::CreateQtPartControl(QWidget *parent)
{
  // create GUI widgets from the Qt Designer's .ui file
  m_Controls.setupUi(parent);

  connect(m_Controls.m_StartGrabbing, SIGNAL(clicked()), this, SLOT(OnStartGrabbing()));
  connect(m_UpdateTimer, SIGNAL(timeout()), this, SLOT(OnUpdateImage()));
}

SimpleOpenCVExample::SimpleOpenCVExample() : m_running(false), m_UpdateTimer(new QTimer())
{
}

SimpleOpenCVExample::~SimpleOpenCVExample() {}

void SimpleOpenCVExample::OnStartGrabbing()
{
  if (!m_running)
  {
    m_running = true;
    m_Controls.m_StartGrabbing->setText("Stop Video Grabbing");
    m_VideoCapture = new cv::VideoCapture(); // open the default camera
    m_VideoCapture->open(m_Controls.m_InputID->value());
    if (!m_VideoCapture->isOpened())  {return;} // check if we succeeded

    if (m_Controls.m_SeparateWindow->isChecked())
    {
      cv::namedWindow("Video", 1);
      while (m_running)
      {
        cv::Mat frame;
        *m_VideoCapture >> frame; // get a new frame from camera
        imshow("Video", frame);
        // Example to write a frame to a file:
        // imwrite("C:/temp/output.jpg", frame);
        // Press 'c' to stop the stream
        if (cv::waitKey(30) == 'c') break;
      }
    }
    else if (m_Controls.m_MITKImage->isChecked())
    {
      mitk::DataNode::Pointer imageNode = mitk::DataNode::New();
      imageNode->SetName("Open CV Example Image Stream");
      imageNode->SetData(mitk::Image::New());
      m_conversionFilter = mitk::OpenCVToMitkImageFilter::New();
      this->GetDataStorage()->Add(imageNode);
      OnUpdateImage();
      // Initialize view on Image
      mitk::IRenderWindowPart *renderWindow = this->GetRenderWindowPart();
      if (renderWindow != NULL) renderWindow->GetRenderingManager()->InitializeViews(
          imageNode->GetData()->GetGeometry(), mitk::RenderingManager::REQUEST_UPDATE_ALL, true);

      m_UpdateTimer->setInterval(20);
      m_UpdateTimer->start();
    
    }
  }
  else
  {
    m_UpdateTimer->stop();
    m_running = false;
    m_Controls.m_StartGrabbing->setText("Start Video Grabbing");
    cv::destroyWindow("Video");
    mitk::DataNode::Pointer imageNode = this->GetDataStorage()->GetNamedNode("Open CV Example Image Stream");
    this->GetDataStorage()->Remove(imageNode);
    m_VideoCapture->release();
    delete m_VideoCapture;
  }
}

void SimpleOpenCVExample::OnUpdateImage() 
{
  cv::Mat image;
  *m_VideoCapture >> image;
  IplImage ipl_img = image;
  m_conversionFilter->SetOpenCVImage(&ipl_img);
  m_conversionFilter->Update();
  this->GetDataStorage()->GetNamedNode("Open CV Example Image Stream")->SetData(m_conversionFilter->GetOutput());
  this->RequestRenderWindowUpdate(mitk::RenderingManager::REQUEST_UPDATE_ALL);
}
