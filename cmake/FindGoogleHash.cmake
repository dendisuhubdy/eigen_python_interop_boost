
if (GOOGLEHASH_INCLUDES AND GOOGLEHASH_LIBRARIES)
  set(GOOGLEHASH_FIND_QUIETLY TRUE)
endif (GOOGLEHASH_INCLUDES AND GOOGLEHASH_LIBRARIES)

find_path(GOOGLEHASH_INCLUDES
  NAMES
  google/dense_hash_map
  PATHS
  ${INCLUDE_INSTALL_DIR}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GOOGLEHASH DEFAULT_MSG GOOGLEHASH_INCLUDES)

mark_as_advanced(GOOGLEHASH_INCLUDES)