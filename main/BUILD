cc_binary(
  name = "emu",
  srcs = ["main.cc"],
  deps = [
    "//lib/logic:wires",
    "//lib/logic:opcodes",
    "@com_google_absl//absl/status:statusor",
    "@status_macros//:status_macros",
    "@com_github_google_glog//:glog",
    "//lib/alu:alu",
    "//lib/immediates:imm_decoder",
    "//lib/cpu:cpu",
  ],
)