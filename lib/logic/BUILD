cc_library(
  name = "opcodes",
  hdrs = ["opcodes.h"],
  visibility = ["//visibility:public"],
)

cc_library(
  name = "wires",
  hdrs = ["wire.h"],
  srcs = ["wire.cc"],
  visibility = ["//visibility:public"],
  deps = [
    ":opcodes",
    "@com_google_absl//absl/status:statusor",
    "@status_macros//:status_macros",
  ],
)

cc_library(
  name = "register",
  hdrs = ["register.h"],
  visibility = ["//visibility:public"],
  deps = [
    ":wires",
  ],
)
