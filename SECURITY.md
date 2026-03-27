# Security notes

This repository is a deliberately vulnerable smoke target. It is never deployed, so there is no
disclosure process. The entries below record the issues that were fixed in this history, so that
the record of what changed and why is in one place.

## 2026-03-02: path traversal in `read_asset` (CWE-22)

The `ASSET` verb joined the client-supplied name onto the asset root and opened the result, so a
name containing `../` read files outside the asset directory. `read_asset` now canonicalises the
joined path with `realpath()` and refuses any result that does not start with the asset root.

## 2026-02-20: command injection in `run_tool` (CWE-78)

The `TOOL` verb built a shell command from the client-supplied tool name and ran it with
`system()`, so shell metacharacters in the name ran arbitrary commands. `run_tool` now accepts
only the names `status` and `version` and performs them directly; there is no command string.

## 2026-02-05: vendored cJSON 1.7.16 (CVE-2023-50472)

cJSON 1.7.16 could read past the end of a buffer in `cJSON_SetValuestring`. The vendored copy was
replaced with 1.7.17, which fixes the length handling.
