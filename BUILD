package(default_visibility = ["//visibility:public"])

# Default target: include all messages and supporting code.
cc_library(
    name = "fusion_engine_client",
    deps = [
        ":core",
        ":messages",
    ],
)

# Support for building a shared library if desired.
cc_binary(
    name = "libfusion_engine_client.so",
    linkshared = True,
    deps = [
        ":fusion_engine_client",
    ],
)

# Core navigation solution support functionality.
cc_library(
    name = "core",
    deps = [
        ":core_headers",
    ],
)

################################################################################
# Message Definitions
################################################################################

# Message definition headers only (all message types).
cc_library(
    name = "messages",
    deps = [
        ":core_headers",
    ],
)

# Core navigation solution message definitions.
cc_library(
    name = "core_headers",
    hdrs = [
        "src/point_one/common/portability.h",
        "src/point_one/messages/core.h",
        "src/point_one/messages/defs.h",
        "src/point_one/messages/solution.h",
    ],
    includes = ["src"],
)
