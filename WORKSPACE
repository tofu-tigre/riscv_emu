load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "status_macros",
    commit = "1592ab2d4b4f92976fc3f4a6cb3a1323a4b549c3",
    remote = "https://github.com/jimrogerz/status_macros.git",
)

http_archive(
  name = "com_google_absl",  # 23-09-07
  urls = ["https://github.com/abseil/abseil-cpp/archive/3cb94be9bea2eda6dd2d572d1cf074efcc48a20b.zip"],
  strip_prefix = "abseil-cpp-3cb94be9bea2eda6dd2d572d1cf074efcc48a20b",
)

# Bazel Skylib.
http_archive(
  name = "bazel_skylib",  # 2023-05-31
  strip_prefix = "bazel-skylib-288731ef9f7f688932bd50e704a91a45ec185f9b",
  urls = ["https://github.com/bazelbuild/bazel-skylib/archive/288731ef9f7f688932bd50e704a91a45ec185f9b.zip"],
)

http_archive(
  name = "com_google_googletest",  # 23-09-07
  urls = ["https://github.com/google/googletest/archive/843976e4f582ccb76cf87e0f128585324335779b.zip"],
  strip_prefix = "googletest-843976e4f582ccb76cf87e0f128585324335779b",
)

http_archive(
    name = "com_github_google_benchmark",  # 23-09-07
    urls = ["https://github.com/google/benchmark/archive/02a354f3f323ae8256948e1dc77ddcb1dfc297da.zip"],
    strip_prefix = "benchmark-02a354f3f323ae8256948e1dc77ddcb1dfc297da",
)

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
)

http_archive(
    name = "com_github_google_glog",
    sha256 = "122fb6b712808ef43fbf80f75c52a21c9760683dae470154f02bddfc61135022",
    strip_prefix = "glog-0.6.0",
    urls = ["https://github.com/google/glog/archive/v0.6.0.zip"],
)





# http_archive(
#   name = "com_google_absl",
#   urls = ["https://github.com/abseil/abseil-cpp/archive/98eb410c93ad059f9bba1bf43f5bb916fc92a5ea.zip"],
#   strip_prefix = "abseil-cpp-98eb410c93ad059f9bba1bf43f5bb916fc92a5ea",
# )

# http_archive(
#   name = "bazel_skylib",
#   urls = ["https://github.com/bazelbuild/bazel-skylib/releases/download/1.2.1/bazel-skylib-1.2.1.tar.gz"],
#   sha256 = "f7be3474d42aae265405a592bb7da8e171919d74c16f082a5457840f06054728",
# )

# http_archive(
#   name = "com_google_googletest",
#   urls = ["https://github.com/google/googletest/archive/011959aafddcd30611003de96cfd8d7a7685c700.zip"],
#   strip_prefix = "googletest-011959aafddcd30611003de96cfd8d7a7685c700",
# )

# http_archive(
#     name = "com_github_google_benchmark",
#     urls = ["https://github.com/google/benchmark/archive/bf585a2789e30585b4e3ce6baf11ef2750b54677.zip"],
#     strip_prefix = "benchmark-bf585a2789e30585b4e3ce6baf11ef2750b54677",
#     sha256 = "2a778d821997df7d8646c9c59b8edb9a573a6e04c534c01892a40aa524a7b68c",
# )
