cc_library(
  name = "cpu",
  hdrs = ["cpu.h"],
  srcs = ["cpu.cc"],
  visibility = ["//visibility:public"],
  deps = [
    "//lib/logic:wires",
    "//lib/alu:alu",
    "@com_google_absl//absl/status:status",
    "@com_google_absl//absl/status:statusor",
    "@status_macros//:status_macros",
    "@com_github_google_glog//:glog",
  ],
)