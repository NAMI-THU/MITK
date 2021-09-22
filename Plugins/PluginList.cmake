
# Plug-ins must be ordered according to their dependencies

set(MITK_PLUGINS

  org.blueberry.core.runtime:ON
  org.blueberry.core.expressions:OFF
  org.blueberry.core.commands:OFF
  org.blueberry.core.jobs:OFF
  org.blueberry.ui.qt:OFF
  org.blueberry.ui.qt.help:ON
  org.blueberry.ui.qt.log:ON
  org.blueberry.ui.qt.objectinspector:OFF
  org.mitk.core.services:ON
  org.mitk.gui.common:ON
  org.mitk.planarfigure:ON
  org.mitk.core.ext:OFF
  org.mitk.core.jobs:OFF
  org.mitk.example.gui.opencv:ON
  org.mitk.gui.qt.ablationplanning:ON
  org.mitk.gui.qt.ARStrokeTreatment:ON
  org.mitk.gui.qt.application:ON
  org.mitk.gui.qt.ext:OFF
  org.mitk.gui.qt.extapplication:OFF
  org.mitk.gui.qt.mitkworkbench.intro:OFF
  org.mitk.gui.qt.common:ON
  org.mitk.gui.qt.stdmultiwidgeteditor:ON
  org.mitk.gui.qt.mxnmultiwidgeteditor:OFF
  org.mitk.gui.qt.common.legacy:OFF
  org.mitk.gui.qt.cmdlinemodules:OFF
  org.mitk.gui.qt.chartExample:OFF
  org.mitk.gui.qt.datamanager:ON
  org.mitk.gui.qt.datamanagerlight:OFF
  org.mitk.gui.qt.datastorageviewertest:OFF
  org.mitk.gui.qt.properties:ON
  org.mitk.gui.qt.basicimageprocessing:OFF
  org.mitk.gui.qt.dicombrowser:OFF
  org.mitk.gui.qt.dicominspector:OFF
  org.mitk.gui.qt.dosevisualization:OFF
  org.mitk.gui.qt.geometrytools:OFF
  org.mitk.gui.qt.igtexamples:OFF
  org.mitk.gui.qt.igttracking:OFF
  org.mitk.gui.qt.openigtlink:OFF
  org.mitk.gui.qt.imagecropper:OFF
  org.mitk.gui.qt.imagenavigator:ON
  org.mitk.gui.qt.viewnavigator:OFF
  org.mitk.gui.qt.materialeditor:OFF
  org.mitk.gui.qt.measurementtoolbox:OFF
  org.mitk.gui.qt.moviemaker:OFF
  org.mitk.gui.qt.pointsetinteraction:OFF
  org.mitk.gui.qt.pointsetinteractionmultispectrum:OFF
  org.mitk.gui.qt.python:OFF
  org.mitk.gui.qt.remeshing:OFF
  org.mitk.gui.qt.segmentation:OFF
  org.mitk.gui.qt.deformableclippingplane:OFF
  org.mitk.gui.qt.aicpregistration:OFF
  org.mitk.gui.qt.renderwindowmanager:OFF
  org.mitk.gui.qt.semanticrelations:OFF
  org.mitk.gui.qt.thrombus:ON
  org.mitk.gui.qt.toftutorial:OFF
  org.mitk.gui.qt.tofutil:OFF
  org.mitk.gui.qt.tubegraph:OFF
  org.mitk.gui.qt.ugvisualization:OFF
  org.mitk.gui.qt.ultrasound:OFF
  org.mitk.gui.qt.volumevisualization:OFF
  org.mitk.gui.qt.eventrecorder:OFF
  org.mitk.gui.qt.xnat:OFF
  org.mitk.gui.qt.igt.app.ultrasoundtrackingnavigation:OFF
  org.mitk.gui.qt.classificationsegmentation:OFF
  org.mitk.gui.qt.overlaymanager:OFF
  org.mitk.gui.qt.igt.app.hummelprotocolmeasurements:OFF
  org.mitk.gui.qt.multilabelsegmentation:OFF
  org.mitk.matchpoint.core.helper:OFF
  org.mitk.gui.qt.matchpoint.algorithm.browser:OFF
  org.mitk.gui.qt.matchpoint.algorithm.control:OFF
  org.mitk.gui.qt.matchpoint.mapper:OFF
  org.mitk.gui.qt.matchpoint.framereg:OFF
  org.mitk.gui.qt.matchpoint.visualizer:OFF
  org.mitk.gui.qt.matchpoint.evaluator:OFF
  org.mitk.gui.qt.matchpoint.manipulator:OFF
  org.mitk.gui.qt.preprocessing.resampling:OFF
  org.mitk.gui.qt.radiomics:OFF
  org.mitk.gui.qt.cest:OFF
  org.mitk.gui.qt.fit.demo:OFF
  org.mitk.gui.qt.fit.inspector:OFF
  org.mitk.gui.qt.fit.genericfitting:OFF
  org.mitk.gui.qt.pharmacokinetics.mri:OFF
  org.mitk.gui.qt.pharmacokinetics.pet:OFF
  org.mitk.gui.qt.pharmacokinetics.simulation:OFF
  org.mitk.gui.qt.pharmacokinetics.curvedescriptor:OFF
  org.mitk.gui.qt.pharmacokinetics.concentration.mri:OFF
  org.mitk.gui.qt.flowapplication:OFF
  org.mitk.gui.qt.flow.segmentation:OFF
)
