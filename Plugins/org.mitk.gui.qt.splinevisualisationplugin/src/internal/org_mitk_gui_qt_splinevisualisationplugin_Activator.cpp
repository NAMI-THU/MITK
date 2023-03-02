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


#include "org_mitk_gui_qt_splinevisualisationplugin_Activator.h"
#include "SplineVisualisationPlugin.h"
#include <internal/mitkSplineDataObjectVisualizationFilter.h>
// #include <internal/mitkSplineVisualisationPluginTestSuite.cpp>

namespace mitk
{
  ctkPluginContext *org_mitk_gui_qt_splinevisualisationplugin_Activator::m_Context = nullptr;

  void org_mitk_gui_qt_splinevisualisationplugin_Activator::start(ctkPluginContext *context)
  {
	m_Context = context;
	  
    BERRY_REGISTER_EXTENSION_CLASS(SplineVisualisationPlugin, context)
  }

  void org_mitk_gui_qt_splinevisualisationplugin_Activator::stop(ctkPluginContext *context) { Q_UNUSED(context) }
  
  ctkPluginContext *org_mitk_gui_qt_splinevisualisationplugin_Activator::GetContext() { return m_Context; }
}
