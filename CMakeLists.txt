build_exec(
        EXECNAME experiment
        SOURCE_FILES experiment.cc rsu-app.cc vehicle-app.cc header-example.cc
        LIBRARIES_TO_LINK "${ns3-libs}" "${ns3-contrib-libs}"
        EXECUTABLE_DIRECTORY_PATH ${CMAKE_OUTPUT_DIRECTORY}/scratch/experiment
)