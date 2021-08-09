project(org_mitk_gui_qt_thrombus)

mitk_create_plugin(
  EXPORT_DIRECTIVE THROMBUS_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDS MitkQtWidgetsExt MitkIGT MitkIGTUI
)
