# appsec-review-smoke-cpp

A deliberately vulnerable **smoke target** for [appsec-review](https://github.com/wsollers/appsec-review).
It exists so that a review of a tiny, fully understood C and C++ program has one correct answer at
every stage. **Do not deploy, run against untrusted input, or reuse any of this code.**

The program is a toy request server: `smoke_server` reads one request line from a socket and
dispatches it; `smoke_synthetic` is the code-graph fixture program.

## Build

One CMake project, no network, no generated code:

```sh
cmake -S . -B build && cmake --build build
```

The build is expected to succeed in the appsec-review `native` build image (gcc 13.3, CMake 3.28)
as an unprivileged user with networking disabled.

## History

`tools/make-history.sh` rebuilds this repository's commit graph from `tools/history/steps/` with
fixed authors and dates, so the history is reproducible and can be reviewed as code.

## License

MIT; see `LICENSE`. Vendored third-party code under `third_party/` keeps its own licenses.
