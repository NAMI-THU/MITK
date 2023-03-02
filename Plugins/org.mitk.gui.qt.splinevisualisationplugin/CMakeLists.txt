project(org_mitk_gui_qt_splinevisualisationplugin)

MITK_CREATE_MODULE_TESTS()

mitk_create_plugin(
  EXPORT_DIRECTIVE SPLINEVISUALISATIONPLUGIN_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src ../../../org.mitk.gui.qt.igttracking/src/internal/ 
  PACKAGE_DEPENDS ITK VTK VTK|IOImage VTK|InfovisCore VTK|InfovisLayout VTK|CommonComputationalGeometry VTK|ViewsCore VTK|ViewsInfovis VTK|CommonColor VTK|CommonCore VTK|InteractionStyle VTK|InteractionWidgets VTK|RenderingCore VTK|RenderingFreeType VTK|TestingCore VTK|ImagingHybrid VTK|FiltersPoints VTK|CommonDataModel
  MODULE_DEPENDS MitkCore MitkQtWidgetsExt MitkIGT MitkIGTUI MitkLegacyGL MitkDICOM MitkContourModel MitkPlanarFigure MitkMapperExt MitkAlgorithmsExt MitkLegacyIO MitkTubeGraph MitkTestingHelper

)

