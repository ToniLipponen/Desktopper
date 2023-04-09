pkg_check_modules(adwaita REQUIRED libadwaita-1)
include_directories(${adwaita_INCLUDE_DIRS})
link_directories(${adwaita_LIBRARY_DIRS})
add_definitions(${adwaita_CFLAGS_OTHER})