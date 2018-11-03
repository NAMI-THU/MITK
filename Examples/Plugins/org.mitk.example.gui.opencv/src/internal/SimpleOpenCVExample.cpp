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

#include "opencv2/opencv.hpp"

const std::string SimpleOpenCVExample::VIEW_ID = "org.mitk.views.simpleopencvexample";

void SimpleOpenCVExample::SetFocus()
{
}

void SimpleOpenCVExample::CreateQtPartControl(QWidget *parent)
{
  // create GUI widgets from the Qt Designer's .ui file
  m_Controls.setupUi(parent);

  connect(m_Controls.m_StartGrabbing, SIGNAL(clicked()), this, SLOT(OnStartGrabbing()));
}

SimpleOpenCVExample::SimpleOpenCVExample() : m_running(false) {

}

SimpleOpenCVExample::~SimpleOpenCVExample()
{
  
}

void SimpleOpenCVExample::OnStartGrabbing()
{	
  if (!m_running)
    {
    m_running = true;
    m_Controls.m_StartGrabbing->setText("Stop Video Grabbing");
    if (m_Controls.m_SeparateWindow->isChecked())
      {
	      cv::VideoCapture cap = cv::VideoCapture(); // open the default camera
        cap.open(0);
        if (!cap.isOpened()) // check if we succeeded
          return;

        cv::namedWindow("Video", 1);
 
        while (m_running)
        {
          cv::Mat frame;
          cap >> frame; // get a new frame from camera
          imshow("Video", frame);

          // imwrite("C:/temp/output.jpg", frame);

          // Press 'c' to escape
          if (cv::waitKey(30) == 'c')
            break;
        }
      }
  
  }
    else
    {
      m_running = false;
      m_Controls.m_StartGrabbing->setText("Start Video Grabbing");
      cv::destroyWindow("Video");
    }
    
}
