set(target concurrency)

# Sources
set(headers
)

set(sources
    task_queue.h
    task_system.h
)

# Build library
# add_library(${target}
#     ${sources}
#     ${headers}
# )
add_library(${target} INTERFACE)
target_include_directories(${target} INTERFACE .)

# Create namespaced alias
add_library(${META_PROJECT_NAME}::${target} ALIAS ${target})
