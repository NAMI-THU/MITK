project(org_mitk_gui_qt_igtregistration)

mitk_create_plugin(
  EXPORT_DIRECTIVE IGTREGISTRATION_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDS MitkQtWidgetsExt
)
