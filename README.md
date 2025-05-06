# appsec-review-smoke-cpp

A deliberately vulnerable **smoke target** for [appsec-review](https://github.com/wsollers/appsec-review).
It exists so that a review of a tiny, fully understood C and C++ program has one correct answer at
every stage. **Do not deploy, run against untrusted input, or reuse any of this code.**

The program is a toy request server: `smoke_server` reads one request line from a socket and
dispatches it; `smoke_synthetic` is the code-graph fixture program.

## License

MIT; see `LICENSE`. Vendored third-party code under `third_party/` keeps its own licenses.
