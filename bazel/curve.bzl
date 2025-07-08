# Copyright 2021 Ant Group Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
warpper bazel cc_xx to modify flags.
"""

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("@yacl//bazel:yacl.bzl", "OMP_CFLAGS", "OMP_DEPS", "OMP_LINKFLAGS")

WARNING_FLAGS = [
    "-Wall",
    "-Wextra",
    "-Werror",
]
DEBUG_FLAGS = ["-O0", "-g", "-DSPDLOG_ACTIVE_LEVEL=1"]
RELEASE_FLAGS = ["-O2"]
FAST_FLAGS = ["-O1"]

def _curve_copts():
    return select({
        "//bazel:curve_build_as_release": RELEASE_FLAGS,
        "//bazel:curve_build_as_debug": DEBUG_FLAGS,
        "//bazel:curve_build_as_fast": FAST_FLAGS,
        "//conditions:default": FAST_FLAGS,
    }) + WARNING_FLAGS

def curve_cc_binary(
        linkopts = [],
        copts = [],
        **kargs):
    cc_binary(
        linkopts = linkopts,
        copts = copts + _curve_copts(),
        **kargs
    )

def curve_cc_library(
        linkopts = [],
        copts = [],
        deps = [],
        **kargs):
    cc_library(
        linkopts = linkopts + OMP_LINKFLAGS,
        copts = _curve_copts() + copts + OMP_CFLAGS,
        deps = deps + [
            "@spdlog//:spdlog",
        ] + OMP_DEPS,
        **kargs
    )

def _curve_version_file_impl(ctx):
    out = ctx.actions.declare_file(ctx.attr.filename)
    ctx.actions.write(
        output = out,
        content = "__version__ = \"{}\"\n".format(ctx.attr.version),
    )
    return [DefaultInfo(files = depset([out]))]

curve_version_file = rule(
    implementation = _curve_version_file_impl,
    attrs = {
        "version": attr.string(),
        "filename": attr.string(),
    },
)

def curve_cc_test(
        linkopts = [],
        copts = [],
        deps = [],
        **kwargs):
    cc_test(
        # -lm for tcmalloc
        linkopts = linkopts + ["-lm", "-ldl"],
        copts = _curve_copts() + copts,
        deps = [
            "@com_google_googletest//:gtest_main",
        ] + deps,
        **kwargs
    )
