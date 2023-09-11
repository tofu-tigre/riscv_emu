cc_library(
  name = "cpu",
  hdrs = ["cpu.h"],
  srcs = ["cpu.cc"],
  visibility = ["//visibility:public"],
  deps = [
    "//lib/logic:register",
    "//lib/logic:wires",
    "//lib/branch_cmp:branch_cmp",
    "//lib/alu:alu",
    ":instr_decoder",
    "//lib/memory:dram",
    "@com_google_absl//absl/status:status",
    "@com_google_absl//absl/status:statusor",
    "@status_macros//:status_macros",
    "@com_github_google_glog//:glog",
  ],
)

cc_library(
  name = "instr_decoder",
  hdrs = ["instr_decoder.h"],
  srcs = ["instr_decoder.cc"],
  visibility = ["//visibility:public"],
  deps = [
    "//lib/logic:register",
    "//lib/logic:wires",
    "//lib/alu:alu",
    "//lib/branch_cmp:branch_cmp",
    "//lib/immediates:imm_decoder",
    "//lib/memory:dram",
    "@com_google_absl//absl/status:status",
    "@com_google_absl//absl/status:statusor",
    "@status_macros//:status_macros",
    "@com_github_google_glog//:glog",
  ],
)